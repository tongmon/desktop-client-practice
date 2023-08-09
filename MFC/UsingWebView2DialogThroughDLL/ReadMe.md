# WebView2 다이얼로그로 활용하기

WebView2를 C++의 MFC를 이용하여 다이얼로그로 활용하는 프로젝트이다.  
Nuget Package 혹은 vcpkg를 통해 wil, webview2와 관련된 항목을 이용할 수 있다.  
해당 프로젝트에서는 vcpkg를 활용한다.  

WebViewDLL 프로젝트를 동적 라이브러리로 빌드하고 WebViewProject에서 이를 사용한다.  
동적 라이브러리를 통해 작동하기에 WebViewDLL과 WebViewProject 간의 C++ 버전이 달라도 상관이 없다는 것이 장점이다.  
하지만 프로젝트 간 멀티바이트, 유니코드 사용 종류는 동일해야 한다.  
&nbsp;  

## DLL Export 방식  

Windows에서 DLL의 특정 함수를 외부에서 이용하는 방법은 크게 두 가지가 있다.  

1. .def 파일을 이용하기  

2. __declspec(dllexport) 이용하기  

각 방식을 설명하기 위해 특정 상황을 가정해보자.  
&nbsp;  

일단 module.dll을 빌드하는 프로젝트에 밑과 같은 함수가 있다고 하자.  
```c++
void PrintHelloWorld()
{
	std::cout << "Hello World";
}
```
&nbsp;  

main.exe에서는 module.dll을 LoadLibrary() 함수를 통해 이용하는 밑과 같은 함수가 있다.  
```c++
void MainFunc()
{
	using SomeFunc = void(*)();

	HINSTANCE h_instance = LoadLibrary(_T("module.dll"));
	if (!h_instance)
		return;

	SomeFunc some_func = reinterpret_cast<SomeFunc>(GetProcAddress(h_instance, "PrintHelloWorld"));

	if (some_func)
		some_func();

	FreeLibrary(h_instance);
}
```
위와 같은 MainFunc를 작동시키기 위해서 각 방식이 어떻게 다른지 알아보자.  
&nbsp;  

### .def 파일 이용  

모듈 정의 파일을 이용하려면 일단 모듈 정의 파일의 경로를 지정해줘야 한다.  
Visual Studio IDE를 기준으로 ```링커 -> 입력 -> 모듈 정의 파일``` 위치에 올바른 .def 파일 경로를 적어준다.  
&nbsp;  

.def 파일의 내용에 PrintHelloWorld에 대한 정보를 적어야 한다.  
```
LIBRARY

EXPORTS
    PrintHelloWorld @1
```
위와 같이 .def를 수정해줬다면 MainFunc()는 잘 작동할 것이다.  
@1는 함수 순번이다.  
여러 함수를 Export 할 때는 ```함수 이름 @2``` 처럼 @ 뒤의 숫자를 증가시켜야 한다.  
&nbsp;  

### __declspec(dllexport) 이용  

일단 PrintHelloWorld 함수의 모습은 밑과 같이 바뀐다.  
```c++
__declspec(dllexport) void PrintHelloWorld()
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
	std::cout << "Hello World";
}
```
__declspec(dllexport)는 Windows에서 미리 정의된 매크로인 AFX_API_EXPORT로 대체할 수 있다.  
이렇게 함수 형태만 위와 같이 바꿔주면 따로 .def 파일 없이도 MainFunc가 잘 작동한다.  
&nbsp;  

## Name Mangling  

간혹 라이브러리 Export 할 때 LNK 4022 빌드 오류가 발생할 때가 있다.  
이는 Name Mangling과 관련있다.  
C는 함수 오버로딩을 지원하지 않기에 ```Sum(int a, int b)``` 함수의 이름은 Sum으로 정해진다.  
반면 C++은 ```Sum(int a, int b)```, ```Sum(double a, double b)```와 같이 동일한 이름의 각기 다른 함수를 구분해야 하는데 내부적으로 예를들어 ```Sum(int a, int b)```은 ```?Sum@HAY12```, ```Sum(double a, double b)```은 ```?Sum@HAZZ7``` 이렇게 이름이 바뀌게 된다.  

여기서 정의한 [def 파일](#def-파일-이용)을 보면 EXPORTS란에 PrintHelloWorld라고 함수 이름이 정직하게 쓰여져 있는데 이러면 LNK 4022 빌드 오류가 발생할 수 있다.  
def 파일에 적힌 PrintHelloWorld 이름을 Name Mangling이 발생한 후의 이름(ex: ?PrintHelloWorld@YAXHD@Z)으로 바꾸거나 ```extern "C"```를 이용해야 한다.  

```extern "C"```는 밑과 같이 사용할 수 있다.  
```c++
#ifdef __cplusplus
extern "C"
{
#endif 

	void PrintHelloWorld()
	{
		std::cout << "Hello World";
	}

#ifdef __cplusplus 
}
#endif 
```
C 파일은 ```extern "C"```를 이용할 필요가 없기에 ```__cplusplus```인 경우에만 Name Mangling을 방지한다.  
만약 PrintHelloWorld 함수가 .hpp, .cpp로 나누어진다면 함수 선언부, 정의부를 모두 ```extern "C"```로 덮어줘야 한다.  
&nbsp;  

## Libraray Runtime Type  

DLL을 Export하는 경우 런타임 라이브러리 유형을 주의해야 한다.  
특히 메모리의 할당과 해제가 발생하는 부분에서 특정 모듈과 메인 프로젝트의 런타임 라이브러리가 다르다면 실행 도중 런타임 에러가 발생한다.  
예시를 보면 이해가 쉽다.  
&nbsp;  

일단 module.dll에 선언된 밑과 같은 구조체가 있다.  
```c++
struct SomeParam
{
	std::string title;
	std::vector<int> ary;
};
```
&nbsp;  

module.dll에는 밑과 같은 함수도 있다.  
```c++
void Printer(SomeParam* param)
{
    std::cout << param->title << "\n";
    for (auto& item : param->ary)
        std::cout << item << " ";
}
```
해당 함수를 main.exe에서 LoadLibrary()를 통해 사용한다고 하자.  
&nbsp;  

module.dll을 이용하기 위해서 main.exe 프로젝트에서도 SomeParam이 정의되어 있어야 한다.  
main.exe에는 밑과 같은 구현이 존재한다.  
```c++
// module.dll의 것과 동일한 구조체를 main.exe에도 정의해줘야 한다.
struct SomeParam
{
	std::string title;
	std::vector<int> ary;
};

void MainFunc()
{
	using SomeFunc = void(*)(SomeParam*);

	HINSTANCE h_instance = LoadLibrary(_T("module.dll"));
	if (!h_instance)
		return;

	SomeFunc some_func = reinterpret_cast<SomeFunc>(GetProcAddress(h_instance, "PrintHelloWorld"));

	if (some_func) {
        SomeParam sp{ "Hello", {1,2,3} };
		some_func(&sp);
    }

	FreeLibrary(h_instance);
}
```
&nbsp;  

MainFunc가 잘 작동하기 위해서는 module.dll과 main.exe의 런타임 라이브러리가 모두 ```/MD(Release)```, ```/MDd(Debug)```여야 한다.  
만약 각 모듈의 런타임 라이브러리 유형이 다르거나 /MT, /MTd라면 각 모듈은 독립적인 힙 영역을 사용하게 되고 std::string, std::vector와 같은 자료형에서 할당 해제가 진행될 때 문제가 발생하게 된다.  