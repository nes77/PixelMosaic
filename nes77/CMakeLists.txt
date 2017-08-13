cmake_minimum_required(VERSION 3.8)
project(pixl_mosaic)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PNG REQUIRED)

include_directories (${PNG_INCLUDE_DIRS})
link_directories (${PNG_LIBRARY_DIRS})
link_libraries (${PNG_LIBRARIES})

set(SOURCE_FILES main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/pngpp)



add_executable(pixl_mosaic ${SOURCE_FILES})
target_compile_options(pixl_mosaic PRIVATE "-O3")
link_libraries(pixl_mosaic)
