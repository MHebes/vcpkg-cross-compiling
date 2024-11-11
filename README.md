# Problem

I want to compile for a target architecture, but have some build-time utilities to run on the host architecture.

For a dummy but concrete example, I have a "game", which I want to compile for both windows (`cl`) and wasm (`emcc`).

As part of the build, I want to transform a source-dir`cube.stl` into a build-dir `cube.glb`. This is done with a ~20 lines of code utility with [Assimp](https://github.com/assimp/assimp).

# Existing solutions

* Create a build directory for your host tools first, run `cmake --build` in there, hardcode the relative paths to the tools: https://stackoverflow.com/a/36084786/3554391
* Whatever magic VTK does that I couldn't understand: https://discourse.cmake.org/t/building-compile-time-tools-when-cross-compiling/601/5
* Just run CMake twice, put it in a bash script, point one to the other: https://stackoverflow.com/a/36294332/3554391

# My solution

This is most useful if you're already using a package manager. I use vcpkg, but could probably be adapted to Conan.

The gist is:
1. Split the build utilities into their own CMake project.
2. Make the build utilities install into a library, including a `build-utilities-config.cmake` file.
3. Write a vcpkg port for your build utilities. Use `vcpkg_copy_tools` in the `portfile.cmake` so they go in the `vcpkg_installed/{triple}/tools` subdirectory when used.
4. Give your target CMake tree a vcpkg dependency on the build utilities, and add a `"overlay-port"` pointing to (3).
5. In the target CMake, set `VCPKG_HOST_TRIPLET` to e.g. `x64-windows`, and set `VCPKG_USE_HOST_TOOLS` to `ON`. This makes that `tools/` directory searchable by `find_program`.
6. Use `find_program` to get the exe path to your build utilties.

# Building

Toolchains are loaded via VCPKG_CHAINLOAD_TOOLCHAIN_FILE, for example via the `CMakePresets.json`.

## Building without a special toolchain

```
cd vcpkg-cross-compiling/target
mkdir -p build/host
cmake -S . -B build/host --toolchain "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
cmake --build ./build/host -j
```

## Building with a special toolchain.

Use `CMakePresets.json` like `cmake --preset emscripten`, or do it manually:

```
cd vcpkg-cross-compiling/target
mkdir -p build/emscripten
cmake -S . \
    -B build/emscripten/ \
    -G Ninja \
    --toolchain "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake"
cmake --build ./build/emscripten -j
```