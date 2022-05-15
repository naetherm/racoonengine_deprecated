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

include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFiles)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCSourceRuns)
include(CheckCCompilerFlag)
include(CheckTypeSize)
include(CheckStructHasMember)
include(CMakeDependentOption)
include(FindPkgConfig)
include(GNUInstallDirs)

macro(re_add_subdirectory dir)
  add_subdirectory(${dir})

  set(RE_MANUAL_SUBDIRS ${RE_MANUAL_SUBDIRS} ${dir})
endmacro()

macro(re_set_current_package name)
  set(RE_CURRENT_PACKAGE ${name})
  set(RE_CURRENT_PACKAGE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

################################################################################
# MACRO: re_add_all_subdirectories
################################################################################
macro(re_add_all_subdirectories)
  # First, collect all subdirectories containing a CMakeLists.txt
  set(directory_list "")
  file(GLOB_RECURSE new_list CMakeLists.txt)

  # Loop through all collected files and directories
  foreach (file ${new_list})
    get_filename_component(dir ${file} PATH)

    if (NOT ${dir} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
      set(dir_list ${dir_list} ${dir})
    endif ()
  endforeach ()

  list(REMOVE_DUPLICATES dir_list)
  foreach (arg in ${ARGN})
    list(APPEND ignoreList ${arg})
  endforeach ()

  # Collected all files, now add them
  foreach (dir ${dir_list})
    get_filename_component(tempfile ${dir} NAME)
    list(FIND ignoreList "${tempfile}" _index)
    # In theory this is the new syntax since cmake >= 3.3 but this is not working!
    #if (NOT ${tempfile} IN_LIST ARGN)
    if (NOT _index GREATER -1)
      add_subdirectory(${dir})
    endif ()
  endforeach ()
endmacro()

################################################################################
# MACRO: re_collection_source_files
################################################################################
macro(re_collection_source_files)

  # Collection all files
  file(
    GLOB_RECURSE all_files
    *.cpp *.h *.hpp *.rc *.inl
  )

  # First, remove duplicates
  list(REMOVE_DUPLICATES all_files)


  # And now filter out all files that do not belong to the current platform
  foreach (file ${all_files})
    # In feature specific folders
    set(file_in ON)

    foreach (platform ${RE_UNSUPPORTED_PLATFORMS})
      if (file MATCHES "/\\${platform}")
        list(REMOVE_ITEM all_files ${file})
        set(file_in OFF)

        break()
      endif ()
    endforeach ()

    if (file_in)

      # Remove supported platform filters out of the file name before processing features
      set(file_name ${file})
      foreach (platform ${RE_SUPPORTED_PLATFORMS})
        string(REGEX REPLACE "/\\${platform}/" "" file_name ${file_name})
      endforeach ()

      # If the filename no longer contains any feature identifiers, keep it
      if (file_name MATCHES "/\\+")

        # Let's browse through all the features the user wants to enable and look
        # if this file belong to one of them
        set(keep_file OFF)
        foreach (feature ${RE_CURRENT_LIST_OF_FEATURES})

          if (file_name MATCHES "/\\+${feature}/")
            set(keep_file ON)
            break()
          endif ()
        endforeach ()

        if (NOT keep_file)
          list(REMOVE_ITEM all_files ${file})
        endif ()
      endif ()

    endif ()
  endforeach ()

  # Add all remaining files to the list
  set(RE_CURRENT_SOURCES ${all_files})
endmacro()

################################################################################
# MACRO: re_group_sources
################################################################################
macro(re_group_sources)
  foreach (file ${RE_CURRENT_SOURCES})
    # Get the file path
    get_filename_component(path ${file} PATH)

    macro(group_sources_helper dir_path subdir)
      string(REGEX MATCH "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/" path_match ${dir_path})

      if (path_match)
        string(REPLACE ${path_match} "" out_path ${dir_path})

        string(REPLACE "/" "\\" out_path ${out_path})

        source_group(${out_path} FILES ${file})
      endif ()
    endmacro()

    group_sources_helper(${path} "Public")
    group_sources_helper(${path} "Private")
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_feature
#
# Adds a feature to the current target
################################################################################
macro(re_add_feature directory dependency flag)
  # Add feature to list of features
  set(RE_CURRENT_LIST_OF_FEATURES ${RE_CURRENT_LIST_OF_FEATURES} "${directory}")

  # Add dependency
  re_add_dependency(${dependency})

  # Add flag
  re_add_compile_flags(-D${flag})
endmacro()

################################################################################
# MACRO: re_add_include_paths
#
# Adds additional include paths to the current project.
################################################################################
macro(re_add_include_paths)
  foreach (path ${ARGN})
    # re_debug_message("For ${RE_CURRENT_PROJECT_NAME}; add include path: ${path}")
    set(RE_CURRENT_INCLUDES ${RE_CURRENT_INCLUDES} ${path})
  endforeach ()
endmacro()
macro(re_add_library_paths)
  foreach (path ${ARGN})
    # re_debug_message("For ${RE_CURRENT_PROJECT_NAME}; add library path: ${path}")
    set(RE_CURRENT_LINK_DIRS ${RE_CURRENT_LINK_DIRS} ${path})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_libs
################################################################################
macro(re_add_libs)
  foreach (lib ${ARGN})
    set(RE_CURRENT_EXT_LIBS ${RE_CURRENT_EXT_LIBS} ${lib})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_compile_defs
################################################################################
macro(re_add_compile_defs)
  foreach (var ${ARGN})
    set(RE_CURRENT_COMPILE_DEFS ${RE_CURRENT_COMPILE_DEFS} ${var})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_compile_flags
################################################################################
macro(re_add_compile_flags)
  foreach (var ${ARGN})
    set(RE_CURRENT_COMPILE_FLAGS "${RE_CURRENT_COMPILE_FLAGS} ${var} ")
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_linker_flags
################################################################################
macro(re_add_linker_flags)
  foreach (var ${ARGN})
    set(RE_CURRENT_LINKER_FLAGS "${RE_CURRENT_LINKER_FLAGS} ${var} ")
  endforeach ()
endmacro()


################################################################################
# MACRO: re_remove_compile_defs
################################################################################
macro(re_remove_compile_defs)
  foreach (var ${ARGN})
    list(REMOVE_ITEM RE_CURRENT_COMPILE_DEFS ${var})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_dependencies
################################################################################
macro(re_add_dependencies)
  foreach (dependency ${ARGN})
    set(RE_CURRENT_DEPENDENCIES ${RE_CURRENT_DEPENDENCIES} ${dependency})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_add_dependency
################################################################################
macro(re_add_dependency module)
  # Add include paths to the module
  re_add_include_paths(${${module}_INCLUDE_DIRS})
  re_add_include_paths(${${module}_EXT_INCLUDE_DIRS})
  re_add_include_paths(${RE_CURRENT_EXT_INCLUDE_DIRS})

  # Add link libraries

  re_add_libs(${${module}_LIBRARIES})# ${${module}_EXT_LIBS})

  # Add the dependency
  re_add_dependencies(${module})
endmacro()

macro(re_add_sources)
  foreach (source ${ARGN})
    set(RE_CURRENT_SOURCES ${RE_CURRENT_SOURCES} ${source})
  endforeach ()
endmacro()

################################################################################
# MACRO: re_project
#
# Defines a new project.
################################################################################
macro(re_external_project name)

  # Define the project variables
  set(RE_CURRENT_PROJECT_NAME ${name})
  set(RE_CURRENT_OUTPUT_NAME "${RE_CURRENT_PACKAGE}.${name}")

  # The IDE path
  string(REPLACE "${RE_CURRENT_PACKAGE_ROOT}" "" RE_CURRENT_IDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
  set(RE_CURRENT_IDE_PATH ${RE_CURRENT_PACKAGE}${RE_CURRENT_IDE_PATH})
  get_filename_component(RE_CURRENT_IDE_PATH ${RE_CURRENT_IDE_PATH} DIRECTORY)

  # create the project
  project(${RE_CURRENT_PROJECT_NAME} C CXX ${RE_RC_COMPILER})

  # initialize project lists
  set(RE_CURRENT_SOURCES "")
  set(RE_CURRENT_INCLUDES "")
  set(RE_CURRENT_EXT_LIBS "")
  set(RE_CURRENT_LINK_DIRS "")
  set(RE_CURRENT_COMPILE_DEFS "")
  set(RE_CURRENT_COMPILE_FLAGS "")
  set(RE_CURRENT_LINKER_FLAGS "")
  set(RE_CURRENT_DEPENDENCIES "")
  set(RE_CURRENT_EXT_INCLUDE_DIRS "")

  set(RE_CURRENT_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Download)
  set(RE_CURRENT_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/Source)
  set(RE_CURRENT_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Build)
  set(RE_CURRENT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Install)
  set(RE_CURRENT_TMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/Temp)
  set(RE_CURRENT_STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/Stamp)

  # Set the default includes paths
  re_add_include_paths(
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
    ${RE_CONFIG_FILE_LOCATION}
  )

  # Set default platform compile definitions
  #    re_group_sources()
  if (WIN32)
    re_add_compile_defs(${WIN32_COMPILE_DEFS})
  elseif (LINUX)
    re_add_compile_defs(${LINUX_COMPILE_DEFS})
  endif ()

  # Current project's include path
  set(RE_CURRENT_PROJECT_INCLUDE_DIRS ${RE_CURRENT_PROJECT_NAME}_INCLUDE_DIRS)

  # Default value
  set(${RE_CURRENT_PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Public CACHE INTERNAL "")
  set(${RE_CURRENT_PROJECT_NAME}_INCLUDE_DIRS ${RE_CURRENT_PROJECT_INCLUDE_DIRS} CACHE INTERNAL "")

  # Current project's output link libraries
  set(RE_CURRENT_PROJECT_LIBRARIES ${RE_CURRENT_PROJECT_NAME}_LIBRARIES)
  set(${RE_CURRENT_PROJECT_NAME}_EXT_LIBS ${RE_CURRENT_EXT_LIBS})

endmacro()

#macro(re_add_include_paths)
#    foreach(path ${ARGN})
#        # re_debug_message("For ${RE_CURRENT_PROJECT_NAME}; add include path: ${path}")
#        set(RE_CURRENT_INCLUDE_DIRS ${RE_CURRENT_INCLUDE_DIRS} ${path})
#    endforeach()
#endmacro()
macro(re_set_external_install_dirs dir)
  set(${RE_CURRENT_PROJECT_NAME}_INCLUDE_DIRS ${dir} CACHE INTERNAL "")
endmacro()

################################################################################
# MACRO: re_project
#
# Defines a new project.
################################################################################
macro(re_project name)

  # Define the project variables
  set(RE_CURRENT_PROJECT_NAME ${name})
  set(RE_CURRENT_OUTPUT_NAME "${RE_CURRENT_PACKAGE}.${name}")

  # The IDE path
  string(REPLACE "${RE_CURRENT_PACKAGE_ROOT}" "" RE_CURRENT_IDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
  set(RE_CURRENT_IDE_PATH ${RE_CURRENT_PACKAGE}${RE_CURRENT_IDE_PATH})
  get_filename_component(RE_CURRENT_IDE_PATH ${RE_CURRENT_IDE_PATH} DIRECTORY)

  # create the project
  project(${RE_CURRENT_PROJECT_NAME} C CXX ${RE_RC_COMPILER})

  # initialize project lists
  set(RE_CURRENT_SOURCES "")
  set(RE_CURRENT_INCLUDES "")
  set(RE_CURRENT_EXT_LIBS "")
  set(RE_CURRENT_COMPILE_DEFS "")
  set(RE_CURRENT_LINK_DIRS "")
  set(RE_CURRENT_COMPILE_FLAGS "")
  set(RE_CURRENT_LINKER_FLAGS "")
  set(RE_CURRENT_DEPENDENCIES "")
  set(RE_CURRENT_EXT_INCLUDE_DIRS "")
  set(RE_CURRENT_LIST_OF_FEATURES "")

  set(RE_CURRENT_PROJECT_DIR ${CMAKE_CURRENT_BINARY_DIR})
  set(RE_CURRENT_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Download)
  set(RE_CURRENT_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/Source)
  set(RE_CURRENT_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Build)
  set(RE_CURRENT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Install)
  set(RE_CURRENT_TMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/Temp)
  set(RE_CURRENT_STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/Stamp)

  # Create specific include path
  set(${name}_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Public)

  # Set the default includes paths
  re_add_include_paths(
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
    ${RE_CONFIG_FILE_LOCATION}
  )

  # Set default platform compile definitions
  #    re_group_sources()
  if (WIN32)
    re_add_compile_defs(${WIN32_COMPILE_DEFS})
  elseif (LINUX)
    re_add_compile_defs(${LINUX_COMPILE_DEFS})
  endif ()

  # Current project's include path
  set(RE_CURRENT_PROJECT_INCLUDE_DIRS ${RE_CURRENT_PROJECT_NAME}_INCLUDE_DIRS)

  # Default value
  set(RE_CURRENT_PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Public CACHE INTERNAL "")

  set(${RE_CURRENT_PROJECT_NAME}_INCLUDE_DIRS ${RE_CURRENT_PROJECT_INCLUDE_DIRS} CACHE INTERNAL "")

  # Current project's output link libraries
  set(RE_CURRENT_PROJECT_LIBRARIES ${RE_CURRENT_PROJECT_NAME}_LIBRARIES)
  set(${RE_CURRENT_PROJECT_NAME}_EXT_LIBS ${RE_CURRENT_EXT_LIBS} CACHE INTERNAL "")

  re_add_include_paths(${${RE_CURRENT_PROJECT_NAME}_EXT_INCLUDE_DIRS} ${RE_CURRENT_EXT_INCLUDE_DIRS})

endmacro()

################################################################################
# MACRO: re_build_library
################################################################################
macro(re_build_library type)
  set(lib_type ${type})

  if ("${lib_type}" STREQUAL "")
    set(lib_type "STATIC")
  elseif("${lib_type}" STREQUAL "PLUGIN")
    set(lib_type "SHARED")
  endif ()

  if ("${type}" STREQUAL "SHARED" OR "${type}" STREQUAL "PLUGIN")
    re_add_compile_defs(_WINDLL _USRDLL)

    set(lib_type "SHARED")
  else ()
    re_remove_compile_defs(_WINDLL _USRDLL)
  endif ()

  if (LINUX)
    if ("${type}" STREQUAL "STATIC")

      re_add_compile_flags("-fPIC")
    endif ()
  endif ()

  # Add library to build
  add_library(${RE_CURRENT_PROJECT_NAME} ${lib_type} ${RE_CURRENT_SOURCES})

  # Generate the folder structure in IDE
  re_group_sources()

  # Includes
  include_directories(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_INCLUDES})

  link_directories(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_LINK_DIRS})

  #set_target_properties(
  #  ${RE_CURRENT_PROJECT_NAME}
  #  PROPERTIES IMPORTED_LOCATION ${RE_CURRENT_DEPENDENCIES})

  # Link all libraries
  target_link_libraries(
    ${RE_CURRENT_PROJECT_NAME}
    PUBLIC
    ${RE_CURRENT_EXT_LIBS})

  message("Current RE_CURRENT_EXT_LIBS")
  message(${RE_CURRENT_EXT_LIBS})

  # Dependencies
  if (NOT "${RE_CURRENT_DEPENDENCIES}" STREQUAL "")
    message("CURRENT RE_CURRENT_DEPENDENCIES")
    message(${RE_CURRENT_DEPENDENCIES})
    add_dependencies(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_DEPENDENCIES})
  endif ()

  # Apply cotire
  # cotire(${RE_CURRENT_PROJECT_NAME})

  # Preprocessor definitions
  set_target_properties(
    ${RE_CURRENT_PROJECT_NAME}
    PROPERTIES
    COMPILE_DEFINITIONS
    "${RE_CURRENT_COMPILE_DEFS}"
  )
  set_target_properties(
    ${RE_CURRENT_PROJECT_NAME}
    PROPERTIES
    COMPILE_FLAGS
    "${RE_CURRENT_COMPILE_FLAGS}"
  )

  # Solution folders
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY
    FOLDER
    "${RE_CURRENT_IDE_PATH}"
  )

  # Output file
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY
    OUTPUT_NAME
    ${RE_CURRENT_OUTPUT_NAME}
  )

  # Set SO versions
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY
    VERSION
    ${RE_PROJECT_VERSION_MAJOR}.${RE_PROJECT_VERSION_MINOR}.${RE_PROJECT_VERSION_PATCH}
  )
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY
    SOVERSION
    ${RE_PROJECT_VERSION_MAJOR}
  )

  # Input libraries for other projects
  set(${RE_CURRENT_PROJECT_NAME}_LIBRARIES ${RE_CURRENT_PROJECT_NAME} CACHE INTERNAL "")
  set(${RE_CURRENT_PROJECT_NAME}_EXT_LIBS ${RE_CURRENT_EXT_LIBS} CACHE INTERNAL "")

  # If the library should be interpreted and used as a plugin we have to create a module
  # description file
  if ("${type}" STREQUAL "PLUGIN")
    re_create_plugin()
  endif ()

  # Installation runtime and link libraries and then the include directory
  install(
    TARGETS ${RE_CURRENT_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT SDK
  )
  install(
    DIRECTORY ${${RE_CURRENT_PROJECT_INCLUDE_DIRS}}/${RE_CURRENT_PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  ) # DESTINATION ${CMAKE_BINARY_DIR}

endmacro()


################################################################################
# MACRO: re_create_plugin
#
# Will automatically add the REPlugin project to the dependencies list of
# the current library. It will also add a post_build command to create the
# necessary module description file.
################################################################################
macro(re_create_plugin)

  # Add a dependency to REPlugin so that we can be save the project was compiled
  # before using it
  re_add_dependencies(PLPlugin)

  # The first will receive the exact location of the compiled library
  # The second will receive the path to this library
  #set(library_file $<TARGET_FILE:${RE_CURRENT_PROJECT_NAME}>)
  #get_filename_component(library_path ${lib_file} PATH)
  #get_filename_component(library_name ${lib_file} NAME)

  #set(replugin_file $<TARGET_FILE:REPlugin>)
  #get_property(lib_file TARGET REPlugin PROPERTY LOCATION)
  #get_filename_component(replugin_path ${lib_file} PATH)

  # Finally, after receiving all necessary information, we'll call REPlugin
  # with this information
  #     message("Will now show the correct directory of REPlugin")
  #     message($<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}>)
  # re_debug_message("DO THIS: $<TARGET_FILE:REPlugin> $<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}> --entry-file $<TARGET_FILE_NAME:${RE_CURRENT_PROJECT_NAME}> --write-plugin --verbose --debug --output-path $<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}>")
  add_custom_command(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    POST_BUILD
    COMMAND $<TARGET_FILE:PLPlugin> $<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}> --module-path $<TARGET_LINKER_FILE:${RE_CURRENT_PROJECT_NAME}> --verbose --output-path $<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}>
    #       WORKING_DIRECTORY ${replugin_path}
    #$<TARGET_FILE_DIR:${RE_CURRENT_PROJECT_NAME}>
  )

