cmake_minimum_required(VERSION 3.23)

execute_process(
  COMMAND vcpkg x-update-baseline --add-initial-baseline
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

project(BoostAsioPractice VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system)