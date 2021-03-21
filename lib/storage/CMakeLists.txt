cmake_minimum_required(VERSION 3.15)

set(LIB_NAME dbdef)
add_library(${LIB_NAME} STATIC)

set_target_properties(${LIB_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug/${LIB_NAME}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release/${LIB_NAME}
)

set(SRC include/basedb.hpp src/basedb.cpp)

find_package(Qt5 COMPONENTS Sql REQUIRED)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(${LIB_NAME} PRIVATE ${SRC})

target_link_libraries(${LIB_NAME} PRIVATE Qt5::Sql)
