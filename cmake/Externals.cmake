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


function(re_download_package URL HASH LOCAL_PATH)
  #
  get_filename_component(file_name ${URL} NAME)

  set(fn_with_ext "${file_name}.tar.xz")

  # check if file exists
  if (NOT EXISTS "${LOCAL_PATH}/${fn_with_ext}")
    file(
      DOWNLOAD
      ${URL}
      ${LOCAL_PATH}/${fn_with_ext}
      SHOW_PROGRESS
      TIMEOUT 180
    )
    # check hash sum
    file(
      SHA256
      ${LOCAL_PATH}/${fn_with_ext}
      hash_result
    )
    string(COMPARE EQUAL ${hash_result} ${HASH} is_equal)
    if (${is_equal})
      # extract
      file(
        ARCHIVE_EXTRACT
        INPUT ${LOCAL_PATH}/${fn_with_ext}
        DESTINATION ${LOCAL_PATH}/${file_name}
      )
    else()
      message(FATAL_ERROR "SHA256 does not match for ${fn_with_ext}, got ${hash_result}, expect ${HASH}")
    endif()
  else()
    message(STATUS "Package ${fn_with_ext} is up to date")

    if (NOT EXISTS "${LOCAL_PATH}/${file_name}")
      file(
        ARCHIVE_EXTRACT
        INPUT ${LOCAL_PATH}/${fn_with_ext}
        DESTINATION ${LOCAL_PATH}/${file_name}
      )
    endif()
  endif()
endfunction()


function(re_associate_package)
  set(_singleValues PACKAGE_NAME PACKAGE_HASH)
  set(_multiValues TARGETS)

  cmake_parse_arguments(re_associate_package "" "${_singleValues}" "${_multiValues}" "${ARGN}")

  re_download_package("http://data.racoonstudios.de/${re_associate_package_PACKAGE_NAME}" ${re_associate_package_PACKAGE_HASH} ${RE_EXTERNAL_LIBRARIES})

  if(NOT re_associate_package_TARGETS)
    message(FATAL_ERROR "re_associate_package was called without the TARGETS argument, at least one target is required")
  endif()

  if(NOT re_associate_package_PACKAGE_NAME)
    message(FATAL_ERROR "re_associate_package was called without the PACKAGE_NAME argument, this is required")
  endif()

  if(NOT re_associate_package_PACKAGE_HASH)
    message(FATAL_ERROR "re_associate_package was called without the PACKAGE_HASH argument, this is required")
  endif()

  foreach(pkg ${re_associate_package_TARGETS})
    set_property(GLOBAL PROPERTY RE_PACKAGE_ASSOCIATION_${pkg} ${re_associate_package_PACKAGE_NAME})
    set_property(GLOBAL PROPERTY RE_PACKAGE_HASH_${re_associate_package_PACKAGE_NAME} ${re_associate_package_HASH})
  endforeach()

  set_property(GLOBAL APPEND PROPERTY RE_PACKAGE_NAMES ${re_associate_package_PACKAGE_NAME})
  set_property(GLOBAL PROPERTY RE_PACKAGE_TARGETS_${re_associate_package_PACKAGE_NAME} ${re_associate_package_TARGETS})

  # Add to cmake_module_path
  #message("INFO: re_associate_package_PACKAGE_NAME: ${re_associate_package_PACKAGE_NAME}")
  #message("INFO: re_associate_package_TARGETS: ${re_associate_package_TARGETS}")
  set(CMAKE_MODULE_PATH "${RE_EXTERNAL_LIBRARIES}/${re_associate_package_PACKAGE_NAME}" ${CMAKE_MODULE_PATH} PARENT_SCOPE)

  #message("INFO CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
  foreach(pkg ${re_associate_package_TARGETS})
  #  find_package(${pkg} REQUIRED MODULE)
  endforeach()

endfunction()

include(cmake/Externals/Externals_${PAL_HOST_PLATFORM_NAME_LOWERCASE}.cmake)