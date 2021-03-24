cmake_minimum_required(VERSION 3.15)

set(EXEC_NAME scanwatcher)

add_executable(${EXEC_NAME})

set_target_properties(${EXEC_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/${EXEC_NAME}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/${EXEC_NAME}

  AUTOMOC ON
  AUTORCC ON
)

set(SRC
  src/main.cpp
  resource/qml.qrc
)

find_package(Qt5 COMPONENTS Qml Quick Sql REQUIRED)

target_include_directories(${EXEC_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${EXEC_NAME} PRIVATE ${SRC})

target_link_libraries(${EXEC_NAME}
  PRIVATE
    Qt5::Qml Qt5::Quick Qt5::Sql
    nmapdef dbdef nscanservice
)
