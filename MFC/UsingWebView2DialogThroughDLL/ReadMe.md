# WebView2 다이얼로그로 활용하기

WebView2를 C++의 MFC를 이용하여 다이얼로그로 활용하는 프로젝트이다.  
Nuget Package 혹은 vcpkg를 통해 wil, webview2와 관련된 항목을 이용할 수 있다.  
해당 프로젝트에서는 vcpkg를 활용한다.  

WebViewDLL 프로젝트를 동적 라이브러리로 빌드하고 WebViewProject에서 이를 사용한다.  
동적 라이브러리를 통해 작동하기에 WebViewDLL과 WebViewProject 간의 C++ 버전이 달라도 상관이 없다는 것이 장점이다.  