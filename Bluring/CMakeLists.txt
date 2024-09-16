cmake_minimum_required(VERSION 3.5.0)
project(Bluring VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Add the executable that will be built from the source files
add_executable(Bluring main.cpp)

# Set the C++ standard to C++17
set_property(TARGET Bluring PROPERTY CXX_STANDARD 17)
