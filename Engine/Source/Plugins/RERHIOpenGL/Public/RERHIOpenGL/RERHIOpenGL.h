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
#include <RECore/RECore.h>
#include <RERHI/RERHI.h>
#include <RERHI/Rhi.h>

#include <RECore/Log/Log.h>
#include <RECore/Tools/MakeId.h>

#ifdef RHI_OPENGL_GLSLTOSPIRV
// Disable warnings in external headers, we can't fix them
	PRAGMA_WARNING_PUSH
		PRAGMA_WARNING_DISABLE_MSVC(4061)	// warning C4061: enumerator '<x>' in switch of enum '<y>' is not explicitly handled by a case label
		PRAGMA_WARNING_DISABLE_MSVC(4100)	// warning C4100: 's': unreferenced formal parameter
		PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from '<x>' to '<y>', signed/unsigned mismatch
		PRAGMA_WARNING_DISABLE_MSVC(4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
		PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
		PRAGMA_WARNING_DISABLE_MSVC(4623)	// warning C4623: 'std::_List_node<_Ty,std::_Default_allocator_traits<_Alloc>::void_pointer>': default constructor was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: '<x>': copy constructor was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4774)	// warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
		PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
		//#include <SPIRV/GlslangToSpv.h>
    #include <spirv/1.2/GLSL.std.450.h>
    #include <spirv/1.2/spirv.hpp>
		#include <glslang/MachineIndependent/localintermediate.h>
    #include <glslang/SPIRV/GlslangToSpv.h>
	PRAGMA_WARNING_POP
#endif

#include <smolv.h>

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

	// Disable warnings in external headers, we can't fix them
	PRAGMA_WARNING_PUSH
		PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '<x>' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
		PRAGMA_WARNING_DISABLE_MSVC(5039)	// warning C5039: 'TpSetCallbackCleanupGroup': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
		#undef GL_GLEXT_PROTOTYPES
		//#include <GL/gl.h>
		//#include <GL/glext.h>	// Requires definitions from "gl.h"
		//#include <GL/wglext.h>	// Requires definitions from "gl.h"
	PRAGMA_WARNING_POP

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
	//#include <Windows.h>

	// Get rid of some nasty OS macros
	#undef max
#elif LINUX
// Get rid of some nasty OS macros
#undef None	// Linux: Undefine "None", this name is used inside enums defined by RacoonEngine (which gets defined inside "Xlib.h" pulled in by "glx.h")

#define CALLBACK

//
#include "GL/glx.h"
#include "GL/glxext.h"
#include "GL/gl.h"
#include "GL/glext.h"	// Requires definitions from "gl.h"

// TODO(naetherm) Review which of the following headers can be removed
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <dlfcn.h>
#include <link.h>
#include <iostream>	// TODO(naetherm) Can this include be removed?

#include <climits> // Required for UINT_MAX

// Need to redefine "None"-macro (which got undefined in "Extensions.h" due name clashes used in enums)
#ifndef None
#define None 0L	///< Universal null resource or null atom
#endif
#else
#error "Unsupported platform"
#endif

#ifdef DEBUG
#include <cstdio>  // For printf(). Remove if you don't need the PrintRanges() function (mostly for debugging anyway).
#endif

#include <cstdint>	// RECore::uint32
#include <limits>	// std::numeric_limits<type>::max()
#include <cstdlib>
#include <cstring>

#if defined(LINUX)

#ifdef None
// ... we undef these to avoid name conflicts...
namespace OSWindowLinuxInclude {
enum {
  X11_None = None,
  X11_Always = Always,
  X11_Above = Above,
  X11_Success = Success
};
}

#undef None
#undef Always
#undef Above
#undef Success

namespace XLib {
enum {
  None = OSWindowLinuxInclude::X11_None,
  Always = OSWindowLinuxInclude::X11_Always,
  Above = OSWindowLinuxInclude::X11_Above,
  Success = OSWindowLinuxInclude::X11_Success
};
}
// ... now that the namespace is name conflict free, go on with includes...
#endif
#endif

//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace {
namespace detail {


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static constexpr const char *GLSL_NAME = "GLSL";
}
}


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef RERHIOPENGL_EXPORTS
// To export classes, methods and variables
#define RERHIOPENGL_API			RE_GENERIC_API_EXPORT
#else
// To import classes, methods and variables
#define RERHIOPENGL_API			RE_GENERIC_API_IMPORT
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
		RHI_ASSERT(&rhiReference == &(resourceReference).getRhi(), "OpenGL error: The given resource is owned by another RHI instance")

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] debugName
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER , [[maybe_unused]] const char debugName[] = ""
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
#endif



#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/OpenGLRuntimeLinking.h"