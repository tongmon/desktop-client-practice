cmake_minimum_required(VERSION 3.23)

execute_process(
  COMMAND vcpkg x-update-baseline --add-initial-baseline
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

project("Messenger_Server" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

# DB 연동을 위해 SOCI 포함
find_package(SOCI CONFIG REQUIRED)

# Boost Asio 포함
find_package(Boost REQUIRED COMPONENTS system filesystem json)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC 
    Boost::boost    
    Boost::system
    Boost::filesystem
    Boost::json
    SOCI::soci_core
    SOCI::soci_postgresql
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/Library/stb
)