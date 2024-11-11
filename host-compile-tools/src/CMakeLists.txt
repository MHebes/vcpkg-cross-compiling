# this project is always built using ExternalProject_Add using the host triplet.

cmake_minimum_required(VERSION 3.26)

project(host-compile-tools VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(
	NOMINMAX
	WIN32_LEAN_AND_MEAN
	UNICODE _UNICODE
	SDL_FUNCTION_POINTER_IS_VOID_POINTER
)

add_executable(PackAsGltf "PackAsGltf.cpp")

find_package(assimp REQUIRED)
target_link_libraries(PackAsGltf PRIVATE assimp::assimp)

# install -config.cmake files and exes

install(
	TARGETS
		PackAsGltf
	EXPORT
		${PROJECT_NAME}-targets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)

configure_package_config_file("${PROJECT_NAME}-config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
	INSTALL_DESTINATION "share/${PROJECT_NAME}"
	PATH_VARS CMAKE_INSTALL_PREFIX
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION share/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}-targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION share/${PROJECT_NAME}
)