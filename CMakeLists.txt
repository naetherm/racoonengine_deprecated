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

##################################################
## CMake
##################################################
cmake_minimum_required(VERSION 3.20.0)

##################################################
## Global options
##################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#set(CMAKE_UNITY_BUILD TRUE)

##################################################
## Project
##################################################
project(RacoonEngine)

##################################################
## Includes
##################################################
include(cmake/PAL.cmake)
include(cmake/Utils.cmake)
include(cmake/Platform.cmake)
include(cmake/Externals.cmake)
include(cmake/Modules/GetGitRevisionDescription.cmake)

##################################################
## Prerequisites
##################################################

set(PL_BIN_DIR      "${CMAKE_INSTALL_PREFIX}/bin/${PL_TARGET_ARCHBITSIZE}")
set(PL_LIB_DIR      "${CMAKE_INSTALL_PREFIX}/lib/${PL_TARGET_ARCHBITSIZE}")
set(PL_INCLUDE_DIR  "${CMAKE_INSTALL_PREFIX}/include")
set(PL_DOC_DIR      "${CMAKE_INSTALL_PREFIX}/docs")
set(PL_ROOT_DIR     "${CMAKE_CURRENT_SOURCE_DIR}")

find_library(STD_CPP_FS stdc++fs)
#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
#find_package(LibClang)
find_package(LLVM REQUIRED CONFIG)
include(cmake/Config.cmake)
include(cmake/Build.cmake)
include(cmake/Project.cmake)
include(cmake/Target.cmake)


pl_split()
pl_message("RacoonEngine Build")
pl_newline()
pl_message("Version:\t${PL_PROJECT_NAME} ${PL_PROJECT_VERSION_MAJOR}.${PL_PROJECT_VERSION_MINOR}.${PL_PROJECT_VERSION_PATCH}-${PL_PROJECT_VERSION_SUFFIX} (${PL_PROJECT_VERSION_RELEASE})")
pl_message("Platform:\t${PL_SUPPORTED_PLATFORMS} ${PL_TARGET_ARCHBITSIZE}")
pl_message("Compiler:\t${CMAKE_CXX_COMPILER_ID}")
pl_newline()
pl_print_configuration()
pl_newline()
pl_split()

##################################################
## Target
##################################################
add_subdirectory(Engine)
add_subdirectory(Tutorial)
#add_subdirectory(Documentation)
