set(VCPKG_POLICY_EMPTY_INCLUDE_FOLDER enabled)
set(VCPKG_POLICY_SKIP_COPYRIGHT_CHECK enabled)

vcpkg_cmake_configure(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../src")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME host-compile-tools)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

# copies these to the vcpkg_installed/{host-triplet}/tools directory,
# allowing users to consume with VCPKG_USE_HOST_TOOLS
vcpkg_copy_tools(
    TOOL_NAMES
        PackAsGltf
    AUTO_CLEAN
)