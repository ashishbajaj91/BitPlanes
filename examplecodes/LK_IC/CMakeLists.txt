cmake_minimum_required(VERSION 3.5)
project(LK_IC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})
add_definitions(${OPENCV_DEFINITIONS})
add_executable(LK_IC ${SOURCE_FILES})
target_link_libraries(LK_IC ${OpenCV_LIBS})