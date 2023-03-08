# VS Code에서 Qt 빌드하는 법  

CMake와 VS Code를 사용하는 환경에서 Qt를 빌드하는 방법을 알아보자.  
해당 빌드 방법은 Windows 10 운영체제에서 Visual Studio 2022를 이용하여 테스트되었다.  
&nbsp;  

## 유의 사항  

Qt에는 다양한 모듈이 존재하는데 각 모듈마다 적용된 라이센스가 다르다.  
오픈 소스 모듈이여도 GPLv3과 LGPLv3로 나뉜다.  
따라서 코드 공개가 꺼려진다면 LGPLv3 라이센스가 적용된 모듈들만 사용하여 제품을 만들어야 한다.  
Qt 모듈 리스트는 https://doc.qt.io/qt-5/qtmodules.html 에서 확인이 가능하다.  
각 모듈마다 적용된 라이센스는 https://www.qt.io/product/features 에서 확인할 수 있다.  
문제는 LGPLv3 라이센스의 설명이 난해하다는 것이다.   
단순히 동적 링크를 하여 Qt 모듈들을 이용하면 소스 코드 공개의무가 없다고 생각하면 편하다.  
정적 링크도 코드 공개 의무를 피해갈 수 있으나 Qt 관련 모듈 코드를 같이 배포해야 하는 등의 번거로움이 생긴다.  
&nbsp;  

## 사전 준비  

Qt 소스 코드 빌드에 대한 공식 가이드 라인은 https://doc.qt.io/qt-5/windows-building.html 에 적혀있다. (Linux: https://doc.qt.io/qt-5/linux-building.html / MacOS: https://doc.qt.io/qt-5/macos-building.html)  

1. Qt 소스 코드 다운로드  
    https://download.qt.io/archive/qt/ 링크에서 원하는 버전의 Qt를 다운로드 받는다. (굉장히 오래걸릴 것이다...)   
    다운 받은 파일은 보통 ```qt-everywhere-opensource-src-[Qt 버전].zip``` 이런 이름일 것이다.  

2. Qt 모듈 사전 준비  
    받은 파일을 ```C:\Qt\[Qt 버전]``` 폴더에 풀자. (폴더 경로 중 띄어쓰기만 없다면 다른 곳에 풀어도 된다.)  
    필자는 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로에 풀었다.  
    그러면 configure.bat 파일이 ```D:\Projects\Development\Qt\Qt-5.15.8\configure.bat``` 이렇게 위치한 형태가 갖춰질 것이다.  
    설명도 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로 기준으로 하겠다.  

3. 필요 항목 다운로드  
    Visual Studio, Jom, Python을 설치해야 한다.  

    1. Visual Studio  
        https://visualstudio.microsoft.com/ko/ 링크에서 설치 파일 다운로드하고 설치 진행하자.  

    2. Jom  
        https://download.qt.io/official_releases/jom/ 링크에서 최신 버전의 Jom을 다운로드 한다.  
        압출 풀고 나온 모든 녀석들을 ```D:\Projects\Development\Qt\Qt-5.15.8``` 경로에 풀어준다.  
        ```D:\Projects\Development\Qt\Qt-5.15.8\jom.exe``` 형태가 갖춰져야 한다.  

    3. Python  
        https://www.python.org/ 링크에서 바로 설치해도 되지만 chocolate를 이용한 설치 방법이 좀 더 범용적이다.  
        밑은 chocolate 설치 후 chocolate를 이용해 python을 설치하는 방법이다.  

        1. Microsoft Store에 들어가서 Windows Terminal을 설치해준다. (그냥 깔려져있는 Powershell을 이용해도 되지만 편의성을 위해서 설치해준다.)  

        2. Windows Terminal에서 PowerShell 탭을 열고 ```$PSVersionTable``` 명령어를 수행한 뒤에 출력된 PSVersion이 3 이상인지 확인한다. (3이하라면 PowerShell을 업데이트해준다.)  

        3. .NET Framework 4.5 버전 이상이 설치되어 있는지 확인한다. (Visual Studio에서 ```.NET 테스크톱 개발``` 항목을 선택하면 알아서 최신버전의 .NET Framework를 설치해준다.)  

        4. Windows Terminal을 관리자 모드로 열고 PowerShell 탭을 띄운후에 Get-ExecutionPolicy 명령어를 수행 후에 Restricted인지 확인한다.  
        Restricted이라면 Set-ExecutionPolicy AllSigned 명령어를 추가적으로 수행해준다.  

        5. ```Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]       ::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))``` 명령어를 수행해 Chocolatey를 설치해준다.  

        6. 설치가 끝났으면 Windows Terminal를 관리자 모드로 재시작해주고 PowerShell 탭을 띄우고 choco 명령어를 수행해 Chocolatey가 잘 설치되었는지 확인한다.  

        7. ```choco install python```으로 python을 설치해준다.  

