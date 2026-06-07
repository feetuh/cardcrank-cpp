# Building with CMake

## Dependencies

This project uses [vcpkg](https://vcpkg.io/) for dependency management. For a list of dependencies, please refer to [vcpkg.json](vcpkg.json).

### Installing vcpkg

If you don't have vcpkg installed, follow these steps:

```sh
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
```

### Installing Dependencies

```sh
vcpkg install
```

## Build

This project uses vcpkg for dependencies, so you need to provide the vcpkg toolchain file when configuring CMake.

Here are the steps for building in release mode with a single-configuration
generator, like the Unix Makefiles one:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Here are the steps for building in release mode with a multi-configuration
generator, like the Visual Studio ones:

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

**Note:** Replace `$VCPKG_ROOT` with the actual path to your vcpkg installation, or use the integrated path (typically `~/.vcpkg/scripts/buildsystems/vcpkg.cmake`).

### Building with MSVC

Note that MSVC by default is not standards compliant and you need to pass some
flags to make it behave properly. See the `flags-msvc` preset in the
[CMakePresets.json](CMakePresets.json) file for the flags and with what
variable to provide them to CMake during configuration.

### Building on Apple Silicon

CMake supports building on Apple Silicon properly since 3.20.1. Make sure you
have the [latest version][1] installed.

## Testing

To build and run the tests, enable testing during configuration:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
ctest --test-dir build
```

Or run the test executable directly:

```sh
./build/test/cardcrank-cpp_test
```

### Test Dependencies

Testing requires GoogleTest, which is automatically installed via vcpkg when you run `vcpkg install`.

## Install

This project doesn't require any special command-line flags to install to keep
things simple. As a prerequisite, the project has to be built with the above
commands already.

The below commands require at least CMake 3.15 to run, because that is the
version in which [Install a Project][2] was added.

Here is the command for installing the release mode artifacts with a
single-configuration generator, like the Unix Makefiles one:

```sh
cmake --install build
```

Here is the command for installing the release mode artifacts with a
multi-configuration generator, like the Visual Studio ones:

```sh
cmake --install build --config Release
```

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/cmake/help/latest/manual/cmake.1.html#install-a-project
