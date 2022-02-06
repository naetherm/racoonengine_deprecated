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
## Linux/NaCL platform definitions
##
## This file contains compiler and linker settings which are specific to the NaCL compiler suit under Linux
##################################################


##################################################
## Preprocessor definitions
##################################################

# Preprocessor definitions
set(LINUX_COMPILE_DEFS
  ${LINUX_COMPILE_DEFS}
  __STDC_INT64__      # We need "int64_t" and "uint64_t", so force this preprocessor definition (used in "PLCore/RECoreLinux.h")
  __STDINT_LIMITS      # We need "UINTMAX_MAX" (used in "PLCore/RECoreLinux.cpp")
  NACL        # We are using the NaCL toolchain
  HAVE_VISIBILITY_ATTR    # Use visibility attribute (http://gcc.gnu.org/wiki/Visibility)
  )


##################################################
## Compiler flags
##################################################

set(LINUX_COMPILE_FLAGS
  ${LINUX_COMPILE_FLAGS}
  -fvisibility=hidden     # In order to reduce the binary size, don't put private symbols into the resulting binary (http://gcc.gnu.org/wiki/Visibility)
  -fvisibility-inlines-hidden   # In order to reduce the binary size, don't put private symbols into the resulting binary (http://gcc.gnu.org/wiki/Visibility)
  -Wstrict-null-sentinel    # Warn also about the use of an uncasted NULL as sentinel
  -nostdlib       # Don't use the standard C library of the host system (NaCL uses a stripped down version of libc, called "bionic")
  -nodefaultlibs      # Do not use the standard system libraries when linking
  )

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
  set(LINUX_COMPILE_FLAGS
    ${LINUX_COMPILE_FLAGS}
    -Os        # Optimize for binary size, we're targeting browsers so size matters
    -funroll-all-loops    # Perform the optimization of loop unrolling
    -fpeel-loops     # Peels the loops for that there is enough information that they do not roll much (from profile feedback)
    -ftree-vectorize    # Enable the vectorizer
    # The following flag usage is basing on information from http://software.intel.com/en-us/forums/showthread.php?t=66902
    --param max-unroll-times=4
    )
endif ()

# Add NaCL include directory
include_directories(${ANDROID_NDK_SYSROOT}/usr/include/) # [TODO] NaCL


##################################################
## Linker flags
##################################################

# Add NaCL library directory
set(LINUX_LINKER_FLAGS
  ${LINUX_LINKER_FLAGS}
  -Wl,-rpath-link=${ANDROID_NDK_SYSROOT}/usr/lib/  # [TODO] NaCL
  -L${ANDROID_NDK_SYSROOT}/usr/lib/     # If we don't set this, we get "cannot find -lc" later on
  )

# Above we set "nostdlib" to avoid using the standard C library of the host system.
# Now we have to tell the linker to use NaCL stripped down version of libc,
# present in the location specified by using the "-L" in the "LDFLAGS"-option above.
add_to_list(CMAKETOOLS_CURRENT_LIBS c)