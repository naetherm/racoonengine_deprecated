# RacoonEngine

RacoonEngine is a cross-platform 3D engine and application framework for any kind of 3D applications like games, interactive simulations or visualizations. It's based on a highly flexible scene graph system that allows you to compose and visualize any type of 3D scene for your application.

RacoonEngine is written in C++ and has been designed with flexibility and extensibility as one of it's main goals. Therefore, it's not only a 3D engine, but a consistent framework that allows you to combine all the components that you need for your application without having to care about the differences of the actual libraries, APIs or operating systems that you are using. The underlying systems and libraries are abstracted by a powerful reflection and component system, which makes it easier and more effective to create applications for different platforms and target components such as rendering, sound, physics, network, scripting and so on.

## Status

[![Linux](https://github.com/naetherm/racoonengine/actions/workflows/linux_builds.yml/badge.svg?branch=develop)](https://github.com/naetherm/racoonengine/actions/workflows/linux_builds.yml)

## Notes

Please keep in mind that this is a highly prototypical piece of work, structures, classes, file formats can and will change without prior warning.
The screenshot taken for the editor view is also considered highly prototypically as it was just a proof of concept implementation and will change as the next target for this project is to add the editing tools.

## Screenshots

"Game"-Mode view:
![Screen](Documentation/screen00.png "Screen00")

"Editor"-Mode view:

![Screen](Documentation/screen01.png "Screen01")

## Features

Some features available within this project are:

- Low-level rendering (RHI) implementations, designed with AZDO ("Almost Zero Driver Overhead") in mind
- RHIs: OpenGL, Vulkan (currently worked on), OpenGLES3, Direct3D9, Direct3D10, Direct3D11, Direct3D12 (all D3D* interfaces currently worked on)
- Designed to be cross-platform (currently only Linux with some basic structures for Windows systems)
- All shader types are supported, including mesh shaders (not tested as I use AMD)
- Asynchronous resource streaming
- Resource hot reloading support
- Resource compilation to target platform
- Abstract shader language: Write once, use across all RHIs
- Forward and Deferred Rendering (through data-driven compositor system)
- Data-driven material system, designed from the ground up

## Getting Started

Follow the instructions below to compile and run the engine (or to be more precise the examples) from source.

### Linux

This will compile all examples and libraries first and then compile the resources required for the `02.03.HighLevel` example in shipping quality (this can take a while).

```
./Setup.sh
```