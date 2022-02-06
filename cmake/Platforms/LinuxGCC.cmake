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

list(REMOVE_ITEM PL_UNSUPPORTED_PLATFORMS Linux Unix)
set(PL_SUPPORTED_PLATFORMS Linux Unix)

if (CMAKE_GENERATOR MATCHES "Makefiles")
  message(STATUS "Check for GCC compiler version")
  set(CMAKE_TEST_COMPILER ${CMAKE_C_COMPILER})
  if (NOT CMAKE_C_COMPILER)
    set(CMAKE_TEST_COMPILER ${CMAKE_CXX_COMPILER})
  endif ()

  exec_program(${CMAKE_TEST_COMPILER}
    ARGS -dumpversion
    OUTPUT_VARIABLE CMAKE_COMPILER_OUTPUT
    RETURN_VALUE CMAKE_COMPILER_RETURN
    )
  if (NOT CMAKE_COMPILER_RETURN)
    file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Determining the version of compiler passed with the following output:\n"
      "${CMAKE_COMPILER_OUTPUT}\n\n")
    string(REGEX REPLACE "\n" " " compilerVersion "${CMAKE_COMPILER_OUTPUT}")
    message(STATUS "Check for GCC compiler version - ${compilerVersion}")
    set(GCC44)
    set(GCC45)
    set(GCC46)
    string(REGEX REPLACE "^([0-9]+).([0-9]+).([0-9]+)\$" "\\1"
      compilerVersionMajor "${compilerVersion}")
    string(REGEX REPLACE "^([0-9]+)[.]+([0-9]+)[.]+([0-9]+)\$" "\\2"
      compilerVersionMinor "${compilerVersion}")
    string(REGEX REPLACE "^([0-9]+)[.]+([0-9]+)[.]+([0-9]+)\$" "\\3"
      compilerVersionBuild "${compilerVersion}")
    set(GCC${compilerVersionMajor}${compilerVersionMinor} 1)
    set(GCC${compilerVersionMajor}${compilerVersionMinor}${compilerVersionBuild} 1)
    set(GCC_VERSION "${compilerVersion}")
  else ()
    message(STATUS "Check for GCC compiler version - failed")
    file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
      "Determining the version of compiler failed with the following output:\n"
      "${CMAKE_COMPILER_OUTPUT}\n\n")
  endif ()
endif ()

##################################################
## Libraries
##################################################

# X11 libraries
set(LINUX_X11_LIBS
  X11         # X-Lib
  Xext        # X extentions library
  Xcursor        # X cursor library
  Xrandr
  Xxf86vm
  )
set(LINUX_X11_LIB X11)   # X-Lib
set(LINUX_XEXT_LIB Xext)   # X extentions library
set(LINUX_XCURSOR_LIB Xcursor)  # X cursor library

set(LINUX_UUID_LIB uuid)


##################################################
## Preprocessor definitions
##################################################

# Preprocessor definitions
set(LINUX_COMPILE_DEFS
  ${LINUX_COMPILE_DEFS}
  GCC         # We are using the GCC/g++ compiler
  )
if (APPLE)
  # Add a handy APPLE definition (just like WIN32, LINUX and so on)
  set(LINUX_COMPILE_DEFS
    ${LINUX_COMPILE_DEFS}
    APPLE       # We are building for an APPLE OS
    )

  # No visibility compiler flags for now or we get issues with several vtables
  set(NO_VISIBILITY_CHECK 1)   # Do not add the visibility related compiler flags within "CheckLinuxCompiler.cmake" below
endif ()
if (GCC44 AND GCC_VERSION VERSION_LESS "4.4.6")
  # Add ALWAYS_RTTI_EXPORT define because with this compiler version the RTTI elements must always be exported
  set(LINUX_COMPILE_DEFS
    ${LINUX_COMPILE_DEFS}
    ALWAYS_RTTI_EXPORT
    )
endif ()

# Check GCC compiler
include(cmake/Modules/CheckLinuxCompiler.cmake) # Adds e.g. visibility attribute (http://gcc.gnu.org/wiki/Visibility) and checks for c++0x support


##################################################
## Compiler flags
##################################################

set(LINUX_COMPILE_FLAGS
  ${LINUX_COMPILE_FLAGS}
  -mtune=generic      # Produce code optimized for the most common IA32/AMD64/EM64T processors
  -msse3        # Use SSE3 instruction set / code might not run on CPUs with no sse3 instruction
  -Wstrict-null-sentinel    # Warn also about the use of an uncasted NULL as sentinel
  -std=c++20
  #        -Weffc++      # Warn about violations of the following style guidelines from Scott Meyers' Effective C++ series of books
  # Added automatically by c++14+ -Wsized-deallocation            # Warns about places that might want to add a definition.
  -Wall
  -Werror
  )

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
  set(LINUX_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    -O3        # Very aggressive transformations, long compile times, but usally best runtime performance! (but also dramatically increased binary size, e.g. "libREScene,so" is 4.5 MB when using -O3)
    #	-O2								# Balance speed and binary size (e.g. "libREScene,so" is 2.8 MB)
    #	-Os								# Optimize for binary size (e.g. "libREScene,so" is 2.1 MB when using -Os)
    -funroll-all-loops    # Perform the optimization of loop unrolling
    -fpeel-loops     # Peels the loops for that there is enough information that they do not roll much (from profile feedback)
    -ftree-vectorize    # Enable the vectorizer
    # The following flag usage is basing on information from http://software.intel.com/en-us/forums/showthread.php?t=66902
    --param max-unroll-times=4
    # The following flag usage is basing on information from http://developer.amd.com/documentation/articles/pages/Compiler-FlagDrivenPerformanceGains.aspx
    -fomit-frame-pointer   # Don't keep the frame pointer in a register for functions that don't need one
    -fsized-deallocation   # Enable the built-in global declarations
    )
else ()
  ##################################################
  ## Debug
  ##################################################
  set(LINUX_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    -ggdb       # Use gdb debugger
    )
endif ()


##################################################
## Linker flags
##################################################

if (APPLE)
  # X11 libraries not found on Mac OS X 10.6 - add the X11 library directory manually
  set(LINUX_LINKER_FLAGS
    ${LINUX_LINKER_FLAGS}
    -L/usr/X11/lib
    )
else ()
  set(LINUX_LINKER_FLAGS
    ${LINUX_LINKER_FLAGS}
    -L/usr/X11/lib
    -Wl,--no-as-needed
    )
endif ()
