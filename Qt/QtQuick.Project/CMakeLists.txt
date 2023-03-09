cmake_minimum_required(VERSION 3.23)

# 프로젝트 이름
project("QtQuick.Project")

# Qt의 시그널 기능을 만들어주는 MOC 활성화  
set(CMAKE_AUTOMOC ON)

# Qt Widget 모듈과 .ui 파일을 사용한다면 밑 주석을 해제하자.  
# set(CMAKE_AUTOUIC ON)

# C++ 버전
set(CMAKE_CXX_STANDARD 17)

# Qt5Config.cmake 파일의 경로를 적어준다.  
set(Qt5_DIR "D:\\Projects\\Development\\Qt\\5.15.8-MSVC-x64-shared\\lib\\cmake\\Qt5")

# Qt 모듈 포함 (현재는 Quick, Qml만 포함)
find_package(Qt5 REQUIRED Quick Qml)

# 소스 파일들을 SRC_FILES 변수에 저장
file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# CMake 함수 qt5_add_resources()를 통해 리소스 파일을 등록해줘야 함.  
# 리소스 파일들은 qrc 변수에 저장됨
qt5_add_resources(qrc qml.qrc)

# 실행 파일 추가할 때 윈도우라면 꼭 WIN32를 정의해줘야 한다. (리눅스나 맥은 따로 안해줘도 됨)
# 리소스 파일도 함께 넣어줘야 함
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${qrc})

# 포함한 모듈들을 링크해준다.  
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt5::Quick
    Qt5::Qml
)