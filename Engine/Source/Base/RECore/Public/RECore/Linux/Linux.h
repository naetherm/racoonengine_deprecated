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
#define RE_WARNING_PUSH

/**
*  @brief
*    Restores the last warning state pushed onto the stack
*/
#define RE_WARNING_POP

/**
*  @brief
*    Disables a warning
*
*  @param[in] WarningID
*    ID of the warning to disable
*/
#define RE_WARNING_DISABLE(WarningID)


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <stddef.h>
#include <stdint.h>
#include <cinttypes>


//[-------------------------------------------------------]
//[ C++11 definitions                                     ]
//[-------------------------------------------------------]
// GCC: Have a look at http://gcc.gnu.org/projects/cxx0x.html see which C++11 features are supported

// "nullptr"-definition
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ < 3)
// There's a bug in GCC 4.5.x ... 4.5.2 causing compiler errors when using the nullptr-class solution from below:
	//   "Bug 45383 - [4.5 Regression] Implicit conversion to pointer does no longer automatically generate operator== and operator!=."
	//   (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45383)
	// ... looked a few minutes for a solution, without success... (just defining it as 0 is no solution and will introduce other compiler errors!)
	#error "Due to a bug in GCC 4.5.x ... 4.5.2 this GCC version is not supported. Please use a newer or older GCC version instead."
#elif ((__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)) && !defined(__clang__))
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

// "override"-definition, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#override (C++11 keyword is introduced in GCC 4.7 or clang 2.9)
#if ((__GNUC__ < 4 || __GNUC_MINOR__ < 7) && !defined(__clang__))
#define override
#endif


//[-------------------------------------------------------]
//[ Project independent generic export                    ]
//[-------------------------------------------------------]
// To export classes, methods and variables
#if defined(HAVE_VISIBILITY_ATTR)
#define RE_GENERIC_API_EXPORT __attribute__ ((visibility("default")))
#else
#define RE_GENERIC_API_EXPORT
#endif


//[-------------------------------------------------------]
//[ Project independent generic import                    ]
//[-------------------------------------------------------]
// To import classes, methods and variables
#define RE_GENERIC_API_IMPORT


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#if defined(HAVE_VISIBILITY_ATTR) && !defined(RECORE_STATIC)
// To import/export class templates
		#define RECORE_TMPL __attribute__ ((visibility("default")))

		// To export plugin functions
		#define RECORE_PLUGIN_API __attribute__ ((visibility("default")))
#else
// To import/export class templates
#define RECORE_TMPL

// To export plugin functions
#define RECORE_PLUGIN_API
#endif


// To export functions
#if defined(HAVE_VISIBILITY_ATTR)
#define GENERIC_FUNCTION_EXPORT __attribute__ ((visibility("default")))
#else
#define GENERIC_FUNCTION_EXPORT __attribute__((visibility("default")))

//extern "C"
#endif

/**
*  @brief
*    Force the compiler to inline something
*
*  @note
*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
*/
#define FORCEINLINE __attribute__((always_inline))

/**
*  @brief
*    Restrict alias
*/
#define RESTRICT __restrict__
#define RESTRICT_RETURN

/**
*  @brief
*    No operation macro ("_asm nop"/__nop())
*/
#define NOP asm ("nop");

/**
*  @brief
*    Debug break operation macro
*/
#define DEBUG_BREAK __builtin_trap()

#ifdef __clang__
/**
		*  @brief
		*    Platform specific "#pragma clang diagnostic push" (Clang)
		*/
		#define PRAGMA_WARNING_PUSH _Pragma("clang diagnostic push")

		/**
		*  @brief
		*    Platform specific "#pragma clang diagnostic pop" (Clang)
		*/
		#define PRAGMA_WARNING_POP _Pragma("clang diagnostic pop")

		/**
		*  @brief
		*    Platform specific "#pragma warning(disable: <x>)" (Microsoft Windows Visual Studio)
		*/
		#define PRAGMA_WARNING_DISABLE_MSVC(id)

		/**
		*  @brief
		*    Platform specific "#pragma GCC diagnostic ignored <x>" (GCC)
		*/
		#define PRAGMA_WARNING_DISABLE_GCC(id)

		/**
		*  @brief
		*    Platform specific "#pragma clang diagnostic ignored <x>" (Clang)
		*/
		// We need stringify because _Pragma expects an string literal
		#define PRAGMA_STRINGIFY(a) #a
		#define PRAGMA_WARNING_DISABLE_CLANG(id) _Pragma(PRAGMA_STRINGIFY(clang diagnostic ignored id) )
#elif __GNUC__
// gcc
/**
*  @brief
*    Platform specific "#pragma GCC diagnostic push" (GCC)
*/
#define PRAGMA_WARNING_PUSH _Pragma("GCC diagnostic push")

/**
*  @brief
*    Platform specific "#pragma warning(pop)" (GCC)
*/
#define PRAGMA_WARNING_POP _Pragma("GCC diagnostic pop")

/**
*  @brief
*    Platform specific "#pragma warning(disable: <x>)" (Microsoft Windows Visual Studio)
*/
#define PRAGMA_WARNING_DISABLE_MSVC(id)

/**
*  @brief
*    Platform specific "#pragma GCC diagnostic ignored <x>" (GCC)
*/
// We need stringify because _Pragma expects an string literal
#define PRAGMA_STRINGIFY(a) #a
#define PRAGMA_WARNING_DISABLE_GCC(id) _Pragma(PRAGMA_STRINGIFY(GCC diagnostic ignored id) )

/**
*  @brief
*    Platform specific "#pragma clang diagnostic ignored <x>" (Clang)
*/
#define PRAGMA_WARNING_DISABLE_CLANG(id)
#else
#error "Unsupported compiler"
#endif

#define stricmp(a, b)	strcasecmp(a, b)


#ifdef DEBUG
#include <cassert>
		#define ASSERT(expression, message) assert((expression) && message);	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere
#else
#define ASSERT(expression, message)	// TODO(naetherm) "RHI_ASSERT()" should be used everywhere
#endif
#include <inttypes.h>	// For uint32_t, uint64_t etc.
#include <type_traits>	// For "std::integral_constant"


//[-------------------------------------------------------]
//[ Data types                                            ]
//[-------------------------------------------------------]
// Standard types
namespace RECore {


typedef std::int8_t int8;
typedef std::uint8_t uint8;

typedef std::int16_t int16;
typedef std::uint16_t uint16;

typedef std::int32_t int32;
typedef std::uint32_t uint32;

typedef std::int64_t int64;
typedef std::uint64_t uint64;

#if X64_ARCHITECTURE
typedef std::uint64_t uint_ptr;
typedef std::uint64_t handle;
typedef std::uint64_t sizeT;
#else
typedef std::uint32_t 	uint_ptr;
typedef std::uint32_t 	handle;
typedef std::uint32_t   sizeT;
#endif

}

/**
*  @brief
*    Invalid handle value (for data type RECore::handle)
*/
static const RECore::handle NULL_HANDLE    = 0;
static const RECore::handle INVALID_HANDLE = -1;