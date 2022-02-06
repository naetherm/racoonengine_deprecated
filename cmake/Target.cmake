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


function(pl_include_cmake_file_list file)
  #message(${file})
  include(${file})

  get_filename_component(file_path "${file}" PATH)
  if (file_path)
    list(TRANSFORM FILES PREPEND ${file_path}/)
  endif()

  foreach(f ${FILES})
    get_filename_component(absolute_path ${f} ABSOLUTE)
    if (NOT EXISTS ${absolute_path})
      message(SEND_ERROR "The file ${absolute_path} referenced in ${file} does not exist!")
    endif()
  endforeach()
  # Append the _files.cmake file to the list of files so that it is shown in the IDE
  list(APPEND FILES ${file})

  set(PL_CURRENT_SOURCES ${PL_CURRENT_SOURCES} ${FILES} PARENT_SCOPE)
endfunction()

function(pl_add_target)
  unset(PL_CURRENT_SOURCES)

  # Arguments
  set(flags STATIC SHARED MODULE PLUGIN_STATIC PLUGIN_MODULE HEADERONLY EXECUTABLE APPLICATION IMPORTED AUTOMOC AUTOUIC AUTORCC NO_UNITY REFLECT)
  set(oneValueArgs NAME NAMESPACE OUTPUT_SUBDIRECTORY OUTPUT_NAME)
  set(multiValueArgs FILES_CMAKE CONDITIONAL_INCLUDES GENERATED_FILES INCLUDE_DIRECTORIES COMPILE_DEFINITIONS BUILD_DEPENDENCIES RUNTIME_DEPENDENCIES EXTERNAL_DEPENDENCIES PLATFORM_INCLUDE_FILES TARGET_PROPERTIES AUTOGEN_RULES)

  # Parse arguments
  cmake_parse_arguments(pl_add_target "${flags}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  #
  # Before doing anything else, let's first check whether this library should be reflected
  #
  if (pl_add_target_REFLECT)
    # (0) set variables
    set(RTTI_PROCESS_DIRECTORIES "'''${CMAKE_CURRENT_SOURCE_DIR}/Public'''")
    set(RTTI_IGNORED_DIRECTORIES "'''${CMAKE_CURRENT_SOURCE_DIR}/Generated'''")
    set(RTTI_OUTPUT_DIRECTORY "'''${CMAKE_CURRENT_SOURCE_DIR}/Generated'''")
    # TODO(naetherm): Add all dependencies too
    set(RTTI_INCLUDED_DIRECTORIES "'''${CMAKE_CURRENT_SOURCE_DIR}/Public'''")

    # (1) Copy and Configure the ReflectionGeneration.toml file
    configure_file(
      "${PL_ROOT_DIR}/cmake/Config/ReflectionGeneration.toml.in"
      "${CMAKE_CURRENT_SOURCE_DIR}/ReflectionGeneration.toml"
    )

    # (2) Add the directory "${CMAKE_CURRENT_SOURCE_DIR}/Generated" to list of include directories

    # (3) Generate the reflection information
  endif()
  #
  #
  #

  #message("\n\n\n STARTING ${pl_add_target_NAME}")
  # Configure project
  if (NOT pl_add_target_NAME)
    message(FATAL_ERROR "You must provide a name for the target!")
  endif()

  # If the PLUGIN_MODULE tag is passed in set the normal MODULE argument
  if (pl_add_target_PLUGIN_MODULE)
    set(pl_add_target_MODULE ${pl_add_target_PLUGIN_MODULE})
  endif()
  # If the PLUGIN_STATIC tag is passed in set the normal STATIC argument
  if (pl_add_target_PLUGIN_STATIC)
    set(pl_add_target_STATIC ${pl_add_target_PLUGIN_STATIC})
  endif()

  # Add conditional includes
  foreach(cond_include in ${pl_add_target_CONDITIONAL_INCLUDES})

  endforeach()

  # Fetch all source files
  #pl_collection_source_files()
  foreach(f ${pl_add_target_FILES_CMAKE})
    pl_include_cmake_file_list(${f})
  endforeach()

  # Configure
  # If the PLUGIN_MODULE tag is passed set the normal MODULE argument
  if(pl_add_target_PLUGIN_MODULE)
    set(pl_add_target_MODULE ${pl_add_target_PLUGIN_MODULE})
  endif()
  # If the PLUGIN_STATIC tag is passed mark the target as STATIC
  if(pl_add_target_PLUGIN_STATIC)
    set(pl_add_target_STATIC ${pl_add_target_PLUGIN_STATIC})
  endif()
  unset(linking_options)
  unset(linking_count)
  unset(target_type_options)
  if(pl_add_target_STATIC)
    set(linking_options STATIC)
    set(target_type_options STATIC)
    set(linking_count "${linking_count}1")
  endif()
  if(pl_add_target_SHARED)
    set(linking_options SHARED)
    set(target_type_options SHARED)
    set(linking_count "${linking_count}1")
  endif()
  if(pl_add_target_MODULE)
    set(linking_options ${PAL_LINKOPTION_MODULE})
    set(target_type_options ${PAL_LINKOPTION_MODULE})
    set(linking_count "${linking_count}1")
  endif()
  if(pl_add_target_HEADERONLY)
    set(linking_options INTERFACE)
    set(target_type_options INTERFACE)
    set(linking_count "${linking_count}1")
  endif()
  if(pl_add_target_EXECUTABLE)
    set(linking_options EXECUTABLE)
    set(linking_count "${linking_count}1")
  endif()
  if(pl_add_target_APPLICATION)
    set(linking_options APPLICATION)
    set(linking_count "${linking_count}1")
  endif()
  if(NOT ("${linking_count}" STREQUAL "1"))
    message(FATAL_ERROR "More than one of the following options [STATIC | SHARED | MODULE | HEADERONLY | EXECUTABLE | APPLICATION ] was specified and they are mutually exclusive")
  endif()
  if(pl_add_target_IMPORTED)
    list(APPEND target_type_options IMPORTED GLOBAL)
  endif()

  if(pl_add_target_NAMESPACE)
    set(interface_name "${pl_add_target_NAMESPACE}::${pl_add_target_NAME}")
  else()
    set(interface_name "${pl_add_target_NAME}")
  endif()

  set(project_NAME ${pl_add_target_NAME})
  if(pl_add_target_EXECUTABLE)
    add_executable(${pl_add_target_NAME}
      ${target_type_options}
      ${PL_CURRENT_SOURCES} ${pl_add_target_GENERATED_FILES}
      )
    if(pl_add_target_IMPORTED)
      set_target_properties(${pl_add_target_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    endif()
  elseif(pl_add_target_APPLICATION)
    add_executable(${pl_add_target_NAME}
      ${target_type_options}
      ${PAL_EXECUTABLE_APPLICATION_FLAG}
      ${PL_CURRENT_SOURCES} ${pl_add_target_GENERATED_FILES}
      )
    if(pl_add_target_IMPORTED)
      set_target_properties(${pl_add_target_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    endif()
  elseif(pl_add_target_HEADERONLY)
    add_library(${pl_add_target_NAME}
      ${target_type_options}
      ${PL_CURRENT_SOURCES} ${pl_add_target_GENERATED_FILES}
      )
  else()
    add_library(${pl_add_target_NAME}
      ${target_type_options}
      ${PL_CURRENT_SOURCES} ${pl_add_target_GENERATED_FILES}
      )
  endif()

  if(${pl_add_target_EXECUTABLE} OR ${pl_add_target_APPLICATION})
    add_executable(${interface_name} ALIAS ${pl_add_target_NAME})
  else()
    add_library(${interface_name} ALIAS ${pl_add_target_NAME})
  endif()

  if(pl_add_target_PLUGIN_MODULE OR pl_add_target_PLUGIN_STATIC)
    set_target_properties(${pl_add_target_NAME} PROPERTIES PLUGIN_MODULE TRUE)
  endif()

  if (pl_add_target_INCLUDE_DIRECTORIES)
    target_include_directories(${pl_add_target_NAME}
      ${pl_add_target_INCLUDE_DIRECTORIES}
      )
  endif()

  # Add the build dependencies
  if (pl_add_target_BUILD_DEPENDENCIES)
    #message("pl_add_target_BUILD_DEPENDENCIES: ${pl_add_target_BUILD_DEPENDENCIES}")
    pl_search_for_external_dependencies("${pl_add_target_BUILD_DEPENDENCIES}")
    foreach (dep "${pl_add_target_BUILD_DEPENDENCIES}")
      unset(target_group)
      foreach(d ${dep})
        if (NOT d STREQUAL "PUBLIC" AND NOT d STREQUAL "PRIVATE")
          #message(INFO ${d})

          target_include_directories(${pl_add_target_NAME} ${target_group} ${${d}_INCLUDE_DIRS})
          target_include_directories(${pl_add_target_NAME} ${target_group} ${${d}_EXT_INCLUDE_DIRS})
          if (${d} MATCHES "Qt5*")
            target_link_libraries(${pl_add_target_NAME} ${target_group} ${d})
          else()
            #target_link_libraries(${pl_add_target_NAME} ${target_group} ${${d}_LIBRARIES})
            target_link_libraries(${pl_add_target_NAME} ${target_group} ${d})
            #message("INFO>> $<TARGET_PROPERTY:${d},INTERFACE_LINK_LIBRARIES>")
            #target_link_libraries(${pl_add_target_NAME} PUBLIC $<TARGET_PROPERTY:${d},INTERFACE_LINK_LIBRARIES>)
            target_include_directories(${pl_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},INTERFACE_INCLUDE_DIRECTORIES>)
            target_compile_definitions(${pl_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},INTERFACE_COMPILE_DEFINITIONS>)
            target_compile_options(${pl_add_target_NAME} ${target_group} $<TARGET_PROPERTY:${d},INTERFACE_COMPILE_OPTIONS>)
          endif()
          add_dependencies(${pl_add_target_NAME} ${d})
          # target_include_directories(${pl_add_target_NAME} ${PL_CURRENT_EXT_INCLUDE_DIRS})
        else()
          set(target_group ${d})
        endif()
      endforeach()
    endforeach()
    #target_link_libraries(
    #  ${pl_add_target_NAME}
    #  ${pl_add_target_BUILD_DEPENDENCIES})
  endif()

  # Add the runtime dependencies
  if (pl_add_target_RUNTIME_DEPENDENCIES)
    #message("pl_add_target_RUNTIME_DEPENDENCIES: ${pl_add_target_RUNTIME_DEPENDENCIES}")
    pl_search_for_external_dependencies(${pl_add_target_NAME} "${pl_add_target_RUNTIME_DEPENDENCIES}")
  endif()
  if (pl_add_target_EXTERNAL_DEPENDENCIES)

    pl_search_for_external_dependencies(${pl_add_target_NAME}  "${pl_add_target_EXTERNAL_DEPENDENCIES}")
  endif()

  pl_configure_target_platform_properties()

  if(pl_add_target_COMPILE_DEFINITIONS)
    target_compile_definitions(${pl_add_target_NAME}
      ${pl_add_target_COMPILE_DEFINITIONS}
      )
  endif()

  if(pl_add_target_TARGET_PROPERTIES)
    set_target_properties(${pl_add_target_NAME} PROPERTIES COMPILE_FLAGS
      ${pl_add_target_TARGET_PROPERTIES})
  endif()

  # Handle Qt MOC, RCC, UIC
  foreach(prop IN ITEMS AUTOMOC AUTORCC)
    if(${pl_add_target_${prop}})
      set_property(TARGET ${pl_add_target_NAME} PROPERTY ${prop} ON)
    endif()
  endforeach()
  if(${pl_add_target_AUTOUIC})
    get_target_property(all_ui_sources ${pl_add_target_NAME} SOURCES)
    list(FILTER all_ui_sources INCLUDE REGEX "^.*\\.ui$")
    if(NOT all_ui_sources)
      message(FATAL_ERROR "Target ${pl_add_target_NAME} contains AUTOUIC but doesnt have any .ui file")
    endif()
    ly_qt_uic_target(${pl_add_target_NAME})
  endif()

endfunction(pl_add_target)


macro(pl_configure_target_platform_properties)
  foreach(platform_include_file ${pl_add_target_PLATFORM_INCLUDE_FILES})
    set(PL_BUILD_DEPENDENCIES)
    set(PL_RUNTIME_DEPENDENCIES)

    #message("PL_BUILD_DEPENDENCIES: ${PL_BUILD_DEPENDENCIES}")
    #message("PL_RUNTIME_DEPENDENCIES: ${PL_RUNTIME_DEPENDENCIES}")
    #message("platform_include_file: ${platform_include_file}")
    include(${platform_include_file} RESULT_VARIABLE pl_platform_cmake_file)
    #message("pl_platform_cmake_file: ${pl_platform_cmake_file}")
    #message("PL_BUILD_DEPENDENCIES: ${PL_BUILD_DEPENDENCIES}")
    #message("PL_RUNTIME_DEPENDENCIES: ${PL_RUNTIME_DEPENDENCIES}")
    #message("pl_add_target_NAME: ${pl_add_target_NAME}")

    if(PL_BUILD_DEPENDENCIES)
      #message("PL_BUILD_DEPENDENCIES: ${PL_BUILD_DEPENDENCIES}")
      # ly_target_link_libraries
      foreach(item ${PL_BUILD_DEPENDENCIES})
        #message("PL_BUILD_DEPENDENCIES[item]: ${item}")
        target_link_libraries(${pl_add_target_NAME} PRIVATE ${item})
      endforeach()
    endif()
    if(PL_RUNTIME_DEPENDENCIES)
      #message("PL_RUNTIME_DEPENDENCIES: ${PL_RUNTIME_DEPENDENCIES}")
      foreach(item ${PL_RUNTIME_DEPENDENCIES})
        #target_link_libraries(${pl_add_target_NAME}  ${item})
      endforeach()
      # ly_add_dependencies
      #target_link_libraries(${pl_add_target_NAME} ${PL_RUNTIME_DEPENDENCIES})
      #add_dependencies(${pl_add_target_NAME} ${PL_RUNTIME_DEPENDENCIES})
    endif()

  endforeach()
endmacro()


macro(pl_search_for_external_dependencies TARGET)
  #message("pl_search_for_external_dependencies: ${ARGN}")
  foreach(dependency ${ARGN})
    string(REPLACE "::" ";" dependency_list ${dependency})
    list(GET dependency_list 0 dep_namespace)
    if (${dep_namespace} STREQUAL "External")
      list(GET dependency_list 1 asdasd)
      if (NOT TARGET ${dependency})
        list(GET dependency_list 1 pkg)
        #message("IS TARGET ${dependency}=>pkg:${pkg}")
        list(LENGTH dependency_list dependency_list_length)
        if (dependency_list_length GREATER 2)
          # Interface specifier
          #message("INFO: OTHER PACKAGE BRANCH")
          list(GET dependency_list 2 component)
          list(APPEND packages_with_components ${pkg})
          list(APPEND ${pkg}_components ${component})
        else()
          #if (NOT ${pkg}_FOUND)
          #message("FINDING PACKAGE ${pkg}")
            find_package(${pkg} REQUIRED MODULE)
          #endif()
          #add_dependencies(${TARGET} ${pkg})
          # TODO(naetherm): Root of all evil during the inclusion of external libraries
          #message("${pkg}_INCLUDE_DIR => ${${pkg}_INCLUDE_DIR}")
          #message("${pkg}_LIBRARY => ${${pkg}_LIBRARY}")
          target_include_directories(${TARGET} PUBLIC ${${pkg}_INCLUDE_DIR})
          target_link_libraries(${TARGET} PUBLIC ${${pkg}_LIBRARY})
          #target_include_directories(${TARGET} PUBLIC ${pkg})
          #target_link_libraries(${TARGET} ${target_group} $<TARGET_PROPERTY:External::${pkg},INTERFACE_LINK_LIBRARIES>)
          #target_include_directories(${TARGET} PUBLIC $<TARGET_PROPERTY:${pkg},INTERFACE_INCLUDE_DIRECTORIES>)
          #target_compile_definitions(${TARGET} ${target_group} $<TARGET_PROPERTY:${pkg},INTERFACE_COMPILE_DEFINITIONS>)
          #target_compile_options(${TARGET} ${target_group} $<TARGET_PROPERTY:${pkg},INTERFACE_COMPILE_OPTIONS>)
        endif()
      else()
        list(GET dependency_list 1 pkg)
        #message("ELSE OF NOT TARGET ${dependency}=>pkg:${pkg}")
        find_package(${pkg} REQUIRED MODULE)
        #message("${pkg}_INCLUDE_DIR => ${${pkg}_INCLUDE_DIR}")
        #target_include_directories(${TARGET} PUBLIC ${${pkg}_INCLUDE_DIR})
        #target_link_libraries(${TARGET} PUBLIC ${${pkg}_LIBRARY})
        target_link_libraries(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${pkg},INTERFACE_LINK_LIBRARIES>)
        target_include_directories(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${pkg},INTERFACE_INCLUDE_DIRECTORIES>)
        target_compile_definitions(${TARGET} ${target_group} $<TARGET_PROPERTY:External::${pkg},INTERFACE_COMPILE_DEFINITIONS>)
        target_compile_options(${TARGET} ${target_group} $<TARGET_PROPERTY:External::${pkg},INTERFACE_COMPILE_OPTIONS>)
        #message("ELSE OF NOT TARGET ${dependency}")
      endif()
    endif()
  endforeach()

  # Add externals with components
  foreach(dep IN LISTS packages_with_components)
    #message("INFO: ADDING ${dep} now")
    #message(">>>> INFO: ADDING ${${dep}_components}")
    find_package(${dep} REQUIRED MODULE COMPONENTS ${${dep}_components})

    #target_link_libraries(${TARGET} PUBLIC "External::${dep}::${${dep}_components}")
    target_link_libraries(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${dep}::${${dep}_components},INTERFACE_LINK_LIBRARIES>)
    target_include_directories(${TARGET} PUBLIC $<TARGET_PROPERTY:External::${dep}::${${dep}_components},INTERFACE_INCLUDE_DIRECTORIES>)

  endforeach()
endmacro()