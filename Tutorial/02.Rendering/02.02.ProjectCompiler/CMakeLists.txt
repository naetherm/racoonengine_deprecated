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
	NAME 02.02.ProjectCompiler EXECUTABLE
	NAMESPACE RE
	FILES_CMAKE
	${CMAKE_CURRENT_SOURCE_DIR}/ProjectCompiler_files.cmake
	PLATFORM_INCLUDE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ProjectCompiler_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
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
	RERendererToolkit
	#EXTERNAL_DEPENDENCIES
	#PUBLIC
	RUNTIME_DEPENDENCIES
	PUBLIC
	External::zlib
	External::rapidjson
	External::mojoshader
	External::crunch
	External::ies
	External::assimp
	External::meshoptimizer
	External::mikktspace
	External::physfs
	COMPILE_DEFINITIONS
	PUBLIC
	${${PAL_PLATFORM_NAME_UPPERCASE}_COMPILE_DEFS}
	${PAL_PLATFORM_NAME_UPPERCASE}
	#EXAMPLES_MIMALLOC
	RHI_OPENGL
	TARGET_PROPERTIES
	-fPIC
)


##################################################
## Project
##################################################
add_custom_command(TARGET 02.02.ProjectCompiler POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/../Example $<TARGET_FILE_DIR:02.02.ProjectCompiler>/../Example
)