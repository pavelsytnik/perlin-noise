cmake_minimum_required(VERSION 3.10)

project(Noise VERSION 1.0)

option(BUILD_SHARED_LIBS OFF)
option(SFML_USE_STATIC_STD_LIBS ON)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

add_executable(App WIN32 main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)

target_link_libraries(App sfml-graphics)
find_package(SFML 2.6.0 REQUIRED graphics network audio window system)
