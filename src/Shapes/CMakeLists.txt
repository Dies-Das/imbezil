
cmake_policy(SET CMP0069 NEW)

find_package( OpenCV REQUIRED )

add_library(ShapeLib STATIC Shape.hpp Shape.cpp Ellipse.hpp Ellipse.cpp)
target_link_libraries(ShapeLib PUBLIC ${OpenCV_LIBS})