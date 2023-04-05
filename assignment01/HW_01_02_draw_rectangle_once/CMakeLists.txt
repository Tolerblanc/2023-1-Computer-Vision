cmake_minimum_required(VERSION 3.0.0)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set (CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_STATIC_LIB})

configure_file(${OpenCV_DYNAMIC_LIB} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)