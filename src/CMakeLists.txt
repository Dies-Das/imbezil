
project( DisplayImage )
add_subdirectory(Shapes)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0081 NEW)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0057 NEW)
find_package(OpenMP REQUIRED)
find_package( OpenCV REQUIRED )
find_package(Pagmo REQUIRED)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++17 -O3" )
set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -O3" )
include_directories( ${OpenCV_INCLUDE_DIRS}  Shapes)
link_directories(Shapes)

add_executable( DrawImage main.cpp )
target_link_libraries( DrawImage PUBLIC  ShapeLib ${OpenCV_LIBS} Pagmo::pagmo OpenMP::OpenMP_CXX)
target_include_directories(DrawImage PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/Shapes"
                          )