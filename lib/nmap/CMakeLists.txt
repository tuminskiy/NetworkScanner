cmake_minimum_required(VERSION 3.15)

set(LIB_NAME nmapdef)
add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
