cmake_minimum_required(VERSION 3.15)
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RUST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rust_lib)
set(RUST_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/rust_target)
set(CXXBRIDGE_DIR ${RUST_TARGET_DIR}/cxxbridge)

find_program(CXXBRIDGE cxxbridge REQUIRED)

add_custom_command(
    OUTPUT
        ${CXXBRIDGE_DIR}/rust_lib/src/lib.rs.h
        ${CXXBRIDGE_DIR}/rust_lib/src/lib.rs.cc
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CXXBRIDGE_DIR}
    COMMAND ${CXXBRIDGE} ${RUST_LIB_DIR}/src/lib.rs --header > ${CXXBRIDGE_DIR}/rust_lib/src/lib.rs.h
    COMMAND ${CXXBRIDGE} ${RUST_LIB_DIR}/src/lib.rs > ${CXXBRIDGE_DIR}/rust_lib/src/lib.rs.cc
    DEPENDS ${RUST_LIB_DIR}/src/lib.rs
    COMMENT "Generating cxx bridge"
)

add_custom_command(
    OUTPUT ${RUST_TARGET_DIR}/release/rust_lib.lib
    COMMAND cargo build --release --target-dir=${RUST_TARGET_DIR}
    WORKING_DIRECTORY ${RUST_LIB_DIR}
    DEPENDS ${RUST_LIB_DIR}/src/lib.rs ${RUST_LIB_DIR}/Cargo.toml
    COMMENT "Building Rust static library"
)

add_custom_target(rust_build
    DEPENDS ${RUST_TARGET_DIR}/release/rust_lib.lib
)

add_library(rust_lib STATIC IMPORTED)
set_target_properties(rust_lib PROPERTIES
    IMPORTED_LOCATION ${RUST_TARGET_DIR}/release/rust_lib.lib
)
add_dependencies(rust_lib rust_build)

add_library(rust_bridge STATIC
    ${CXXBRIDGE_DIR}/rust_lib/src/lib.rs.cc
)

target_include_directories(rust_bridge PUBLIC
    ${CXXBRIDGE_DIR}
)

target_link_libraries(rust_bridge
    rust_lib
    ws2_32
    userenv
    ntdll
)

add_dependencies(rust_bridge rust_build)

add_executable(engine src/main.cpp)

target_include_directories(engine PRIVATE
    ${CXXBRIDGE_DIR}  
)

target_link_libraries(engine rust_bridge)