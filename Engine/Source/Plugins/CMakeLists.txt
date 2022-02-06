#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2019 - 2022 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////

################################################################################
# Current package
################################################################################
pl_add_subdirectory(REFrontendQt)

pl_add_subdirectory(RERHINull)
pl_add_subdirectory(RERHIOpenGL)
#pl_add_subdirectory(RERHIOpenGLES3)
#pl_add_subdirectory(RERHID3D9)
#pl_add_subdirectory(RERHID3D10)
#pl_add_subdirectory(RERHID3D11)
#pl_add_subdirectory(RERHID3D12)
pl_add_subdirectory(RERHIVulkan)

pl_add_subdirectory(REScriptBindings)
pl_add_subdirectory(REScriptLua)
