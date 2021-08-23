
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Clui VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Test1 Test1.cc)

target_link_libraries(Test1 ${CURSES_LIBRARIES})

