
find_package( OpenCV REQUIRED )

add_library(ShapeLib STATIC Shape.hpp Shape.cpp)
target_link_libraries(ShapeLib PUBLIC ${OpenCV_LIBS})