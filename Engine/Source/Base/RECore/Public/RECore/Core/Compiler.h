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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//
// Determine the compiler version and type
//
// #define RE_COMPILER_RVCT 0
// #define RE_COMPILER_ARM 0
// #define RE_COMPILER_CLANG 0
// #define RE_COMPILER_GNUC 0
// #define RE_COMPILER_MSVC 0

// Inspired by EASTL

#if defined(__ARMCC_VERSION)

#define RE_COMPILER_RVCT    1
#define RE_COMPILER_ARM     1
#define RE_COMPILER_VERSION __ARMCC_VERSION
#define RE_COMPILER_NAME    "RVCT"

#elif defined(__clang__)

#define RE_COMPILER_CLANG 1
#define RE_COMPILER_NAME "clang"
#define RE_COMPILER_VERSION (__clang_major __ * 100 + __clang_minor__)
#define RE_COMPILER_STRING

#elif defined(__GNUC__)

#define RE_COMPILER_GNUC 1
#define RE_COMPILER_NAME "GCC"
#define RE_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#define RE_COMPILER_STRING

#elif defined(__BORLANDC__)

#define RE_COMPILER_BORLANDC 1
#define RE_COMPILER_NAME "Borland C"
#define RE_COMPILER_VERSION __BORNLANDC__
#define RE_COMPILER_STRING

#elif defined(__ICL) || defined(_ICC)


#elif defined(_MSC_VER)

#define RE_COMPILER_MSVC 1
#define RE_COMPILER_Name "MS Visual C++"
#define RE_COMPILER_VERSION _MSC_VER

// Additionally we have to distinguish between the different versions

#if (_MSC_VER <= 1200) // VS6.x
#define RE_COMPILER_MSVC6 1

#elif (_MSC_VER <= 1300) // VS7.x
#define RE_COMPILER_MSVC7 1

#elif (_MSC_VER < 1400) // VS2003
#define RE_COMPILER_MSVC7_1 1

#elif (_MSC_VER < 1500) // VS2005
#define RE_COMPILER_MSVC8 1

#elif (_MSC_VER < 1600) // VS2008
#define RE_COMPILER_MSVC9 1

#elif (_MSC_VER < 1700) // VS2010
#define RE_COMPILER_MSVC10 1

#elif (_MSC_VER < 1800) // VS2011/12
#define RE_COMPILER_MSVC11 1
#define RE_COMPILER_MSVC12 1

#elif (_MSC_VER < 1900) // VS2013
#define RE_COMPILER_MSVC13 1

#elif (_MSC_VER < 2000) // VS2015
#define RE_COMPILER_MSVC14 1
#else
#endif

#else

#endif


#ifndef RE_COMPILER_HAS_FEATURE
#if defined(__clang__)
#define RE_COMPILER_HAS_FEATURE(x) __has_feature(x)
#else
#define RE_COMPILER_HAS_FEATURE(x) 0
#endif
#endif


#ifndef RE_COMPILER_HAS_BUILTIN
#if defined(__clang__)
#define RE_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
#else
#define RE_COMPILER_HAS_BUILTIN(x) 0
#endif
#endif


#ifndef RE_UNIT_ALIGN_OF
#if   defined(_MSC_VER) && (_MSC_VER < 1700)
// Workaround for this VS 2010 compiler bug: https://connect.microsoft.com/VisualStudio/feedback/details/682695
#define RE_UNIT_ALIGN_OF(...) ( (sizeof(__VA_ARGS__)*0) + (__alignof(__VA_ARGS__)) )
#elif !defined(__GNUC__) || (__GNUC__ >= 3) // GCC 2.x doesn't do __alignof correctly all the time.
#define RE_UNIT_ALIGN_OF __alignof
#else
#define RE_UNIT_ALIGN_OF(type) ((size_t)offsetof(struct{ char c; type m; }, m))
#endif
#endif


#if   defined(__GNUC__) && (__GNUC__ < 3)
#define RE_ALIGN_OF(type) ((size_t)__alignof__(type))
#define RE_ALIGN(n)
#define RE_PREFIX_ALIGN(n)
#define RE_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
#define RE_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
#define RE_PACKED __attribute__((packed))
#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__xlC__) || defined(__clang__)
#define RE_ALIGN_OF(type) ((std::size_t)__alignof__(type))
#define RE_ALIGN(n) __attribute__((aligned(n)))
#define RE_PREFIX_ALIGN(n)
#define RE_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
#define RE_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
#define RE_PACKED __attribute__((packed))
#elif (defined(RE_COMPILER_MSVC))
#define RE_ALIGN_OF(type) ((size_t)__alignof(type))
#define RE_ALIGN(n) __declspec(align(n))
#define RE_PREFIX_ALIGN(n) RE_ALIGN(n)
#define RE_POSTFIX_ALIGN(n)
#define RE_ALIGNED(variable_type, variable, n) RE_ALIGN(n) variable_name variable
#define RE_PACKED
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore