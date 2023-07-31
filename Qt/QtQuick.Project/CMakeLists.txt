cmake_minimum_required(VERSION 3.23)

# 프로젝트 이름
project("QtQuick.Project")

# Qt의 시그널 기능을 만들어주는 MOC 활성화  
set(CMAKE_AUTOMOC ON)

# .qrc 파일을 자동으로 리소스 타겟에 포함 시키기 위해 사용
set(CMAKE_AUTORCC ON)

# C++ 버전
set(CMAKE_CXX_STANDARD 17)

# QT_DIR 설정, Qt5Config.cmake 혹은 Qt6Config.cmake 파일의 경로를 적어준다.  
set(QT_DIR "C:/Users/DP91-HSK/Documents/Libraries/5.15.10-MSVC-x64-shared/lib/cmake/Qt5")

# Qt 모듈 포함
# 해당 라인이 수행되어야 QT_VERSION_MAJOR가 정의된다.
# 일단 QT_VERSION_MAJOR 선언이 목적이기에 Core 모듈만 포함한다.
find_package(QT NAMES Qt6 Qt5 REQUIRED Core)

# Major Version에 대한 경로도 설정해줘야 한다.
set(Qt${QT_VERSION_MAJOR}_DIR ${QT_DIR})

# Qt 모듈 포함
find_package(Qt${QT_VERSION_MAJOR} REQUIRED Quick Qml)

# 소스 파일들을 SRC_FILES 변수에 저장
file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# qrc 파일들을 QRC_FILES 변수에 저장
# CMAKE_AUTORCC 옵션이 꺼져있다면 밑 함수를 주석 처리하자.
file(GLOB QRC_FILES CONFIGURE_DEPENDS ./*.qrc)

# 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
# qrc 파일도 함께 넣어줘야 함
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${QRC_FILES})

# 포함한 모듈들을 링크해준다.  
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
     COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> --qmldir ${CMAKE_SOURCE_DIR}/QmlDir ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
     WORKING_DIRECTORY ${QT_DIR}/../../../bin
)