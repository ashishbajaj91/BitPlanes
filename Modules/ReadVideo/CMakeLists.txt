cmake_minimum_required(VERSION 3.5)
project(ReadVideo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(POSSIBLE_OPENCV_LOCATION 
	C:/OpenCV_3.2.0_new/build)


find_package(OpenCV PATHS ${POSSIBLE_OPENCV_LOCATION})



include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(ReadVideo ${SOURCE_FILES})

target_link_libraries(ReadVideo ${OpenCV_LIBS})