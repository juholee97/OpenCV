cmake_minimum_required(VERSION 3.10)

project(BlurOpenCV)

add_library(BlurOpenCV SHARED BlurOpenCV.cpp)

find_package(OpenCV REQUIRED)

target_include_directories(BlurOpenCV PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(BlurOpenCV PUBLIC ${OpenCV_LIBS})

set_target_properties(BlurOpenCV PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME "BlurOpenCV"
    PREFIX ""
)
