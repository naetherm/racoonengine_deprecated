# Notes

The first example, 02.01.LowLevel can be called the following way.
Before that make sure you copied the RERHIOpenGL.so file into the build directory of 02.01.LowLevel! 

./02.01.LowLevel --rhi OpenGL --example Triangle

The examples 02.02.ProjectCompiler and 02.03.HighLevel have to be executed in this order.

First, call 02.02.ProjectCompiler with the target configuration you want to generate the runtime information for, e.g.:

./02.02.ProjectCompiler --target OpenGL_440

Second, and **after** you copied the RERHIOpenGL.so file into the build directory of 02.03.HighLevel, you can call:

./02.03.HighLevel --rhi OpenGL --example Scene

(Sorry for the complexity with copying files here and there, this will be a topic for one of the upcoming releases)