cmake_minimum_required(VERSION 3.19)
project(libaegis)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_library(libaegis src/aegis.cpp)

target_link_libraries(libaegis PUBLIC ${OpenCV_LIBS})
target_include_directories(libaegis PUBLIC includes ${OpenCV_INCLUDE_DIRS})