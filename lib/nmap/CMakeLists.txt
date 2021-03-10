cmake_minimum_required(VERSION 3.15)

set(NMAP_LIB nmap_lib)
add_library(${NMAP_LIB} STATIC)

set_target_properties(${NMAP_LIB} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug/${NMAP_LIB}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release/${NMAP_LIB}
)

set(SRC
  include/definitions.hpp
  
  include/convert.hpp src/convert.cpp
)

find_package(Boost REQUIRED)

target_include_directories(${NMAP_LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${Boost_INCLUDE_DIR}
)

target_sources(${NMAP_LIB} PRIVATE ${SRC})

target_link_libraries(${NMAP_LIB} PRIVATE ${Boost_LIBRARIES})
