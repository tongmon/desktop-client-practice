cmake_minimum_required(VERSION 3.23)

# 프로젝트 이름
project("BorderLessApp")

file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    dwmapi.lib
)