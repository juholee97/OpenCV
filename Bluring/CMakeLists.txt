cmake_minimum_required(VERSION 3.5.0)

set(M_CURRENT_PROJECT BluringApplication) 
set(M_TARGET_NAME Application) 
set(PATH_OPEN_CV_DIR ${PATH_OPEN_CV}/opencv/build)

project(${M_CURRENT_PROJECT} LANGUAGES C CXX)

# Find OpenCV package
# opencv version : 4.10.0-windows
find_package(OpenCV REQUIRED)

add_definitions(-DMY_CONFIG_DIR=\"${CMAKE_SOURCE_DIR}/config.json\")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/App
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CApplicationManager
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageConfig
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageObject
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CLogger
	${CMAKE_CURRENT_SOURCE_DIR}/src/libs
	${CMAKE_CURRENT_SOURCE_DIR}/src/ext_libs
	${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
	${OpenCV_INCLUDE_DIRS}
	${PATH_OPEN_CV_DIR}/include
)

set(BUILD_SHARED_LIBS ON)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/libs/BlurOpenCV" "${CMAKE_BINARY_DIR}/src/libs/BlurOpenCV") 
add_subdirectory("${CMAKE_SOURCE_DIR}/src/libs/BlurCustom" "${CMAKE_BINARY_DIR}/src/libs/BlurCustom") 

file(GLOB_RECURSE  PROJECT_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/main.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CApplicationManager/CApplicationManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CApplicationManager/CApplicationManager.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageObject/CImageObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageObject/CImageObject.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageConfig/CImageConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CImageConfig/CImageConfig.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/App/CLogger/CLogger.h

	${CMAKE_CURRENT_SOURCE_DIR}/src/ext_lib/json.hpp
)

add_executable(${M_TARGET_NAME} ${PROJECT_SOURCE_FILES}
)

target_link_libraries(${M_TARGET_NAME} ${OpenCV_LIBS})

set_property(TARGET ${M_TARGET_NAME} PROPERTY CXX_STANDARD 17 )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${PROJECT_SOURCE_FILES})