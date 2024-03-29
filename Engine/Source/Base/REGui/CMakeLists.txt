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
## Project
##################################################
re_add_target(
  NAME REGui SHARED
  NAMESPACE RE
  FILES_CMAKE
    ${CMAKE_CURRENT_SOURCE_DIR}/REGui_files.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/REGui_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
  #PLATFORM_INCLUDE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/REGui_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
  INCLUDE_DIRECTORIES
    PUBLIC
      ${RE_CONFIG_FILE_LOCATION}
      ${CMAKE_CURRENT_SOURCE_DIR}/Public
      ${CMAKE_CURRENT_SOURCE_DIR}/Private
  BUILD_DEPENDENCIES
    PUBLIC
      RECore
      RERHI
      RERenderer
      External::imgui
      External::imgui_filedialog
  RUNTIME_DEPENDENCIES
    PUBLIC
      RECore
      RERHI
      RERenderer
      External::imgui
      External::imgui_filedialog
  COMPILE_DEFINITIONS
    PUBLIC
      REGUI_EXPORTS
      #EXAMPLES_MIMALLOC
      ${${PAL_PLATFORM_NAME_UPPERCASE}_COMPILE_DEFS}
      ${PAL_PLATFORM_NAME_UPPERCASE}
      GLM_FORCE_CXX17
      GLM_FORCE_INLINE
      GLM_FORCE_SIMD_AVX2
      GLM_FORCE_XYZW_ONLY
      GLM_FORCE_LEFT_HANDED
      GLM_FORCE_DEPTH_ZERO_TO_ONE
      GLM_FORCE_RADIANS
      GLM_ENABLE_EXPERIMENTAL
      RENDERER_IMGUI
      RENDERER_OPENVR
  TARGET_PROPERTIES
    -fPIC
)
