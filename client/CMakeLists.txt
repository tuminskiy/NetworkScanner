cmake_minimum_required(VERSION 3.15)

set(EXEC_NAME scanwatcher)

add_executable(${EXEC_NAME})

set_target_properties(${EXEC_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug/${EXEC_NAME}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release/${EXEC_NAME}
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui

  AUTOUIC ON
  AUTOMOC ON
)

set(GUI_SRC
  include/gui/window/connectdialog.hpp src/gui/window/connectdialog.cpp
  include/gui/window/mainwindow.hpp src/gui/window/mainwindow.cpp
  include/gui/window/hostwidget.hpp src/gui/window/hostwidget.cpp
)

set(NSCANCLIENT_SRC include/nscanclient/nscanclient.hpp src/nscanclient/nscanclient.cpp)

set(STORAGE_SRC
  include/storage/database.hpp src/storage/database.cpp
  include/storage/detail/query.hpp src/storage/detail/query.cpp
)

set(SRC
  src/main.cpp
  ${GUI_SRC}
  ${NSCANCLIENT_SRC}
  ${STORAGE_SRC}
)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

target_include_directories(${EXEC_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${EXEC_NAME} PRIVATE ${SRC})

target_link_libraries(${EXEC_NAME}
  PRIVATE
    Qt5::Widgets Qt5::Sql
    nmapdef dbdef nscanservice
)
