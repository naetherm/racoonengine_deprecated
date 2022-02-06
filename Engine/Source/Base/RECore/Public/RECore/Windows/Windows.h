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
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Backups the current warning state for all warnings
*/
#ifdef __MINGW32__
	#define RE_WARNING_PUSH
#else
	#define RE_WARNING_PUSH __pragma(warning(push))
#endif

/**
*  @brief
*    Restores the last warning state pushed onto the stack
*/
#ifdef __MINGW32__
	#define RE_WARNING_POP
#else
	#define RE_WARNING_POP __pragma(warning(pop))
#endif

/**
*  @brief
*    Disables a warning
*
*  @param[in] WarningID
*    ID of the warning to disable
*/
#ifdef __MINGW32__
	#define RE_WARNING_DISABLE(WarningID)
#else
	#define RE_WARNING_DISABLE(WarningID) __pragma(warning(disable: WarningID))
#endif


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
#ifdef __MINGW32__
	#include <_mingw.h>	// For "__int8", "__int16" and so so definitions
	// Within "_mingw.h", "__int32" is defined as "long" while RacoonEngine assumes "int", so, we change the definition
	#define __int32 int
	#include <stddef.h>	// For "size_t"
#else
	// Within Microsoft Visual Studio, 'wchar_t' can be defined as native type, in this case we don't need a special include
	#if !defined(_MSC_VER) || !defined(_WCHAR_T_DEFINED)
		#include <wctype.h>
	#endif
#endif


//[-------------------------------------------------------]
//[ C++11 definitions                                     ]
//[-------------------------------------------------------]
// Microsoft Visual Studio 2010: Have a look at http://blogs.msdn.com/b/vcblog/archive/2010/04/06/c-0x-core-language-features-in-vc10-the-table.aspx see which C++11 features are supported

// "nullptr"-definition
#if defined(__INTEL_COMPILER) || !defined(_MSC_VER) || _MSC_VER < 1600	// The Intel C++ compiler has no support for nullptr, 1600 = Microsoft Visual Studio 2010
	/**
	*  @brief
	*    nullptr definition for compilers don't supporting this C++11 feature
	*
	*  @note
	*    - The implementation comes from the "A name for the null pointer: nullptr"-document (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf)
	*/
	const class {
		public:
			template<class T> operator T*() const  { return 0; }
			template<class C, class T> operator T C::*() const  { return 0; }
		private:
			void operator&() const;
	} nullptr = {};
#endif


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
RE_WARNING_DISABLE(4100) // "'identifier' : unreferenced formal parameter"
RE_WARNING_DISABLE(4201) // "nonstandard extension used : nameless struct/union"
RE_WARNING_DISABLE(4231) // "nonstandard extension used : 'identifier' before template explicit instantiation" -> It's part of C++11 (see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates)
RE_WARNING_DISABLE(4481) // "nonstandard extension used: override specifier 'override'" -> It's part of C++11 (see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#override)


// To export functions
#define GENERIC_FUNCTION_EXPORT extern "C" __declspec(dllexport)

/**
*  @brief
*    Force the compiler to inline something
*
*  @note
*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
*/
#define FORCEINLINE __forceinline

/**
*  @brief
*    Restrict alias
*/
#define RESTRICT __restrict
#define RESTRICT_RETURN __restrict

/**
*  @brief
*    No operation macro ("_asm nop"/"__nop()")
*/
#define NOP __nop()

/**
*  @brief
*    Debug break operation macro
*/
#define DEBUG_BREAK __debugbreak()

/**
*  @brief
*    Platform specific "#pragma warning(push)" (Microsoft Windows Visual Studio)
*/
#define PRAGMA_WARNING_PUSH __pragma(warning(push))

/**
*  @brief
*    Platform specific "#pragma warning(pop)" (Microsoft Windows Visual Studio)
*/
#define PRAGMA_WARNING_POP __pragma(warning(pop))

/**
*  @brief
*    Platform specific "#pragma warning(disable: <x>)" (Microsoft Windows Visual Studio)
*/
#define PRAGMA_WARNING_DISABLE_MSVC(id) __pragma(warning(disable: id))

/**
*  @brief
*    Platform specific "#pragma clang diagnostic ignored <x>" (Clang)
*/
#define PRAGMA_WARNING_DISABLE_CLANG(id)

/**
*  @brief
*    Platform specific "#pragma GCC diagnostic ignored <x>" (GCC)
*/
#define PRAGMA_WARNING_DISABLE_GCC(id)


// Disable warnings in external headers, we can't fix them
__pragma(warning(push))
__pragma(warning(disable: 4574)) // warning C4574: '_HAS_ITERATOR_DEBUGGING' is defined to be '0': did you mean to use '#if _HAS_ITERATOR_DEBUGGING'?
__pragma(warning(disable: 4668)) // warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#ifdef DEBUG
#include <cassert>
			#define ASSERT(expression, message) assert((expression) && message);	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere
#else
#define ASSERT(expression, message)	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere
#endif
#include <inttypes.h>	// For uint32_t, uint64_t etc.
#include <type_traits>	// For "std::integral_constant"
__pragma(warning(pop))


//[-------------------------------------------------------]
//[ Project independent generic export                    ]
//[-------------------------------------------------------]
// To export classes, methods and variables
#define RE_GENERIC_API_EXPORT	__declspec(dllexport)


//[-------------------------------------------------------]
//[ Project independent generic import                    ]
//[-------------------------------------------------------]
// To import classes, methods and variables
#define RE_GENERIC_API_IMPORT	__declspec(dllimport)


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
// To export/import class templates
#define RECORE_TMPL

// To export plugin functions
#ifdef RECORE_STATIC
	#define RECORE_PLUGIN_API
#else
	#define RECORE_PLUGIN_API __declspec(dllexport)
#endif


//[-------------------------------------------------------]
//[ Data types                                            ]
//[-------------------------------------------------------]
namespace RECore {


	// Standard types
	typedef __int8					int8;
	typedef unsigned __int8			uint8;

	typedef __int16					int16;
	typedef unsigned __int16		uint16;

	typedef __int32					int32;
	typedef unsigned __int32		uint32;

	typedef __int64					int64;
	typedef unsigned __int64		uint64;

	#if WIN64
		typedef unsigned __int64	uint_ptr;
		typedef unsigned __int64	handle;
	#else
		typedef unsigned __int32	uint_ptr;
		typedef unsigned __int32	handle;
	#endif


}

/**
*  @brief
*    Invalid handle value (for data type RECore::handle)
*/
#ifdef WIN64
	static const RECore::handle NULL_HANDLE    = 0x0000000000000000;
	static const RECore::handle INVALID_HANDLE = 0xffffffffffffffff;
#else
	static const RECore::handle NULL_HANDLE    = 0x00000000;
	static const RECore::handle INVALID_HANDLE = 0xffffffff;
#endif


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
RE_WARNING_DISABLE(4355) // 'this' : used in base member initializer list"