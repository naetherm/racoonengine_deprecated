////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RERHI/Rhi.h>
#include <RECore/Log/Log.h>
#include <RECore/Tools/MakeId.h>

#define GL_GLES_PROTOTYPES 0
#include "GLES3/gl3.h"
#include "GLES3/gl2ext.h"
#include "GLES3/gl3ext.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(5039)	// warning C5039: 'TpSetCallbackCleanupGroup': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception
#include "EGL/egl.h"
#include "EGL/eglext.h"
PRAGMA_WARNING_POP

#ifdef _WIN32
// Set Windows version to Windows Vista (0x0600), we don't support Windows XP (0x0501)
	#ifdef WINVER
		#undef WINVER
	#endif
	#define WINVER			0x0600
	#ifdef _WIN32_WINNT
		#undef _WIN32_WINNT
	#endif
	#define _WIN32_WINNT	0x0600

	// Exclude some stuff from "windows.h" to speed up compilation a bit
	#define NOGDICAPMASKS
	#define NOMENUS
	#define NOICONS
	#define NOKEYSTATES
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define OEMRESOURCE
	#define NOATOM
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NOCRYPT
	#include <Windows.h>

	// Get rid of some nasty OS macros
	#undef max
#endif

#ifdef __ANDROID__
#include <android/native_window.h>	// For "ANativeWindow_setBuffersGeometry()"
#endif

#if defined LINUX || defined(__ANDROID__)
// Get rid of some nasty OS macros
#undef None	// Linux: Undefine "None", this name is used inside enums defined by RacoonEngine (which gets defined inside "Xlib.h" pulled in by "egl.h")

#include <dlfcn.h>	// For "dlopen()", "dlclose()" and so on
#include <link.h>	// For getting the path to the library (for the error message)
#include <climits>
#endif

#if defined(LINUX)
#include <RERHI/Linux/X11Context.h>
#endif


//[-------------------------------------------------------]
//[ Macros & definitions                                  ]
//[-------------------------------------------------------]
#ifdef DEBUG
/*
	*  @brief
	*    Check whether or not the given resource is owned by the given RHI
	*/
	#define RHI_MATCH_CHECK(rhiReference, resourceReference) \
		RHI_ASSERT(&rhiReference == &(resourceReference).getRhi(), "OpenGL ES 3 error: The given resource is owned by another RHI instance")

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] debugName
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER , [[maybe_unused]] const char debugName[] = ""
	#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT , [[maybe_unused]] const char debugName[]
#else
/*
*  @brief
*    Check whether or not the given resource is owned by the given RHI
*/
#define RHI_MATCH_CHECK(rhiReference, resourceReference)

/**
*  @brief
*    Resource name for debugging purposes, ignored when not using "DEBUG"
*
*  @param[in] debugName
*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
*/
#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER
#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER_NO_DEFAULT
#endif
//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
#ifndef GLchar
#define GLchar char
#endif
#ifndef GLenum
#define GLenum GLuint
#endif


#include "RERHIOpenGLES3/OpenGLES3RuntimeLinking.h"
#include "RERHIOpenGLES3/OpenGLES3ContextRuntimeLinking.h"