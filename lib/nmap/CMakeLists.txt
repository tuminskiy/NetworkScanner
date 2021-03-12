cmake_minimum_required(VERSION 3.15)

set(LIB_NAME nmapdef)
add_library(${LIB_NAME} STATIC)

set_target_properties(${LIB_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug/${LIB_NAME}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release/${LIB_NAME}
)

set(SRC
  include/definitions.hpp
  include/convert.hpp src/convert.cpp
)

find_package(Boost REQUIRED)

target_include_directories(${LIB_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)

target_sources(${LIB_NAME} PRIVATE ${SRC})

target_link_libraries(${LIB_NAME} PRIVATE ${Boost_LIBRARIES})
