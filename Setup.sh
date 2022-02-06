#!/bin/bash

ROOT_PATH=$(pwd)

mkdir build-cmake-release
cd build-cmake-release

echo "Generating project files ..."
cmake .. -DCMAKE_BUILD_TYPE=Release
echo "Done."

echo "Building all projects ..."
make -j $(nproc)
echo "Done."

echo "Auto-Generate runtime-compiled resources for OpenGL_440"
cd Tutorial/02.Rendering/02.02.ProjectCompiler
./02.02.ProjectCompiler --target OpenGL_440
echo "Done."

cd ${ROOT_PATH}