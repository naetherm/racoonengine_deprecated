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

set(RE_BIN_DIR      "${CMAKE_INSTALL_PREFIX}/bin/${RE_TARGET_ARCHBITSIZE}")
set(RE_LIB_DIR      "${CMAKE_INSTALL_PREFIX}/lib/${RE_TARGET_ARCHBITSIZE}")
set(RE_INCLUDE_DIR  "${CMAKE_INSTALL_PREFIX}/include")
set(RE_DOC_DIR      "${CMAKE_INSTALL_PREFIX}/docs")
set(RE_ROOT_DIR     "${CMAKE_CURRENT_SOURCE_DIR}")

find_library(STD_CPP_FS stdc++fs)
include(cmake/Config.cmake)
include(cmake/Build.cmake)
include(cmake/Project.cmake)
include(cmake/Target.cmake)
find_package(DBus REQUIRED)


re_split()
re_message("RacoonEngine Build")
re_newline()
re_message("Version:\t${RE_PROJECT_NAME} ${RE_PROJECT_VERSION_MAJOR}.${RE_PROJECT_VERSION_MINOR}.${RE_PROJECT_VERSION_PATCH}-${RE_PROJECT_VERSION_SUFFIX} (${RE_PROJECT_VERSION_RELEASE})")
re_message("Platform:\t${RE_SUPPORTED_PLATFORMS} ${RE_TARGET_ARCHBITSIZE}")
re_message("Compiler:\t${CMAKE_CXX_COMPILER_ID}")
re_newline()
re_print_configuration()
re_newline()
re_split()

##################################################
## Target
##################################################
add_subdirectory(Engine)
add_subdirectory(Tutorial)
#add_subdirectory(Documentation)
