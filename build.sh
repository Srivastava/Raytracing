DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BUILD_DIR="$DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# cmake -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe ..
cmake -G "Visual Studio 16 2019" ..
make --jobs=4

