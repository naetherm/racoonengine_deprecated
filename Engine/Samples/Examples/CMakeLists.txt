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
pl_add_target(
	NAME Examples EXECUTABLE
	NAMESPACE RE
	FILES_CMAKE
	${CMAKE_CURRENT_SOURCE_DIR}/Examples_files.cmake
	${CMAKE_CURRENT_SOURCE_DIR}/Examples_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
	PLATFORM_INCLUDE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Examples_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
	INCLUDE_DIRECTORIES
	PUBLIC
	${PL_CONFIG_FILE_LOCATION}
	${CMAKE_CURRENT_SOURCE_DIR}/Public
	${CMAKE_CURRENT_SOURCE_DIR}/Private
	${DBUS_INCLUDE_DIRS}
	BUILD_DEPENDENCIES
	PUBLIC
	RECore
	RERHI
	RERenderer
	RERendererToolkit
	REInput
	External::physfs
	External::openvr
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
	External::openvr
	COMPILE_DEFINITIONS
	PUBLIC
	${${PAL_PLATFORM_NAME_UPPERCASE}_COMPILE_DEFS}
	${PAL_PLATFORM_NAME_UPPERCASE}
	RENDERER
	RHI_OPENGL
	RHI_VULKAN
	EXAMPLES_MIMALLOC
	SHARED_LIBRARIES
	#RHI_OPENGL_GLSLTOSPIRV
	TARGET_PROPERTIES
	-fPIC
)