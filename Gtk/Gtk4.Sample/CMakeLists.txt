cmake_minimum_required(VERSION 3.23)

# Vcpkg 버전 고정을 위한 명령어 입력(GTK4와 무관)
execute_process(
  COMMAND vcpkg x-update-baseline --add-initial-baseline
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# 윈도우 운영체제에서 GTK 앱을 구동시키면서 콘솔창을 같이 띄우지 않기 위해 GCC 컴파일러에 -mwindows 옵션을 넘겨준다.  
set(CMAKE_CXX_FLAGS "-mwindows ${CMAKE_CXX_FLAGS}")

project(Gtk4.Sample VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
# set(BUILD_SHARED_LIBS OFF)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# pkg-config를 이용해 MSYS2에서 설치된 gtk4 패키지 찾기
find_package(PkgConfig REQUIRED)
# IMPORTED_TARGET 옵션을 사용해 따로 헤더 파일 경로와 라이브러리 경로를 지정해주지 않아도 된다.  
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# GTK4 라이브러리 연동
target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::GTK4)