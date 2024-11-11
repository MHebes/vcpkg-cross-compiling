cmake_minimum_required(VERSION 3.26)

# set VCPKG_HOST_TRIPLET to the current host if not overriden by user
if(NOT VCPKG_HOST_TRIPLET)
    # @todo determine this programmatically?
    set(VCPKG_HOST_TRIPLET x64-windows)
endif()

# allows find_program to work with vcpkg tools (like those in host-compile-tools)
set(VCPKG_USE_HOST_TOOLS ON)

project(test)

set(CMAKE_CXX_STANDARD 23)

# because of VCPKG_HOST_TRIPLET and VCPKG_USE_HOST_TOOLS are both set above,
# we can use find_program to find our host exe
find_program(PackAsGltf PackAsGltf)

message(STATUS "PackAsGltf = ${PackAsGltf}")

find_package(asio CONFIG REQUIRED)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE asio::asio)

add_custom_command(TARGET test
    POST_BUILD
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/cube.glb"
    COMMAND "${PackAsGltf}" "${CMAKE_CURRENT_SOURCE_DIR}/cube.stl" "${CMAKE_CURRENT_BINARY_DIR}/cube.glb"
    COMMENT "Generating cube.glb"
)

install(TARGETS test)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cube.glb" TYPE BIN)