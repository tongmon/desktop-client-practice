cmake_minimum_required(VERSION 3.23)

# 프로젝트 이름
project("QtWidget.Project")

# Qt의 시그널 기능을 만들어주는 MOC 활성화  
set(CMAKE_AUTOMOC ON)

# .ui 파일과 소스 코드 연동을 위한 UIC 활성화
set(CMAKE_AUTOUIC ON)

# .qrc 파일을 자동으로 리소스 타겟에 포함 시키기 위해 사용
set(CMAKE_AUTORCC ON)

# C++ 버전
set(CMAKE_CXX_STANDARD 17)

# Qt5Config.cmake 파일의 경로를 적어준다.  
set(Qt5_DIR "D:\\Projects\\Libraries\\Qt\\5_15_10_MSVC_x64_shared\\lib\\cmake\\Qt5")

# Qt 모듈 포함 (현재는 Widgets만 포함)
find_package(Qt5 REQUIRED Widgets Core Gui)

# 소스 파일들을 SRC_FILES 변수에 저장
file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# UI 파일들을 UI_FILES 변수에 저장
file(GLOB UI_FILES CONFIGURE_DEPENDS ./*.ui)

# qrc 파일들을 QRC_FILES 변수에 저장
file(GLOB QRC_FILES CONFIGURE_DEPENDS ./*.qrc)

# Private 포함 파일 추가
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

# 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
# UI 파일도 함께 넣어줘야 함
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${UI_FILES} ${QRC_FILES})

# 포함한 모듈들을 링크해준다.  
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    gdi32.lib
    dwmapi.lib
)

# 배포 자동화
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#      COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
#      WORKING_DIRECTORY ${Qt5_DIR}/../../../bin
# )