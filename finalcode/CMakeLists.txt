cmake_minimum_required(VERSION 3.5)
project(finalcode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp ComputeDs.h ds2H.h generateBitplanes.h getParameters.h imageFunctions.h imwarp.h
        lucaskanade.h matrixFunctions.h padImages.h readImage.h readVideo.h RunLucasKanade.h Test.h warpCoordinates.h)
add_executable(finalcode ${SOURCE_FILES})
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(finalcode ${OpenCV_LIBS})