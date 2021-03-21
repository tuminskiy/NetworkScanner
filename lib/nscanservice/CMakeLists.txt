cmake_minimum_required(VERSION 3.15)

set(LIB_NAME nscanservice)
add_library(${LIB_NAME} STATIC)

set_target_properties(${LIB_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug/${LIB_NAME}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release/${LIB_NAME}
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)

set(PROTO_FILES nscanservice.proto)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_sources(${LIB_NAME} PRIVATE ${PROTO_FILES})

target_link_libraries(${LIB_NAME} PUBLIC protobuf::libprotobuf gRPC::grpc gRPC::grpc++)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ${LIB_NAME} LANGUAGE cpp)
protobuf_generate(TARGET ${LIB_NAME} LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")