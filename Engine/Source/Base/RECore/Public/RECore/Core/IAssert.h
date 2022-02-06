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
#include "RECore/RECore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract assert interface
*
*  @note
*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
*/
class IAssert
{

  // Public virtual RECore::IAssert methods
public:
  /**
  *  @brief
  *    Handle assert
  *
  *  @param[in] expression
  *    Expression as ASCII string
  *  @param[in] file
  *    File as ASCII string
  *  @param[in] line
  *    Line number
  *  @param[in] format
  *    "snprintf"-style formatted UTF-8 assert message
  *
  *  @return
  *    "true" to request debug break, else "false"
  */
  [[nodiscard]] virtual bool handleAssert(const char* expression, const char* file, uint32_t line, const char* format, ...) = 0;

  // Protected methods
protected:
  inline IAssert()
  {}

  inline virtual ~IAssert()
  {}

  explicit IAssert(const IAssert&) = delete;
  IAssert& operator=(const IAssert&) = delete;

};

// Macros & definitions
/**
*  @brief
*    Ease-of-use assert macro, potential issues checked by asserts shouldn't happen in shipped builds and hence are optimized out there
*
*  @param[in] context
*    RHI context to ask for the assert interface
*  @param[in] expression
*    Expression which must be true, else the assert triggers
*  @param[in] format
*    "snprintf"-style formatted UTF-8 assert message
*
*  @note
*    - Example: RHI_ASSERT(mContext, isInitialized, "Direct3D 11 RHI implementation assert failed")
*    - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
*/
#ifdef DEBUG
#define RHI_ASSERT(expression, format, ...) \
			do \
			{ \
				if (!(expression) && DefaultAssert::handleAssert(#expression, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__)) \
				{ \
					DEBUG_BREAK; \
				} \
			} while (0);
		#define RHI_ASSERT_ONCE(expression, format, ...) \
			do \
			{ \
				static bool loggedOnce = false; \
				if (!loggedOnce && !(expression)) \
				{ \
					loggedOnce = true; \
					if (DefaultAssert::handleAssert(#expression, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__)) \
					{ \
						DEBUG_BREAK; \
					} \
				} \
			} while (0);
#else
#define RHI_ASSERT(expression, format, ...)
#define RHI_ASSERT_ONCE(expression, format, ...)
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore