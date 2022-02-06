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
include(FindPackageHandleStandardArgs)

find_path(OPENGLES_H_PATH NAMES GLES/gl.h)
find_library(OPENGLES_LIB NAMES GLES_CM)
mark_as_advanced(OPENGLES_H_PATH OPENGLES_LIB OPENGLES_LIB_DBG)

find_package_handle_standard_args(OPENGLES DEFAULT_MSG OPENGLES_LIB OPENGLES_H_PATH)

# set up output vars
if (OPENGLES_FOUND)
  set(OPENGLES_INCLUDE_DIR ${OPENGLES_H_PATH})
  set(OPENGLES_LIBRARIES ${OPENGLES_LIB})
  if (OPENGLES_LIB_DBG)
    set(OPENGLES_LIBRARIES_DBG ${OPENGLES_LIB_DBG})
  endif ()
else ()
  set(OPENGLES_INCLUDE_DIR)
  set(OPENGLES_LIBRARIES)
  set(OPENGLES_LIBRARIES_DBG)
endif ()


find_path(OPENGLES2_H_PATH NAMES GLES2/gl2.h)
find_library(OPENGLES2_LIB NAMES GLES_CM)
mark_as_advanced(OPENGLES2_H_PATH OPENGLES2_LIB OPENGLES2_LIB_DBG)

find_package_handle_standard_args(OPENGLES2 DEFAULT_MSG OPENGLES2_LIB OPENGLES2_H_PATH)
# set up output vars
if (OPENGLES2_FOUND)
  set(OPENGLES2_INCLUDE_DIR ${OPENGLES2_H_PATH})
  set(OPENGLES2_LIBRARIES ${OPENGLES2_LIB})
  if (OPENGLES2_LIB_DBG)
    set(OPENGLES2_LIBRARIES_DBG ${OPENGLES2_LIB_DBG})
  endif ()
else ()
  set(OPENGLES2_INCLUDE_DIR)
  set(OPENGLES2_LIBRARIES)
  set(OPENGLES2_LIBRARIES_DBG)
endif ()


find_path(OPENGLES3_H_PATH NAMES GLES3/gl3.h)
find_library(OPENGLES3_LIB NAMES GLES_CM)
mark_as_advanced(OPENGLES3_H_PATH OPENGLES3_LIB OPENGLES3_LIB_DBG)

find_package_handle_standard_args(OPENGLES3 DEFAULT_MSG OPENGLES3_LIB OPENGLES3_H_PATH)
# set up output vars
if (OPENGLES3_FOUND)
  set(OPENGLES3_INCLUDE_DIR ${OPENGLES3_H_PATH})
  set(OPENGLES3_LIBRARIES ${OPENGLES3_LIB})
  if (OPENGLES3_LIB_DBG)
    set(OPENGLES3_LIBRARIES_DBG ${OPENGLES3_LIB_DBG})
  endif ()
else ()
  set(OPENGLES3_INCLUDE_DIR)
  set(OPENGLES3_LIBRARIES)
  set(OPENGLES3_LIBRARIES_DBG)
endif ()