cmake_minimum_required(VERSION 3.23)

# 프로젝트 이름
project("FrameLessWindow")

# Qt의 시그널 기능을 만들어주는 MOC 활성화  
set(CMAKE_AUTOMOC ON)

# .ui 파일과 소스 코드 연동을 위한 UIC 활성화
set(CMAKE_AUTOUIC ON)

# qt5_add_resources()와 같은 별도의 CMake 명령 없이 .qrc 파일을 리소스 타겟에 포함하려면 주석을 해제하자.
set(CMAKE_AUTORCC ON)

# C++ 버전
set(CMAKE_CXX_STANDARD 17)

# Qt5Config.cmake 파일의 경로를 적어준다.  
# C:\\Qt\\5_15_9_msvc_x64_shared\\lib\\cmake\\Qt5
# C:/Users/DP91-HSK/Documents/Libraries/5.15.10-MSVC-x64-shared/lib/cmake/Qt5
set(QT_DIR "C:/Users/DP91-HSK/Documents/Libraries/5.15.10-MSVC-x64-shared/lib/cmake/Qt5")

# Qt 모듈 포함
# 해당 라인이 수행되어야 QT_VERSION_MAJOR가 정의된다.
find_package(QT NAMES Qt6 Qt5 REQUIRED Core)

# Major Version에 대한 경로도 설정해줘야 한다.
set(Qt${QT_VERSION_MAJOR}_DIR ${QT_DIR})

find_package(Qt${QT_VERSION_MAJOR} REQUIRED Qml Quick QuickControls2)

# 소스 파일들을 SRC_FILES 변수에 저장
file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# ui 파일들을 UI_FILES 변수에 저장
file(GLOB UI_FILES CONFIGURE_DEPENDS ./*.ui)

# qrc 파일들을 UI_FILES 변수에 저장
file(GLOB QRC_FILES CONFIGURE_DEPENDS ./*.qrc)

# 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${UI_FILES} ${QRC_FILES})

# 포함한 모듈들을 링크해준다. 
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    dwmapi.lib
    gdi32.lib
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
     COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> --qmldir "${CMAKE_SOURCE_DIR}/Qmls"  "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
     WORKING_DIRECTORY "${QT_DIR}/../../../bin"
)
