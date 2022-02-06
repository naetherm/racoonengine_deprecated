#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2021 RacoonStudios
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
# Project Configuration
################################################################################
set(PL_PROJECT_NAME "RacoonEngine")
set(PL_PROJECT_VERSION_MAJOR 0)
set(PL_PROJECT_VERSION_MINOR 1)
set(PL_PROJECT_VERSION_PATCH 0)
set(PL_PROJECT_VERSION_RELEASE "Dione")
set(PL_PROJECT_VERSION_SUFFIX "Alpha")

# New stuff, document me!
get_git_head_revision(PL_PROJECT_GIT_BRANCH PL_PROJECT_GIT_SHA)
string(SUBSTRING "${PL_PROJECT_GIT_SHA}" 0 7 PL_PROJECT_VERSION_REV) # 7 characters from the SHA is git's default abbreviated commit desc
string(TIMESTAMP PL_BUILD_TIME "%Y-%m-%d %H:%M UTC" UTC)
configure_file("cmake/Config/BuildInfo.json.in" "${CMAKE_BINARY_DIR}/BuildInfo.json")

configure_file("cmake/Config/RacoonEngine.h.in" "${CMAKE_BINARY_DIR}/Engine/RacoonEngine.h")
set(PL_CONFIG_FILE_LOCATION "${CMAKE_BINARY_DIR}/Engine" CACHE INTERNAL "Location of RacoonEngine.h file")

set(PL_USE_SYSTEM_LIBS OFF CACHE BOOL "Should we use system libraries?")

################################################################################
# CMake Configuration
################################################################################
set(CMAKE_CXX_STANDARD 17)

set(PL_EXTERNAL_LIBRARIES "$ENV{HOME}/.racoonengine/Externals" CACHE INTERNAL "Location of external resources")

################################################################################
# Macros
################################################################################
macro (pl_print_configuration)
  pl_message("** CONFIGURATION **")
  pl_message("** General settings **")
endmacro()

################################################################################
# Build Configuration
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



# Add our own modules to the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
