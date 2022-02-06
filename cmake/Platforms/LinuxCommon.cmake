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
# Determine distribution
################################################################################
find_program(LSB_RELEASE lsb_release)
execute_process(
  COMMAND ${LSB_RELEASE} -is
  OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

################################################################################
# Preprocessor definitions
################################################################################
set(LINUX_COMPILE_DEFS
  LINUX
  PIC
  _REENTRANT
  ARCHITECTURE_STRING="${PL_TARGET_ARCHBITSIZE}"
  )
if ((NOT PL_TARGET_BITSIZE MATCHES 32) AND (CMAKE_SIZEOF_VOID_P MATCHES 8))
  message(STATUS "Setting x64 build preprocessor definitions")

  set(LINUX_COMPILE_DEFS
    ${LINUX_COMPILE_DEFS}
    X64_ARCHITECTURE
    )
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(LINUX_COMPILE_DEFS
    ${LINUX_COMPILE_DEFS}
    _DEBUG
    )
else ()
  set(LINUX_COMPILE_DEFS
    ${LINUX_COMPILE_DEFS}
    NDEBUG
    )
endif ()

################################################################################
# Compiler flags
################################################################################
if ((NOT PL_TARGET_BITSIZE MATCHES 32) AND (CMAKE_SIZEOF_VOID_P MATCHES 8))
  message(STATUS "Add compiler flags for 32Bit on 64Bit host")

  set(LINUX_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    #        -m32
    )
endif ()

set(LINUX_COMPILE_FLAGS
  ${LINUX_COMPILE_FLAGS}
  -std=c++20
  -frtti
  -pthread
  -fPIC
  #    -fno-rtti
  -fno-exceptions
  -pipe
  -ffunction-sections
  -fdata-sections
  -ffast-math
  -Wnon-virtual-dtor
  -Wreorder
  -Wsign-promo
  -Wall
  -Werror
  )

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(LINUX_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    -g3
    -ggdb
    )
endif ()

################################################################################
# Linker flags
################################################################################
if ((NOT PL_TARGET_BITSIZE MATCHES 32) AND (CMAKE_SIZEOF_VOID_P MATCHES 8))
  message(STATUS "Add linker flags for 32Bit on 64Bit host")

  set(LINUX_LINKER_FLAGS
    ${LINUX_LINKER_FLAGS}
    #        -m32
    -static-libstdc++
    )
endif ()

if (NOT APPLE)
  set(LINUX_LINKER_FLAGS
    ${LINUX_LINKER_FLAGS}
    -Wl,--no-as-needed
    -Wl,--gc-sections
    -Wl,--no-undefined
    )

  if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    set(LINUX_LINKER_FLAGS
      ${LINUX_LINKER_FLAGS}
      -Wl,--strip-all
      )
  endif ()
endif ()