# Build QT with VS Code, MSVC Compiler  

CMake와 VS Code 그리고 MSVC 컴파일러를 사용하는 환경에서 Qt를 빌드하는 방법을 알아보자.  
VS Code에서 CMake 빌드 환경 구축은 이미 되어있다는 가정 하에 설명한다.  
해당 빌드 방법은 Windows 10 운영체제에서 Visual Studio 2022를 이용하여 테스트되었다.  
&nbsp;  

## 유의 사항  

Qt에는 다양한 모듈이 존재하는데 각 모듈마다 적용된 라이센스가 다르다.  
오픈 소스 모듈이여도 GPLv3과 LGPLv3로 나뉜다.  
따라서 코드 공개가 꺼려진다면 LGPLv3 라이센스가 적용된 모듈들만 사용하여 제품을 만들어야 한다.  
Qt 모듈 리스트는 [이곳](https://doc.qt.io/qt-5/qtmodules.html)에서 확인이 가능하다.  
각 모듈마다 적용된 라이센스는 [이곳](https://www.qt.io/product/features)에서 확인할 수 있다.  

문제는 LGPLv3 라이센스의 설명이 난해하다는 것이다.   
단순히 ```동적 링크를 하여 Qt 모듈들을 이용하면 소스 코드 공개의무가 없다.```라고 생각하면 편하다.  
정적 링크도 코드 공개 의무를 피해갈 수 있으나 obj 코드를 공개해야 하기에 껄끄럽다.  
Qt 라이센스에 관해 [이곳](https://embeddeduse.com/2023/01/06/using-qt-5-15-and-qt-6-under-lgplv3/)에서 자세하게 정리해 놓았으니 확인해보자.  
&nbsp;  

## Qt 모듈 빌드   

Qt 소스 코드 빌드에 대한 공식 가이드 라인은 [이곳](https://doc.qt.io/qt-5/windows-building.html)에 적혀있다. (Linux: [링크](https://doc.qt.io/qt-5/linux-building.html) / MacOS: [링크](https://doc.qt.io/qt-5/macos-building.html))  

1. **Qt 소스 코드 다운로드**  
    [이곳](https://download.qt.io/archive/qt/)에서 원하는 버전의 Qt를 다운로드 받는다. (굉장히 오래걸릴 것이다...)   
    다운 받은 파일은 보통 ```qt-everywhere-opensource-src-[Qt 버전].zip``` 이런 이름일 것이다.  

2. **Qt 모듈 사전 준비**  
    받은 파일을 ```C:\Qt\[Qt 버전]``` 폴더에 풀자. (폴더 경로 중 띄어쓰기만 없다면 어떤 곳에 풀어도 상관없다.)  
    필자는 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로에 풀었다.  
    그러면 configure.bat 파일이 ```D:\Projects\Development\Qt\Qt-5.15.8\configure.bat``` 이렇게 위치한 형태가 갖춰질 것이다.  
    설명도 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로 기준으로 하겠다.  

3. **필요 항목 다운로드**  
    Visual Studio, Jom, Python을 설치해야 한다.  

    1. **Visual Studio**  
        [이곳](https://visualstudio.microsoft.com/ko/)에서 설치 파일 다운로드하고 설치 진행하자.  

    2. **Jom**  
        Visual Studio의 nmake를 사용할 것이라면 건너뛰어도 된다.  
        [이곳](https://download.qt.io/official_releases/jom/)에서 최신 버전의 Jom을 다운로드 한다.  
        압출 풀고 나온 모든 녀석들을 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로에 풀어준다.  
        ```D:\Projects\Development\Qt\Qt-5.15.8\jom.exe``` 형태가 갖춰져야 한다.  

    3. **Python**  
        Qt를 빌드할 때는 python2를 설치하는 것이 권장되는데 이유는 Qt WebEngine, Qt Pdf 모듈을 빌드할 때 python2가 필요하기 때문이다.  
        그 외의 python 버전은 Qt WebEngine, Qt Pdf 모듈을 빌드하지 못한다.  
        [이곳](https://www.python.org/)에서 바로 설치해도 되지만 chocolate를 이용한 설치 방법이 좀 더 범용적이다.  
        밑은 chocolate 설치 후 chocolate를 이용해 python2을 설치하는 방법이다.  

        1. Windows Terminal 설치  
            Microsoft Store에 들어가서 Windows Terminal을 설치해준다. (그냥 깔려져있는 Powershell을 이용해도 되지만 편의성을 위해서 설치해준다.)  

        1. PowerShell 버전 확인  
            Windows Terminal에서 PowerShell 탭을 열고 ```$PSVersionTable``` 명령어를 수행한 뒤에 출력된 PSVersion이 3 이상인지 확인한다. (3이하라면 PowerShell을 업데이트해준다.)  

        1. .NET Framework 설치  
            .NET Framework 4.5 버전 이상이 설치되어 있는지 확인한다.  
            Visual Studio에서 ```.NET 테스크톱 개발``` 항목을 선택하면 알아서 최신버전의 .NET Framework를 설치해준다.  

        1. ExecutionPolicy 확인  
            Windows Terminal을 관리자 모드로 열고 PowerShell 탭을 띄운후에 Get-ExecutionPolicy 명령어를 수행 후에 Restricted인지 확인한다.  
            Restricted이라면 Set-ExecutionPolicy AllSigned 명령어를 추가적으로 수행해준다.  

        1. Chocolatey 설치  
            ```Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]       ::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))``` 명령어를 수행해 Chocolatey를 설치해준다.  

        1. Chocolatey 설치 확인  
            설치가 끝났으면 Windows Terminal를 관리자 모드로 재시작해주고 PowerShell 탭을 띄우고 choco 명령어를 수행해 Chocolatey가 잘 설치되었는지 확인한다.  

        1. python 버전 2 설치  
            ```choco install python2```으로 python2를 설치해준다.  

        1. python 설치 버전 확인  
            만약 python2 보다 상위 버전의 python이 이미 설치되어 있는 경우 python2의 환경 변수를 덮어씌우고 있어 ```python --version``` 명령어를 수행해도 이미 설치된 상위 버전의 python 버전으로 출력될 것이다.  
            따라서 Qt 빌드 시에는 python2 보다 상위 버전의 python에서 설정한 환경 변수 Path를 잠시 빼주자.  
            예를 들어 python 2.7.18 버전을 사용하고 싶은데 python 3.1.1 버전이 이미 설치되어 있다면 보통 환경 변수 path에 ```C:\Python311```, ```C:\Python311\Scripts\``` 요런 녀석들이 들어 있을텐데 저 두 경로를 Qt 빌드할 때 빼주자.  

4. **환경 변수 세팅**  
    ```D:\Projects\Development\Qt```에 qt5vars.cmd 파일을 만들어 준다.  
    cmd 파일 내용은 밑과 같다.  
    ```batch
    CALL "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" amd64
    SET _ROOT=D:\Projects\Development\Qt\Qt-5.15.8
    SET PATH=%_ROOT%\qtbase\bin;%_ROOT%\gnuwin32\bin;%PATH%
    SET _ROOT=
    ```
    vcvarsall.bat의 위치는 Visual Studio 버전에 따라 상이하니 직접 경로를 찾아보고 넣어줘야 한다.  
    amd64 부분은 64-bit로 빌드할 때 사용된다. (자신이 32-bit를 빌드해야 한다면 이 부분을 x86으로 바꾸자.)  
    _ROOT 경로도 자신이 설치한 Qt 경로에 맞게 수정해준다.  
    맨 밑 두 줄은 수정 없이 그대로 놓아두면 된다.  

5. **qt5vars.cmd 수행**  
    cmd 창을 관리자 모드로 열고 ```%SystemRoot%\system32\cmd.exe /E:ON /V:ON /k D:\Projects\Development\Qt\qt5vars.cmd``` 명령을 수행한다.  
    ```
    **********************************************************************
    ** Visual Studio 2022 Developer Command Prompt v17.5.1
    ** Copyright (c) 2022 Microsoft Corporation
    **********************************************************************
    [vcvarsall.bat] Environment initialized for: 'x64'
    ```
    Visual Studio 2022에서는 정상적으로 되었다면 위와 같은 처리 텍스트가 뜬다.  

6. **라이브러리 옵션 설정**  
    cmd 창에서 ```cd D:\Projects\Development\Qt\Qt-5.15.8``` 명령으로 현재 위치를 바꿔준다.  
    현재 상태에서 configure 명령을 통해 Qt 모듈들을 빌드하게 되는데 옵션이 많다.  
    필수적인 옵션 몇 가지를 보자.  

    * -debug-and-release  
    문자 그대로 디버그, 릴리즈 모듈 모두를 빌드한다.  

    * -opensource    
    오픈 소스 전용 모듈만 빌드한다.  

    * -platform   
    타겟 플랫폼 전용으로 빌드한다.  
        * win32-g++  
            Gcc로 윈도우에서 컴파일  

        * win32-msvc  
            MSVC로 윈도우에서 컴파일 (설치된 MSVC 중 최신 버전을 알아서 선택해준다.)     

        * linux-clang  
            Clang으로 리눅스에서 컴파일  
    Visual Studio 2022로 윈도우에서 Qt 모듈을 빌드하고 싶다면 ```-platform win32-msvc2022``` 옵션을 추가하면 된다.  
    이 외에도 많은 플랫폼을 지원한다.  
    자세한 내용은 [이곳](https://doc.qt.io/qt-5/supported-platforms.html)을 참고하자.  

    * -nomake  
    빌드하기 싫은 파트를 정할 수 있다.  
    가능한 파트는 examples, libs, tests, tools 정도가 있다.  
    예시를 빌드하는 시간도 꽤 걸리기 때문에 이를 제외하고 싶다면 ```-nomake examples``` 옵션을 추가하면 된다.  
    
    * -skip  
    특정 모듈을 빌드하기 싫은 경우 지정할 수 있다.  
    Qt WebEngine 모듈은 LGPLv3 라이선스가 아닌데다가 Qt WebView로 대체가 가능하기 때문에 빌드하기 싫다면 ```-skip qtwebengine``` 옵션을 추가하면 된다.  

    * -prefix  
    별도로 빌드 모듈이 산출될 폴더를 지정하고 싶다면 해당 옵션을 지정한다.  
    해당 옵션을 사용하면 따로 install 명령을 수행해야 Qt 라이브러리 설치가 진행된다.  

    * -shared  
    Qt 라이브러리를 공유 링크로 사용한다.  

    * -static  
    Qt 라이브러리를 정적 링크로 사용한다.  

    필자는 LGPLv3 라이센스가 적용되지 않는 대부분의 모듈들을 제외하기 위해 밑과 같은 명령을 사용하겠다.  
    ```cmd
    configure -debug-and-release -opensource -skip qtwebengine -skip qtactiveqt -skip qtcharts -skip qtdatavis3d -skip qtnetworkauth -skip qtvirtualkeyboard -skip qtquick3d -skip qtquicktimeline -skip qtlottie -shared -confirm-license -platform win32-msvc2022 -prefix "D:\Projects\Development\Qt\5.15.8-MSVC-x64-shared"
    ```
    더 많은 옵션에 대한 정보는 ```configure -help```를 통해 알 수 있다.  

7. **빌드 전 주의사항**  
    다른 모듈은 크게 상관이 없는데 Qt WebEngine 모듈을 빌드해야 한다면 반드시 Windows의 시스템 locale을 영어로 변경해줘야 한다.  
    Qt WebEngine에서 요구하는 chromium을 빌드할 때 ninja가 사용되는 데 ninja가 한국어 관련 오류를 발생시키기에 필수적이다.  
    왠만하면 Qt를 빌드할 때 만이라도 locale을 영어로 설정하고 빌드하자.  
    Windows 10에서는 ```설정``` -> ```시간 및 언어``` -> ```언어``` -> ```기본 언어 설정``` -> ```시스템 로켈 변경``` 항목에서 변경이 가능하고 설정 후에는 재부팅을 해야 한다.  

8. **빌드 및 설치**  
    ```Developer Command Prompt for [Visual Studio 버전]```을 관리자 모드로 열어 ```cd D:\Projects\Development\Qt\Qt-5.15.8```로 Qt 소스 코드 위치로 이동한다.  
    ```jom``` (nmake 사용자는 ```nmake```) 명령어를 수행하여 빌드를 진행한다.  
    빌드가 완료되면 ```jom install``` (nmake 사용자는 ```nmake install```) 명령어를 수행해 설치를 진행한다.  
    설치까지 완료했으면 컴퓨터를 재부팅한다.  

9.  **재빌드**  
    QT의 configure 옵션을 바꾸고 싶은 경우가 있을 것이다.  
    그럴때는 소스 파일 날리고 다시 압축 풀어서 처음부터 진행하자.  
    cmd 창에서 소스 파일 위치인 ```cd D:\Projects\Development\Qt\Qt-5.15.8```로 이동해서 ```jom clean``` (nmake 사용자는 ```nmake distclean```) 명령어를 수행하는 법도 있긴한데 잔여 캐시 파일 때문인지 재빌드가 잘 안된다.  
&nbsp;  

## Qt 활용 프로젝트 빌드  

Qt가 모두 설치되었으면 Qt를 활용해야 한다.  
VS Code에서 CMake를 이용한 프로젝트를 만들어보자.  
필자가 사용하는 MSVC 컴파일러를 기반으로 설명하겠다.  
다른 컴파일러를 사용중이라면 CMakePresets.json의 구성을 컴파일러에 맞게 수정해줘야 한다.  
&nbsp;  

### Qml 프로젝트  

Qml 프로젝트 구성은 밑과 같다.  
```
QtQuick.Project
|- QmlDir
    |- main.qml
|- CMakeLists.txt
|- CMakePresets.json
|- main.cpp
|- qml.qrc
```

* CMakeLists.txt  
    CMakeLists.txt 파일만 잘 보면 다른 것들은 별 문제가 되지 않는다.  
    ```cmake
    cmake_minimum_required(VERSION 3.23)

    # 프로젝트 이름
    project("QtQuick.Project")

    # Qt의 시그널 기능을 만들어주는 MOC 활성화  
    set(CMAKE_AUTOMOC ON)

    # Qt Widget 모듈과 .ui 파일을 사용한다면 밑 주석을 해제하자.  
    # set(CMAKE_AUTOUIC ON)

    # .qrc 파일을 자동으로 리소스 타겟에 포함 시키기 위해 사용
    set(CMAKE_AUTORCC ON)

    # C++ 버전
    set(CMAKE_CXX_STANDARD 17)

    # Qt5Config.cmake 파일의 경로를 적어준다.  
    set(Qt5_DIR "D:\\Projects\\Development\\Qt\\5.15.8-MSVC-x64-shared\\lib\\cmake\\Qt5")

    # Qt 모듈 포함 (현재는 Quick, Qml만 포함)
    find_package(Qt5 REQUIRED Quick Qml)

    # 소스 파일들을 SRC_FILES 변수에 저장
    file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

    # qrc 파일들을 QRC_FILES 변수에 저장
    # CMAKE_AUTORCC 옵션이 꺼져있다면 밑 함수를 주석 처리하자.
    file(GLOB QRC_FILES CONFIGURE_DEPENDS ./*.qrc)

    # CMAKE_AUTORCC 옵션이 꺼져있다면 밑 함수를 주석 해제하자.
    # qt5_add_resources(QRC_FILES qml.qrc)

    # 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
    # qrc 파일도 함께 넣어줘야 함  
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${QRC_FILES})

    # 포함한 모듈들을 링크해준다.  
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        Qt5::Quick
        Qt5::Qml
    )
    ```
    Qt5_DIR 설정은 ```[Qt 설치 경로]/lib/cmake/[Qt5 or Qt6]```로 해주면 된다.  

* CMakePresets.json
    ```json
    {
        "version": 4,
        "cmakeMinimumRequired": {
            "major": 3,
            "minor": 23,
            "patch": 0
        },
        "configurePresets": [
            {
                "description": "윈도우 빌드 전용 / 컴파일 빌드 폴더와 설치 폴더 지정",
                "name": "windows-base",
                "hidden": true,
                "binaryDir": "${sourceDir}/Build/${presetName}",
                "installDir": "${sourceDir}/Installed/${presetName}",
                "toolchainFile": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
                "condition": {
                    "type": "equals",
                    "lhs": "${hostSystemName}",
                    "rhs": "Windows"
                }
            },
            {
                "description": "MSVC 컴파일러 사용",
                "name": "msvc",
                "hidden": true,
                "generator": "Visual Studio 17 2022",
                "inherits": "windows-base"
            },
            {
                "description": "MSVC 컴파일러로 x64 빌드",
                "name": "msvc-x64",
                "displayName": "MSVC x64",
                "inherits": "msvc",
                "architecture": {
                    "value": "x64",
                    "strategy": "set"
                },
                "cacheVariables": {
                    "VCPKG_TARGET_TRIPLET": "x64-windows-static",
                    "CMAKE_CXX_FLAGS": "/MP /D_UNICODE /DUNICODE /D_CRT_SECURE_NO_WARNINGS /JMC /permissive- /EHsc",
                    "CMAKE_MSVC_RUNTIME_LIBRARY": "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
                }
            },
            {
                "description": "MSVC 컴파일러로 x86 빌드",
                "name": "msvc-x86",
                "displayName": "MSVC x86",
                "inherits": "msvc",
                "architecture": {
                    "value": "Win32",
                    "strategy": "set"
                },
                "cacheVariables": {
                    "VCPKG_TARGET_TRIPLET": "x86-windows-static",
                    "CMAKE_CXX_FLAGS": "/DWIN32 /MP /D_UNICODE /DUNICODE /D_CRT_SECURE_NO_WARNINGS /JMC /permissive- /EHsc",
                    "CMAKE_MSVC_RUNTIME_LIBRARY": "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
                }
            }
        ],
        "buildPresets": [
            {
                "name": "msvc-base-build-settings",
                "hidden": true,
                "nativeToolOptions": [
                    "/maxcpucount",
                    "/nologo",
                    "/verbosity:minimal"
                ]
            },
            {
                "name": "msvc-x64-debug-build",
                "displayName": "Debug Build",
                "inherits": "msvc-base-build-settings",
                "configuration": "Debug",
                "configurePreset": "msvc-x64"
            },
            {
                "name": "msvc-x64-release-build",
                "displayName": "Release Build",
                "inherits": "msvc-base-build-settings",
                "configuration": "Release",
                "configurePreset": "msvc-x64"
            },
            {
                "name": "msvc-x86-debug-build",
                "displayName": "Debug Build",
                "inherits": "msvc-base-build-settings",
                "configuration": "Debug",
                "configurePreset": "msvc-x86"
            },
            {
                "name": "msvc-x86-release-build",
                "displayName": "Release Build",
                "inherits": "msvc-base-build-settings",
                "configuration": "Release",
                "configurePreset": "msvc-x86"
            }
        ],
        "testPresets": []
    }
    ```
    보통의 MSVC 컴파일러 세팅이다.  
    유의할 점은 빌드한 Qt가 shared인지 static인지에 따라 CMAKE_MSVC_RUNTIME_LIBRARY 옵션을 맞춰 설정해줘야 한다는 것이다.  
    필자는 shared로 Qt를 빌드했기에 ```MultiThreaded$<$<CONFIG:Debug>:Debug>DLL```로 설정하였다.  

* main.cpp
    ```c++
    #include <QDebug>
    #include <QGuiApplication>
    #include <QQmlApplicationEngine>

    int main(int argc, char *argv[])
    {
        qDebug() << "tongstar";

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/QmlDir/main.qml"));
        QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl)
            {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
    }
    ```
    별 특이한 건 없고 main.qml 파일을 호출한다.  

* qml.qrc
    ```qrc
    <RCC>
        <qresource prefix="/">
            <file>QmlDir/main.qml</file>
        </qresource>
    </RCC>
    ```
    qml 파일이 리소스로 등록되어 있다.  

* main.qml
    ```qml
    import QtQuick 2.12
    import QtQuick.Window 2.12
    import QtQuick.Controls 2.12
    import QtQuick.Layouts 1.12

    Window {
        visible: true
        width: 500
        minimumWidth: 200
        height: 300
        minimumHeight: 100
        title: "CMake and Qt Quick"

        ColumnLayout {
            anchors.fill: parent

            Text {
                id: txt
                Layout.fillWidth: true
                Layout.fillHeight: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "tongstar"
                font.family: "Verdana"
                font.pointSize: 30
                wrapMode: Text.WordWrap
            }

            Button {
                Layout.fillWidth: true
                text: "some button"
                onClicked: {
                    txt.text = "Some random number: ".concat(Math.floor((Math.random() * 100) + 1))
                }
            }
        }
    }
    ```
    버튼을 누르면 랜덤한 숫자를 띄우는 예제이다.  

자세한 내용은 같은 레포지토리 경로에 있는 QtQuick.Project를 확인해라.  
&nbsp;  

#### 배포  

Qt 프로젝트가 빌드 되었다고 하더라도 종속된 dll들이 같이 포함되지 않아서 실행이 안될 것이다.  
Qt가 설치된 bin 폴더를 보자.  
필자는 ```D:\Projects\Development\Qt\5.15.8-MSVC-x64-shared\bin``` 인데 해당 폴더 안에 windeployqt.exe 파일이 있을 것이다.  
요 녀석을 활용하여 배포할 수 있다.  
cmd를 관리자 모드로 켜서 ```cd D:\Projects\Development\Qt\5.15.8-MSVC-x64-shared\bin``` 명령어로 이동한다.  
```windeployqt.exe [디버그: --debug, 릴리즈: --release] --qmldir [Qml 파일들이 위치한 폴더 경로] [실행 파일 경로]``` 명령어를 실행하면 실행 파일이 수행되기 위한 dll 들이 모두 실행 파일 위치에 복사된다.  
필자는 ```windeployqt.exe --debug --qmldir D:\Projects\VSCode\QtQuick.Project\QmlDir D:\Projects\VSCode\QtQuick.Project\Build\msvc-x64\Debug\QtQuick.Project.exe```  명령을 수행했다.  
이제 실행 파일이 담긴 폴더를 압축해서 배포하면 된다.  
&nbsp;  

##### CMake를 통한 배포  

배포할 때마다 위와 같이 명령어를 치는 것은 번거로운 일이기에 CMake로 자동화를 시켜주자.  
```cmake
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
     COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> --qmldir ${CMAKE_SOURCE_DIR}/QmlDir ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
     WORKING_DIRECTORY ${Qt5_DIR}/../../../bin
)
```
위 구문을 해당 Qml 예제에서 다룬 CMakeLists.txt 파일의 최하단에 추가하면 빌드할 때 자동으로 배포를 위한 과정이 수행된다.  
&nbsp;  

### Widget 프로젝트  

프로젝트 구성은 밑과 같다.  
```
QtWidget.Project
|- CMakeLists.txt
|- CMakePresets.json
|- main.cpp
|- mainwindow.cpp
|- mainwindow.h
|- mainwindow.ui
```
Widget 프로젝트이기에 qml은 없고 ui 파일이 있다.  

* CMakeLists.txt
    ```cmake
    cmake_minimum_required(VERSION 3.23)

    # 프로젝트 이름
    project("QtWidget.Project")

    # Qt의 시그널 기능을 만들어주는 MOC 활성화  
    set(CMAKE_AUTOMOC ON)

    # .ui 파일과 소스 코드 연동을 위한 UIC 활성화
    set(CMAKE_AUTOUIC ON)

    # qt5_add_resources()와 같은 별도의 CMake 명령 없이 .qrc 파일을 리소스 타겟에 포함하려면 주석을 해제하자.
    # set(CMAKE_AUTORCC ON)

    # C++ 버전
    set(CMAKE_CXX_STANDARD 17)

    # Qt5Config.cmake 파일의 경로를 적어준다.  
    set(Qt5_DIR "D:\\Projects\\Development\\Qt\\5.15.8-MSVC-x64-shared\\lib\\cmake\\Qt5")

    # Qt 모듈 포함 (현재는 Widgets만 포함)
    find_package(Qt5 REQUIRED Widgets)

    # 소스 파일들을 SRC_FILES 변수에 저장
    file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

    # ui 파일들을 UI_FILES 변수에 저장
    file(GLOB UI_FILES CONFIGURE_DEPENDS ./*.ui)

    # 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
    # ui 파일도 함께 넣어줘야 함
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${UI_FILES})

    # 포함한 모듈들을 링크해준다.  
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        Qt5::Widgets
    )
    ```
    Qml 프로젝트와 크게 다른 점은 .ui 파일에 대한 처리이다.  
    .ui 파일을 연동하기 위해 CMAKE_AUTOUIC 옵션을 켜고 실행 파일 빌드 시에도 포함 시켰다.  

* CMakePresets.json  
    Qml 프로젝트와 동일하다.  

* main.cpp  
    ```c++
    #include "mainwindow.h"
    #include <QApplication>

    int main(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }
    ```
    Qml을 따로 이용하지 않기에 QApplication을 이용한다.  

* mainwindow.h  
    ```c++
    #ifndef MAINWINDOW_H
    #define MAINWINDOW_H

    #include <QMainWindow>

    namespace Ui
    {
    class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

      public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

      private:
        Ui::MainWindow *ui;

        void on_pushButton_Clicked();
    };

    #endif // MAINWINDOW_H
    ```
    클래스 이름이 MainWindow 것이 중요하다.  
    꼭 ui에 정의된 class 이름과 동일해야 한다.  

* mainwindow.cpp  
    ```c++
    #include "mainwindow.h"
    #include "ui_mainwindow.h"
    #include <cstdlib>
    #include <ctime>

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_Clicked);

        srand((unsigned int)time(NULL));
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::on_pushButton_Clicked()
    {
        ui->label->setText("Some random number: " + QString::number(rand() % 100 + 1));
    }
    ```
    1 ~ 99 사이의 난수를 도출하는 로직이 들어있다.  

* mainwindow.ui  
    ```ui
    <?xml version="1.0" encoding="UTF-8"?>
    <ui version="4.0">
     <class>MainWindow</class>
     <widget class="QMainWindow" name="MainWindow">
      <property name="geometry">
       <rect>
        <x>0</x>
        <y>0</y>
        <width>500</width>
        <height>300</height>
       </rect>
      </property>
      <property name="minimumSize">
       <size>
        <width>300</width>
        <height>100</height>
       </size>
      </property>
      <property name="windowTitle">
       <string>CMake and Qt Widgets</string>
      </property>
      <widget class="QWidget" name="centralWidget">
       <layout class="QVBoxLayout" name="verticalLayout">
        <item alignment="Qt::AlignHCenter">
         <widget class="QLabel" name="label">
          <property name="font">
           <font>
            <family>Verdana</family>
            <pointsize>30</pointsize>
           </font>
          </property>
          <property name="text">
           <string>tongstar</string>
          </property>
          <property name="wordWrap">
           <bool>true</bool>
          </property>
         </widget>
        </item>
        <item>
         <widget class="QPushButton" name="pushButton">
          <property name="minimumSize">
           <size>
            <width>0</width>
            <height>50</height>
           </size>
          </property>
          <property name="font">
           <font>
            <family>Verdana</family>
            <pointsize>20</pointsize>
           </font>
          </property>
          <property name="text">
           <string>some button</string>
          </property>
          <property name="flat">
           <bool>false</bool>
          </property>
         </widget>
        </item>
       </layout>
      </widget>
     </widget>
     <layoutdefault spacing="6" margin="11"/>
     <resources/>
     <connections/>
    </ui>
    ```
    주요하게 봐야될 부분은 ```<class>MainWindow</class>```와 ```<widget class="QMainWindow" name="MainWindow">```이다.  
    mainwindow.h에 정의된 클래스 이름과 MainWindow 부분이 동일한 것이 중요하다.  
    동일하지 않으면 비정상적으로 동작한다.  

자세한 내용은 같은 레포지토리 경로에 있는 QtWidget.Project에서 확인할 수 있다.  
&nbsp;  

#### 배포  

Qml 프로젝트와 별반 다르지 않고 --qmldir만 지정해주지 않을 뿐이다.  
cmd를 관리자 모드로 켜서 ```cd D:\Projects\Development\Qt\5.15.8-MSVC-x64-shared\bin``` 명령어로 이동한다.  
```windeployqt.exe [디버그: --debug, 릴리즈: --release] [실행 파일 경로]``` 명령어를 실행하면 실행 파일이 수행되기 위한 dll 들이 모두 실행 파일 위치에 복사된다.  
필자는 ```windeployqt.exe --debug D:\Projects\VSCode\QtWidget.Project\Build\msvc-x64\Debug\QtWidget.Project.exe``` 명령을 수행했다.  
이제 실행 파일이 담긴 폴더를 압축해서 배포하면 된다.  
&nbsp;  

##### CMake를 통한 배포  

Widget 프로젝트도 번거로운 배포 과정을 CMake로 자동화할 수 있다.  
```cmake
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
     COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
     WORKING_DIRECTORY ${Qt5_DIR}/../../../bin
)
```
위 구문을 해당 Widget 예제에서 다룬 CMakeLists.txt 파일의 최하단에 추가하면 빌드할 때 자동으로 배포를 위한 과정이 수행된다.  