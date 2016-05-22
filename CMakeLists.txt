cmake_minimum_required(VERSION 3.0)

project( OpenCvProject )

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable( OpenCvProject
"${PROJECT_SOURCE_DIR}/src/Source.cpp" 
)

target_link_libraries( OpenCvProject ${OpenCV_LIBS} )