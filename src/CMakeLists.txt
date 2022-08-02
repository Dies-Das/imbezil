cmake_minimum_required(VERSION 3.15)
project( imbezil )
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_subdirectory(Shapes)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0081 NEW)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0069 NEW)

find_package(OpenMP REQUIRED)
find_package( OpenCV REQUIRED )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++17 -O3 -Wextra -fno-strict-aliasing -flto=auto"  )
set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -O3" )
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
include_directories( ${OpenCV_INCLUDE_DIRS}  Shapes)
link_directories(Shapes)

add_executable( imbezil main.cpp )
target_link_libraries( imbezil PUBLIC  ShapeLib ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
target_include_directories(imbezil PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/Shapes"
                          )