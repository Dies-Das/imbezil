
cmake_policy(SET CMP0069 NEW)

find_package( OpenCV REQUIRED )

add_library(ShapeLib STATIC Shape.hpp Shape.cpp Ellipse.hpp Ellipse.cpp Triangle.hpp Triangle.cpp Line.hpp Line.cpp Quadratic.cpp Quadratic.hpp)
target_link_libraries(ShapeLib PUBLIC ${OpenCV_LIBS})