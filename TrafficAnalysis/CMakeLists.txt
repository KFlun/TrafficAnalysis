cmake_minimum_required(VERSION 3.15)
project(TrafficAnalysis)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "F:\\Opencv-3.2.0\\opencv\\Mingw-build\\install")#改为mingw-bulid的位置
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(TrafficAnalysis main.cpp)
#add_executable(TrafficAnalysis test.cpp)

# add libs you need
#set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
link_libraries("opencv_core")
link_libraries("opencv_imgproc")
link_libraries("opencv_highgui")
link_libraries("opencv_imgcodecs")

target_link_libraries(TrafficAnalysis ${OpenCV_LIBS})

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")






