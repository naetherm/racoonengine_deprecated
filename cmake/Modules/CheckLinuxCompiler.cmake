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

##################################################
## Includes
##################################################
include(CheckCXXSourceCompiles)


##################################################
## Macro: Check if GCC visibility flag is available
##################################################
macro(pl_check_cxx_visibility_flag var)
  set(SAFE_CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS}")
  set(CMAKE_REQUIRED_DEFINITIONS "-fvisibility=hidden -fvisibility-inlines-hidden")
  set(CODE "__attribute__ ((visibility(\"default\"))) void testfunc(){} int main() {return 0;}")
  check_cxx_source_compiles("${CODE}" ${var})
  set(CMAKE_REQUIRED_DEFINITIONS "${SAFE_CMAKE_REQUIRED_DEFINITIONS}")
endmacro(pl_check_cxx_visibility_flag)


##################################################
## Macro: Check if GCC C++0x flag is available
##################################################
macro(pl_check_cxx_cpp0x_flag var)
  message(STATUS "check compiler has C++20 flag")
  set(SAFE_CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS}")
  set(CMAKE_REQUIRED_DEFINITIONS "-std=c++20")
  set(CODE "void testfunc(void *pointer){} int main() {testfunc(nullptr);return 0;}")
  check_cxx_source_compiles("${CODE}" ${var})
  set(CMAKE_REQUIRED_DEFINITIONS "${SAFE_CMAKE_REQUIRED_DEFINITIONS}")
endmacro(pl_check_cxx_cpp0x_flag)


##################################################
## Check compiler compatibilities
##################################################

# Check compiler visibility and C++0x flag
if (NOT NO_VISIBILITY_CHECK)
  # Check compiler visibility flag
  pl_check_cxx_visibility_flag(PL_COMPILER_VISIBILITY_FLAG)
  if (PL_COMPILER_VISIBILITY_FLAG)
    #ADD_DEFINITIONS(-fvisibility=hidden -fvisibility-inlines-hidden -DHAVE_VISIBILITY_ATTR)
    message(STATUS "compiler has visibility flag")
  endif ()
endif ()

# Check compiler C++0x flag
pl_check_cxx_cpp0x_flag(PL_COMPILER_CPP0X_FLAG)
if (PL_COMPILER_CPP0X_FLAG)
  ADD_DEFINITIONS(-std=c++20)
  message(STATUS "compiler has C++20 flag")
endif ()
