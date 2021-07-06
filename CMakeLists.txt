cmake_minimum_required(VERSION 3.19)
project(libaegis)

set(CMAKE_CXX_STANDARD 20)

#option(AEGIS_DEBUG "This controls whether or not to compile with debug options" OFF)
#if (AEGIS_DEBUG)
#    add_compile_definitions(AEGIS_DEBUG)
#endif()

#ption(AEGIS_NAMESPACE "This specifies the namespace to use for the library" aegis)
#dd_compile_definitions(AEGIS_NAMESPACE=${AEGIS_NAMESPACE})

find_package(OpenCV REQUIRED)

add_library(libaegis src/pipeline.cpp)

target_link_libraries(libaegis PUBLIC ${OpenCV_LIBS})

target_include_directories(libaegis PUBLIC includes ${OpenCV_INCLUDE_DIRS})