cmake_minimum_required(VERSION 3.15)

set(EXEC_NAME nscan)

add_executable(${EXEC_NAME})

set_target_properties(${EXEC_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/${EXEC_NAME}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/${EXEC_NAME}

  AUTOMOC ON
)

set(NMAP_SRC
  include/nmap/scanner.hpp src/nmap/scanner.cpp
  include/nmap/nmapresult.hpp src/nmap/nmapresult.cpp
)

set(STORAGE_SRC
  include/storage/database.hpp src/storage/database.cpp
  include/storage/detail/query.hpp src/storage/detail/query.cpp
)

set(NSCANSERVICE_SRC include/nscanservice/nscanservice.hpp src/nscanservice/nscanservice.cpp)

set(UTIL_SRC include/util/functions.hpp src/util/functions.cpp)

set(SRC
  src/main.cpp
  ${NMAP_SRC}
  ${STORAGE_SRC}
  ${NSCANSERVICE_SRC}
  ${UTIL_SRC}
)

find_package(Qt5 COMPONENTS Core Sql REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

target_include_directories(${EXEC_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)

target_sources(${EXEC_NAME} PRIVATE ${SRC})

target_link_libraries(${EXEC_NAME}
  PRIVATE
    Qt5::Core Qt5::Sql
    ${Boost_LIBRARIES}
    nmapdef dbdef nscanservice
)