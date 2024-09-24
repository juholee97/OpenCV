cmake_minimum_required(VERSION 3.10)

project(BlurCustom)

add_library(BlurCustom SHARED BlurCustom.cpp)

set_target_properties(BlurCustom PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME "BlurCustom"
    PREFIX ""
)