endmacro()

################################################################################
# MACRO: re_build_executable
################################################################################
macro(re_build_executable subsystem)

  if (WIN32)
    if ("${subsystem}" MATCHES "WIN32")
      set(subsys WIN32)
    else ()
      set(subsys "")
    endif ()

    # Add build options for subsystem
    if ("${subsys}" MATCHES "WIN32")
      re_remove_compile_defs(_CONSOLE)
      re_add_compile_defs(_WINDOWS)
    else ()
      re_remove_compile_defs(_WINDOWS)
      re_add_compile_defs(_CONSOLE)
    endif ()
  endif ()

  re_add_compile_flags("-fPIC")

  # Add the executable to the build
  add_executable(${RE_CURRENT_PROJECT_NAME} ${subsys} ${RE_CURRENT_SOURCES})

  # Generate folder structure in IDE
  re_group_sources()

  # Add includes
  include_directories(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_INCLUDES})

  link_directories(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_LINK_DIRS})



  #set_target_properties(
  #  ${RE_CURRENT_PROJECT_NAME}
  #  PROPERTIES IMPORTED_LOCATION ${RE_CURRENT_EXT_LIBS} ${RE_CURRENT_DEPENDENCIES})

  # Gather all external libraries
  #foreach(EXT ${RE_CURRENT_DEPENDENCIES})
  #  set(RE_CURRENT_EXT_LIBS ${RE_CURRENT_EXT_LIBS} ${${EXT}_EXT_LIBS})
  #endforeach()
  # Link libraries
  target_link_libraries(
    ${RE_CURRENT_PROJECT_NAME}
    PUBLIC
    ${RE_CURRENT_EXT_LIBS})
  # Dependencies
  if (NOT "${RE_CURRENT_DEPENDENCIES}" STREQUAL "")
    add_dependencies(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_DEPENDENCIES})
  endif ()

  # Preprocessor defintions
  set_target_properties(
    ${RE_CURRENT_PROJECT_NAME}
    PROPERTIES
    COMPILE_DEFINITIONS
    "${RE_CURRENT_COMPILE_DEFS}"
  )
  set_target_properties(
    ${RE_CURRENT_PROJECT_NAME}
    PROPERTIES
    COMPILE_FLAGS
    "${RE_CURRENT_COMPILE_FLAGS}"
  )
  set_target_properties(
    ${RE_CURRENT_PROJECT_NAME}
    PROPERTIES
    LINK_FLAGS
    "${RE_CURRENT_LINKER_FLAGS}"
  )

  # Solution folders
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY FOLDER "${RE_CURRENT_IDE_PATH}"
  )

  # Output file
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY
    OUTPUT_NAME
    ${RE_CURRENT_OUTPUT_NAME}
  )

  # Install
  install(
    TARGETS ${RE_CURRENT_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin COMPONENT SDK
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT SDK
  )
endmacro()

macro(re_build_external)
  set_property(
    TARGET ${RE_CURRENT_PROJECT_NAME}
    PROPERTY FOLDER "${RE_CURRENT_PACKAGE}"
  )

  if (NOT "${RE_CURRENT_DEPENDENCIES}" STREQUAL "")
    add_dependencies(${RE_CURRENT_PROJECT_NAME} ${RE_CURRENT_DEPENDENCIES})
  endif ()
endmacro()


macro(re_memory_check)
  if (UNIX)
    find_program(VALGRIND valgrind)
    if (VALGRIND)
      add_custom_target(
        ${RE_CURRENT_PROJECT_NAME}_MemCheck
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./Tests.${RE_CURRENT_PROJECT_NAME}
        DEPENDS ${RE_CURRENT_DEPENDENCIES} ${RE_CURRENT_PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    endif ()
  endif ()
endmacro(re_memory_check)


################################################################################
# MACRO: re_build_documentation
################################################################################
macro(re_build_documentation)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif ()

  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(Documentation
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endmacro()


################################################################################
# MACRO: re_doc_project
################################################################################
macro(re_doc_project name)
  # GetBegin project
  project(${RE_CURRENT_PACKAGE}.${name})

  # Set project variables
  set(RE_CURRENT_PROJECT ${name})
  set(RE_CURRENT_TARGET ${name})
  set(RE_CURRENT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Documentation/${RE_CURRENT_TARGET})

  # Declare a target for the project
  add_custom_target(${RE_CURRENT_PACKAGE}.${RE_CURRENT_TARGET})
endmacro(re_doc_project name)


################################################################################
# MACRO: re_add_dvi_document
################################################################################
macro(re_add_dvi_document target texfile)
  # Set project variables
  set(RE_CURRENT_TARGET_TEX ${texfile})
  set(RE_CURRENT_TARGET_DVI ${RE_CURRENT_OUTPUT_DIR}/${target}.dvi)

  # Check if LaTeX compiler is available
  if (LATEX_COMPILER)
    # Invoke LaTeX compiler (three times needed actually to get everything right)
    add_custom_command(
      OUTPUT ${RE_CURRENT_TARGET_DVI}
      DEPENDS ${RE_CURRENT_TARGET_TEX}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${LATEX_COMPILER}
      ARGS -halt-on-error -quiet -output-directory=${RE_CURRENT_OUTPUT_DIR} ${RE_CURRENT_TARGET_TEX}
      COMMAND ${LATEX_COMPILER}
      ARGS -halt-on-error -quiet -output-directory=${RE_CURRENT_OUTPUT_DIR} ${RE_CURRENT_TARGET_TEX}
      COMMAND ${LATEX_COMPILER}
      ARGS -halt-on-error -quiet -output-directory=${RE_CURRENT_OUTPUT_DIR} ${RE_CURRENT_TARGET_TEX}
      COMMENT "Compiling LaTeX: ${target}.dvi"
    )

    # Declare a target
    add_custom_target(${RE_CURRENT_PACKAGE}.${target}-DVI
      DEPENDS ${RE_CURRENT_TARGET_DVI}
      )

    # Depend Docs-target on DVI-target
    # add_dependencies(Docs-${RE_CURRENT_TARGET} Docs-${target}-DVI)
  endif ()
endmacro(re_add_dvi_document)


macro(re_add_pdf_document target texfile)
  # Set project variables
  #    set(RE_CURRENT_TARGET_DVI ${dvifile})
  set(RE_CURRENT_TARGET_PDF ${RE_CURRENT_OUTPUT_DIR}/${target}.pdf)

  # Check if LaTeX compiler and PDF converter are available
  if (LATEX_COMPILER AND PDFLATEX_COMPILER)
    # Invoke PDF converter
    add_custom_command(
      OUTPUT ${RE_CURRENT_TARGET_PDF}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${PDFLATEX_COMPILER}
      ARGS ${texfile}
      COMMENT "Generating PDF: ${target}.pdf"
    )

    # Declare a target
    add_custom_target(${RE_CURRENT_PACKAGE}.${target}-PDF
      DEPENDS ${RE_CURRENT_TARGET_PDF}
      )

    #        file (COPY "${CMAKE_CURRENT_SOURCE_DIR}/${target}.pdf"
    #                DESTINATION "${RE_CURRENT_OUTPUT_DIR}")

  endif ()
endmacro(re_add_pdf_document)


# Requires:
# - n/a
# Optional:
# - X11_SHARED opt
# - HAVE_DLOPEN opt
macro(CheckX11)
  if (VIDEO_X11)
    foreach (_LIB X11 Xext Xcursor Xinerama Xi Xrandr Xrender Xss Xxf86vm)
      FindLibraryAndSONAME("${_LIB}")
    endforeach ()

    find_path(X_INCLUDEDIR X11/Xlib.h)
    if (X_INCLUDEDIR)
      set(X_CFLAGS "-I${X_INCLUDEDIR}")
    endif ()

    check_include_file(X11/Xcursor/Xcursor.h HAVE_XCURSOR_H)
    check_include_file(X11/extensions/Xinerama.h HAVE_XINERAMA_H)
    check_include_file(X11/extensions/XInput2.h HAVE_XINPUT_H)
    check_include_file(X11/extensions/Xrandr.h HAVE_XRANDR_H)
    check_include_file(X11/extensions/Xrender.h HAVE_XRENDER_H)
    check_include_file(X11/extensions/scrnsaver.h HAVE_XSS_H)
    check_include_file(X11/extensions/shape.h HAVE_XSHAPE_H)
    check_include_files("X11/Xlib.h;X11/extensions/xf86vmode.h" HAVE_XF86VM_H)
    check_include_files("X11/Xlib.h;X11/Xproto.h;X11/extensions/Xext.h" HAVE_XEXT_H)

    if (X11_LIB)
      if (NOT HAVE_XEXT_H)
        message_error("Missing Xext.h, maybe you need to install the libxext-dev package?")
      endif ()

      set(HAVE_VIDEO_X11 TRUE)
      set(HAVE_SDL_VIDEO TRUE)

      file(GLOB X11_SOURCES ${SDL2_SOURCE_DIR}/src/video/x11/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${X11_SOURCES})
      set(SDL_VIDEO_DRIVER_X11 1)

      if (APPLE)
        set(X11_SHARED OFF)
      endif ()

      check_function_exists("shmat" HAVE_SHMAT)
      if (NOT HAVE_SHMAT)
        check_library_exists(ipc shmat "" HAVE_SHMAT)
        if (HAVE_SHMAT)
          list(APPEND EXTRA_LIBS ipc)
        endif ()
        if (NOT HAVE_SHMAT)
          add_definitions(-DNO_SHARED_MEMORY)
          set(X_CFLAGS "${X_CFLAGS} -DNO_SHARED_MEMORY")
        endif ()
      endif ()

      if (X11_SHARED)
        if (NOT HAVE_DLOPEN)
          message_warn("You must have SDL_LoadObject() support for dynamic X11 loading")
          set(HAVE_X11_SHARED FALSE)
        else ()
          set(HAVE_X11_SHARED TRUE)
        endif ()
        if (HAVE_X11_SHARED)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC "\"${X11_LIB_SONAME}\"")
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT "\"${XEXT_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${X11_LIB} ${XEXT_LIB})
        endif ()
      endif ()

      set(SDL_CFLAGS "${SDL_CFLAGS} ${X_CFLAGS}")

      set(CMAKE_REQUIRED_LIBRARIES ${X11_LIB} ${X11_LIB})
      check_c_source_compiles("
          #include <X11/Xlib.h>
          #include <X11/Xproto.h>
          #include <X11/extensions/Xext.h>
          #include <X11/extensions/extutil.h>
          extern XExtDisplayInfo* XextAddDisplay(XExtensionInfo* a,Display* b,_Xconst char* c,XExtensionHooks* d,int e,XPointer f);
          int main(int argc, char **argv) {}" HAVE_CONST_XEXT_ADDDISPLAY)
      if (HAVE_CONST_XEXT_ADDDISPLAY)
        set(SDL_VIDEO_DRIVER_X11_CONST_PARAM_XEXTADDDISPLAY 1)
      endif ()

      check_c_source_compiles("
          #include <X11/Xlib.h>
          int main(int argc, char **argv) {
            Display *display;
            XEvent event;
            XGenericEventCookie *cookie = &event.xcookie;
            XNextEvent(display, &event);
            XGetEventData(display, cookie);
            XFreeEventData(display, cookie); }" HAVE_XGENERICEVENT)
      if (HAVE_XGENERICEVENT)
        set(SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1)
      endif ()

      check_function_exists(XkbKeycodeToKeysym SDL_VIDEO_DRIVER_X11_HAS_XKBKEYCODETOKEYSYM)

      if (VIDEO_X11_XCURSOR AND HAVE_XCURSOR_H)
        set(HAVE_VIDEO_X11_XCURSOR TRUE)
        if (HAVE_X11_SHARED AND XCURSOR_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XCURSOR "\"${XCURSOR_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XCURSOR_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XCURSOR 1)
      endif ()

      if (VIDEO_X11_XINERAMA AND HAVE_XINERAMA_H)
        set(HAVE_VIDEO_X11_XINERAMA TRUE)
        if (HAVE_X11_SHARED AND XINERAMA_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XINERAMA "\"${XINERAMA_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XINERAMA_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XINERAMA 1)
      endif ()

      if (VIDEO_X11_XINPUT AND HAVE_XINPUT_H)
        set(HAVE_VIDEO_X11_XINPUT TRUE)
        if (HAVE_X11_SHARED AND XI_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XINPUT2 "\"${XI_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XI_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XINPUT2 1)

        # Check for multitouch
        check_c_source_compiles("
            #include <X11/Xlib.h>
            #include <X11/Xproto.h>
            #include <X11/extensions/XInput2.h>
            int event_type = XI_TouchBegin;
            XITouchClassInfo *t;
            Status XIAllowTouchEvents(Display *a,int b,unsigned int c,Window d,int f)
            {
              return (Status)0;
            }
            int main(int argc, char **argv) {}" HAVE_XINPUT2_MULTITOUCH)
        if (HAVE_XINPUT2_MULTITOUCH)
          set(SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH 1)
        endif ()
      endif ()

      if (VIDEO_X11_XRANDR AND HAVE_XRANDR_H)
        if (HAVE_X11_SHARED AND XRANDR_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XRANDR "\"${XRANDR_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XRANDR_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XRANDR 1)
        set(HAVE_VIDEO_X11_XRANDR TRUE)
      endif ()

      if (VIDEO_X11_XSCRNSAVER AND HAVE_XSS_H)
        if (HAVE_X11_SHARED AND XSS_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XSS "\"${XSS_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XSS_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XSCRNSAVER 1)
        set(HAVE_VIDEO_X11_XSCRNSAVER TRUE)
      endif ()

      if (VIDEO_X11_XSHAPE AND HAVE_XSHAPE_H)
        set(SDL_VIDEO_DRIVER_X11_XSHAPE 1)
        set(HAVE_VIDEO_X11_XSHAPE TRUE)
      endif ()

      if (VIDEO_X11_XVM AND HAVE_XF86VM_H)
        if (HAVE_X11_SHARED AND XXF86VM_LIB)
          set(SDL_VIDEO_DRIVER_X11_DYNAMIC_XVIDMODE "\"${XXF86VM_LIB_SONAME}\"")
        else ()
          list(APPEND EXTRA_LIBS ${XXF86VM_LIB})
        endif ()
        set(SDL_VIDEO_DRIVER_X11_XVIDMODE 1)
        set(HAVE_VIDEO_X11_XVM TRUE)
      endif ()

      set(CMAKE_REQUIRED_LIBRARIES)
    endif ()
  endif ()
endmacro()


# Requires:
# - EGL
# - PkgCheckModules
# Optional:
# - MIR_SHARED opt
# - HAVE_DLOPEN opt
macro(CheckMir)
  if (VIDEO_MIR)
    find_library(MIR_LIB mirclient mircommon egl)
    pkg_check_modules(MIR_TOOLKIT mirclient mircommon)
    pkg_check_modules(EGL egl)
    pkg_check_modules(XKB xkbcommon)

    if (MIR_LIB AND MIR_TOOLKIT_FOUND AND EGL_FOUND AND XKB_FOUND)
      set(HAVE_VIDEO_MIR TRUE)
      set(HAVE_SDL_VIDEO TRUE)

      file(GLOB MIR_SOURCES ${SDL2_SOURCE_DIR}/src/video/mir/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${MIR_SOURCES})
      set(SDL_VIDEO_DRIVER_MIR 1)

      list(APPEND EXTRA_CFLAGS ${MIR_TOOLKIT_CFLAGS} ${EGL_CLFAGS} ${XKB_CLFLAGS})

      if (MIR_SHARED)
        if (NOT HAVE_DLOPEN)
          message_warn("You must have SDL_LoadObject() support for dynamic Mir loading")
        else ()
          FindLibraryAndSONAME(mirclient)
          FindLibraryAndSONAME(xkbcommon)
          set(SDL_VIDEO_DRIVER_MIR_DYNAMIC "\"${MIRCLIENT_LIB_SONAME}\"")
          set(SDL_VIDEO_DRIVER_MIR_DYNAMIC_XKBCOMMON "\"${XKBCOMMON_LIB_SONAME}\"")
          set(HAVE_MIR_SHARED TRUE)
        endif ()
      else ()
        set(EXTRA_LIBS ${MIR_TOOLKIT_LIBRARIES} ${EXTRA_LIBS})
      endif ()
    endif ()
  endif ()
endmacro()


macro(WaylandProtocolGen _SCANNER _XML _PROTL)
  set(_WAYLAND_PROT_C_CODE "${CMAKE_CURRENT_BINARY_DIR}/wayland-generated-protocols/${_PROTL}-protocol.c")
  set(_WAYLAND_PROT_H_CODE "${CMAKE_CURRENT_BINARY_DIR}/wayland-generated-protocols/${_PROTL}-client-protocol.h")

  add_custom_command(
    OUTPUT "${_WAYLAND_PROT_H_CODE}"
    DEPENDS "${_XML}"
    COMMAND "${_SCANNER}"
    ARGS client-header "${_XML}" "${_WAYLAND_PROT_H_CODE}"
  )

  add_custom_command(
    OUTPUT "${_WAYLAND_PROT_C_CODE}"
    DEPENDS "${_WAYLAND_PROT_H_CODE}"
    COMMAND "${_SCANNER}"
    ARGS code "${_XML}" "${_WAYLAND_PROT_C_CODE}"
  )

  set(SOURCE_FILES ${SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/wayland-generated-protocols/${_PROTL}-protocol.c")
endmacro()

# Requires:
# - EGL
# - PkgCheckModules
# Optional:
# - WAYLAND_SHARED opt
# - HAVE_DLOPEN opt
macro(CheckWayland)
  if (VIDEO_WAYLAND)
    pkg_check_modules(WAYLAND wayland-client wayland-scanner wayland-protocols wayland-egl wayland-cursor egl xkbcommon)

    # We have to generate some protocol interface code for some various Wayland features.
    if (WAYLAND_FOUND)
      execute_process(
        COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=pkgdatadir wayland-client
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        RESULT_VARIABLE WAYLAND_CORE_PROTOCOL_DIR_RC
        OUTPUT_VARIABLE WAYLAND_CORE_PROTOCOL_DIR
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if (NOT WAYLAND_CORE_PROTOCOL_DIR_RC EQUAL 0)
        set(WAYLAND_FOUND FALSE)
      endif ()
    endif ()

    if (WAYLAND_FOUND)
      execute_process(
        COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=pkgdatadir wayland-protocols
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        RESULT_VARIABLE WAYLAND_PROTOCOLS_DIR_RC
        OUTPUT_VARIABLE WAYLAND_PROTOCOLS_DIR
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if (NOT WAYLAND_PROTOCOLS_DIR_RC EQUAL 0)
        set(WAYLAND_FOUND FALSE)
      endif ()
    endif ()

    if (WAYLAND_FOUND)
      execute_process(
        COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=wayland_scanner wayland-scanner
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        RESULT_VARIABLE WAYLAND_SCANNER_RC
        OUTPUT_VARIABLE WAYLAND_SCANNER
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if (NOT WAYLAND_SCANNER_RC EQUAL 0)
        set(WAYLAND_FOUND FALSE)
      endif ()
    endif ()

    if (WAYLAND_FOUND)
      link_directories(
        ${WAYLAND_LIBRARY_DIRS}
      )
      include_directories(
        ${WAYLAND_INCLUDE_DIRS}
      )
      set(HAVE_VIDEO_WAYLAND TRUE)
      set(HAVE_SDL_VIDEO TRUE)

      file(GLOB WAYLAND_SOURCES ${SDL2_SOURCE_DIR}/src/video/wayland/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${WAYLAND_SOURCES})

      # We have to generate some protocol interface code for some unstable Wayland features.
      file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/wayland-generated-protocols")
      include_directories("${CMAKE_CURRENT_BINARY_DIR}/wayland-generated-protocols")

      WaylandProtocolGen("${WAYLAND_SCANNER}" "${WAYLAND_CORE_PROTOCOL_DIR}/wayland.xml" "wayland")

      foreach (_PROTL relative-pointer-unstable-v1 pointer-constraints-unstable-v1)
        string(REGEX REPLACE "\\-unstable\\-.*$" "" PROTSUBDIR ${_PROTL})
        WaylandProtocolGen("${WAYLAND_SCANNER}" "${WAYLAND_PROTOCOLS_DIR}/unstable/${PROTSUBDIR}/${_PROTL}.xml" "${_PROTL}")
      endforeach ()

      if (VIDEO_WAYLAND_QT_TOUCH)
        set(SDL_VIDEO_DRIVER_WAYLAND_QT_TOUCH 1)
      endif ()

      if (WAYLAND_SHARED)
        if (NOT HAVE_DLOPEN)
          message_warn("You must have SDL_LoadObject() support for dynamic Wayland loading")
        else ()
          FindLibraryAndSONAME(wayland-client)
          FindLibraryAndSONAME(wayland-egl)
          FindLibraryAndSONAME(wayland-cursor)
          FindLibraryAndSONAME(xkbcommon)
          set(SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC "\"${WAYLAND_CLIENT_LIB_SONAME}\"")
          set(SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_EGL "\"${WAYLAND_EGL_LIB_SONAME}\"")
          set(SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_CURSOR "\"${WAYLAND_CURSOR_LIB_SONAME}\"")
          set(SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_XKBCOMMON "\"${XKBCOMMON_LIB_SONAME}\"")
          set(HAVE_WAYLAND_SHARED TRUE)
        endif ()
      else ()
        set(EXTRA_LIBS ${WAYLAND_LIBRARIES} ${EXTRA_LIBS})
      endif ()

      set(SDL_VIDEO_DRIVER_WAYLAND 1)
    endif ()
  endif ()
endmacro()


################################################################################
# MACRO: re_add_package_group
################################################################################
macro(re_add_package_group name prefix)

endmacro()


################################################################################
# MACRO: re_add_package_group
################################################################################
macro(re_initialze_qt5)

  # Find includes in corresponding build directories
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)
  # Find the QtWidgets library
  find_package(Qt5Widgets)

endmacro()


macro(re_conditional_include module_name external_dep)
  set(upper_name "")
  string(TOUPPER ${module_name} upper_name)

  if (RE_WITH_${upper_name}_SUPPORT)
    re_add_feature(${module_name} ${external_dep} RE_WITH_${upper_name}_SUPPORT)
  else()
    message("The feature RE_WITH_${upper_name}_SUPPORT is not active")
  endif()
endmacro()