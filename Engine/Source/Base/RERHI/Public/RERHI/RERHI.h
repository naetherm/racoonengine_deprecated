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
// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'return': conversion from 'int' to 'std::char_traits<wchar_t>::int_type', signed/unsigned mismatch
PRAGMA_WARNING_DISABLE_MSVC(4574)	// warning C4574: '_HAS_ITERATOR_DEBUGGING' is defined to be '0': did you mean to use '#if _HAS_ITERATOR_DEBUGGING'?
PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::codecvt_base': copy constructor was implicitly defined as deleted
PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
PRAGMA_WARNING_DISABLE_MSVC(4987)	// warning C4987: nonstandard extension used: 'throw (...)'
#include <new>			// For placement new
#include <cmath>
#include <string.h>		// For "strcpy()"
#include <inttypes.h>	// For RECore::uint32, uint64_t etc.
PRAGMA_WARNING_POP
#ifdef _WIN32
#include <intrin.h>	// For "__nop()"
#endif
#ifdef LINUX
// Copied from "Xlib.h"
struct _XDisplay;

// Copied from "wayland-client.h"
struct wl_display;
struct wl_surface;
#endif


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef RERHI_EXPORTS
// To export classes, methods and variables
	#define RERHI_API			RE_GENERIC_API_EXPORT
#else
// To import classes, methods and variables
#define RERHI_API			RE_GENERIC_API_IMPORT
#endif


#ifdef DEBUG
#include <cassert>
	#define ASSERT(expression, message) assert((expression) && message);	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere

	/**
	*  @brief
	*    Resource name for debugging purposes, ignored when not using "DEBUG"
	*
	*  @param[in] debugName
	*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
	*/
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER , const char debugName[] = ""
	#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT , const char debugName[]

	/**
	*  @brief
	*    Pass resource name for debugging purposes, ignored when not using "DEBUG"
	*/
	#define RHI_RESOURCE_DEBUG_PASS_PARAMETER , debugName
#else
#define ASSERT(expression, message)	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere

/**
*  @brief
*    Resource name for debugging purposes, ignored when not using "DEBUG"
*
*  @param[in] debugName
*    ASCII name for debugging purposes, must be valid (there's no internal null pointer test)
*/
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER
#define RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT

/**
*  @brief
*    Pass resource name for debugging purposes, ignored when not using "DEBUG"
*/
#define RHI_RESOURCE_DEBUG_PASS_PARAMETER
#endif
#ifdef RHI_STATISTICS
// Disable warnings in external headers, we can't fix them
	PRAGMA_WARNING_PUSH
		PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'return': conversion from 'int' to 'std::char_traits<wchar_t>::int_type', signed/unsigned mismatch
		PRAGMA_WARNING_DISABLE_MSVC(4623)	// warning C4623: 'std::_List_node<_Ty,std::_Default_allocator_traits<_Alloc>::void_pointer>': default constructor was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::_Ptr_base<_Ty>': copy constructor was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::_Compressed_pair<glslang::pool_allocator<char>,std::_String_val<std::_Simple_types<_Ty>>,false>': assignment operator was implicitly defined as deleted
		PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
		PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::atomic_flag': move constructor was implicitly defined as deleted (compiling source file E:\private\racoonengine\Source\Rhi\Private\Direct3D10Rhi\Direct3D10Rhi.cpp)
		PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Compressed_pair<glslang::pool_allocator<char>,std::_String_val<std::_Simple_types<_Ty>>,false>': move assignment operator was implicitly defined as deleted
		#include <atomic>	// For "std::atomic<>"
	PRAGMA_WARNING_POP
#endif