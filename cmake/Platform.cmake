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
# Architecture
################################################################################
if (CMAKE_GENERATOR MATCHES "Visual Studio*")
  if (CMAKE_GENERATOR MATCHES "Win64")
    set(X86_64 1)
  else ()
    set(X86 1)
  endif ()
elseif (CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(X86_64 1)
else ()
  set(X86 1)
endif ()

# Target architecture
if (PL_TARGET_ARCH)
  set(PL_TARGET_ARCH ${PL_TARGET_ARCH} CACHE STRING "Target architecture")
else ()
  set(PL_TARGET_ARCH "x86" CACHE STRING "Target architecture")
endif ()

# Sets the default bit size, depending on host system
if (X86_64 AND PL_TARGET_ARCH MATCHES "x86")
  # Target bitsize
  set(PL_TARGET_BITSIZE "64" CACHE STRING "Target bitsize")

  # Target architecture and bitsize
  set(PL_TARGET_ARCHBITSIZE "x64" CACHE STRING "Target architecture and bitsize")
else ()
  # Target bitsize
  set(PL_TARGET_BITSIZE "32" CACHE STRING "Target bitsize")

  # Target architecture and bitsize
  set(PL_TARGET_ARCHBITSIZE ${PL_TARGET_ARCH} CACHE STRING "Target architecture and bitsize")
endif ()

################################################################################
# Platform specific includes
################################################################################

set(PL_UNSUPPORTED_PLATFORMS Linux Unix Windows Android MacOSX IPhone)
set(PL_SUPPORTED_PLATFORMS "")

set(PL_PLATFORM_COMPILE_FLAGS "")
set(PL_PLATFORM_COMPILE_DEFS "")
set(PL_PLATFORM_LINKER_FLAGS "")
set(PL_PLATFORM_LINKER_DEFS "")

if (UNIX)
  set(LINUX TRUE)
endif ()

if (WIN32)
  include(cmake/Platforms/WinMSVC.cmake)


  set(PL_PLATFORM_COMPILE_FLAGS ${WINDOWS_COMPILE_FLAGS})
  set(PL_PLATFORM_COMPILE_DEFS ${WINDOWS_COMPILE_DEFS})
  set(PL_PLATFORM_LINKER_FLAGS ${WINDOWS_LINKER_FLAGS})
  set(PL_PLATFORM_LINKER_DEFS ${WINDOWS_LINKER_DEFS})
elseif (LINUX)

  # Include finds for all window managers
  include(cmake/Modules/FindMIR.cmake)
  include(cmake/Modules/FindWayland.cmake)
  include(cmake/Modules/FindXCB.cmake)
  find_package(X11)

  # Include all common settings that are equal for all platforms
  include(cmake/Platforms/LinuxCommon.cmake)

  # Should we build for Android
  if (ANDROID)
    include(cmake/Platforms/LinuxNDK.cmake)

    # Should we build for NaCL
  elseif (NACL)
    include(cmake/Platforms/LinuxNaCL.cmake)

    # Use GCC?
  elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    include(cmake/Platforms/LinuxGCC.cmake)

    # Use Clang?
  elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    include(cmake/Platforms/LinuxClang.cmake)

    # Something is wrong ...
  else ()
    message(FATAL_ERROR "Unsupported compiler was specified. Only GNU, Clang and NDK are supported")
  endif ()

  set(PL_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
  set(PL_PLATFORM_COMPILE_DEFS ${LINUX_COMPILE_DEFS})
  set(PL_PLATFORM_LINKER_FLAGS ${LINUX_LINKER_FLAGS})
  set(PL_PLATFORM_LINKER_DEFS ${LINUX_LINKER_DEFS})
endif ()
