#!/bin/bash

ROOT_PATH=$(pwd)
BUILD_PATH=${ROOT_PATH}/build-cmake-release

if [ ! -d "build-cmake-release" ]
then
  mkdir build-cmake-release
fi
cd build-cmake-release

echo "Generating project files ..."
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DCMAKE_BUILD_TYPE=Release
echo "Done."

echo "Building all projects ..."
make -j $(nproc)
ln -s ${BUILD_PATH}/Engine/Source/Plugins/RERHIOpenGL/libRERHIOpenGL.so ${BUILD_PATH}/Tutorial/01.Frontends/01.02.Gui/libRERHIOpenGL.so
ln -s ${BUILD_PATH}/Engine/Source/Plugins/RERHIOpenGL/libRERHIOpenGL.so ${BUILD_PATH}/Tutorial/02.Rendering/02.01.LowLevel/libRERHIOpenGL.so
ln -s ${BUILD_PATH}/Engine/Source/Plugins/RERHIOpenGL/libRERHIOpenGL.so ${BUILD_PATH}/Tutorial/02.Rendering/02.03.HighLevel/libRERHIOpenGL.so
echo "Done."

echo "Auto-Generate runtime-compiled resources for OpenGL_440"
cd Tutorial/02.Rendering/02.02.ProjectCompiler
./02.02.ProjectCompiler --target OpenGL_440
echo "Done."

cd ${ROOT_PATH}