4. 환경 변수 세팅  
    MSVC 컴파일러를 사용할 경우 ```D:\Projects\Development\Qt```에 qt5vars.cmd 파일을 만들어 준다.  
    cmd 파일 내용은 밑과 같다.  
    ```cmd
    CALL "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" amd64
    SET _ROOT=D:\Projects\Development\Qt\Qt-5.15.8
    SET PATH=%_ROOT%\qtbase\bin;%_ROOT%\gnuwin32\bin;%PATH%
    SET _ROOT=
    ```
    vcvarsall.bat의 위치는 Visual Studio 버전에 따라 상이하니 직접 경로를 찾아보고 넣어줘야 한다.  
    amd64 부분은 64-bit로 빌드할 때 사용된다. (자신이 32-bit를 빌드해야 한다면 이 부분을 x86으로 바꾸자.)  
    _ROOT 경로도 자신이 설치한 Qt 경로에 맞게 수정해준다.  
    맨 밑 두 줄은 수정 없이 그대로 놓아두면 된다.  

    MSVC 이외의 GCC, Clang 컴파일러를 사용할 경우 qt5vars.cmd를 호출하는 대신 ```고급 시스템 설정 -> 환경 변수``` 로 이동하여 Path 항목에 Qt 라이브러리가 설치될 bin 경로를 추가하면 된다.  
    Qt 라이브러리가 설치될 bin 경로는 라이브러리 세팅 단계의 -prefix 옵션에 따라 달라지므로 추후에 다루겠다.  

5. qt5vars.cmd 수행  
    cmd 창을 관리자 모드로 열고 ```%SystemRoot%\system32\cmd.exe /E:ON /V:ON /k D:\Projects\Development\Qt\qt5vars.cmd``` 명령을 수행한다.  
    ```
    **********************************************************************
    ** Visual Studio 2022 Developer Command Prompt v17.5.1
    ** Copyright (c) 2022 Microsoft Corporation
    **********************************************************************
    [vcvarsall.bat] Environment initialized for: 'x64'
    ```
    Visual Studio 2022에서는 정상적으로 되었다면 위와 같은 처리 텍스트가 뜬다.  

6. 라이브러리 옵션 설정  
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
            MSVC로 윈도우에서 컴파일   

        * linux-clang  
            Clang으로 리눅스에서 컴파일       
    만약 Visual Studio 2022로 윈도우에서 Qt 모듈을 빌드하고 싶다면 ```-platform win32-msvc2022``` 옵션을 추가하면 된다.  
    이 외에도 많은 플랫폼을 지원한다.  
    자세한 내용은 https://doc.qt.io/qt-5/supported-platforms.html 링크를 참고하자.  

    * -nomake  
    빌드하기 싫은 모듈을 정할 수 있다.  
    qtquick3d 모듈이 GPLv3 라이센스라 껄끄러워 빌드하기 싫으면 ```-nomake qtquick3d``` 옵션을 추가하면 된다.  

    * -prefix  
    별도로 빌드 모듈이 산출될 폴더를 지정하고 싶다면 해당 옵션을 지정한다.  
    해당 옵션을 사용하면 따로 install 명령을 수행해야 Qt 라이브러리 설치가 진행된다.  

    * -shared  
    Qt 라이브러리를 공유 링크로 사용한다.  

    * -static  
    Qt 라이브러리를 정적 링크로 사용한다.  

    필자는 ```configure -debug-and-release -opensource -shared -confirm-license -platform win32-msvc2022 -prefix D:\Projects\Development\Qt\5.15.8-MSVC-x64-shared``` 명령어를 수행하여 옵션 설정을 하겠다.  
    더 많은 옵션에 대한 정보는 ```configure -help```를 통해 알 수 있다.  

7. 빌드 및 설치  
    열려있는 cmd 창에서 ```jom``` 명령어를 수행해 빌드를 진행한다.  
    빌드가 완료되면 ```jom install``` 명령어를 수행해 설치를 진행한다.  
    설치까지 완료했으면 컴퓨터를 재부팅한다.  