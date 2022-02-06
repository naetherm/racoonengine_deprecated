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
*    Abstract log interface
*
*  @note
*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
*/
class ILog
{

  // Public definitions
public:
  /**
  *  @brief
  *    Log message type
  */
  enum class Type
  {
    TRACE,					///< Trace, also known as verbose logging
    DEBUG,					///< Debug
    INFORMATION,			///< Information
    WARNING,				///< General warning
    PERFORMANCE_WARNING,	///< Performance related warning
    COMPATIBILITY_WARNING,	///< Compatibility related warning
    CRITICAL				///< Critical
  };

  // Public virtual RECore::ILog methods
public:
  /**
  *  @brief
  *    Print a formatted log message
  *
  *  @param[in] type
  *    Log message type
  *  @param[in] attachment
  *    Optional attachment (for example build shader source code), can be a null pointer
  *  @param[in] file
  *    File as ASCII string
  *  @param[in] line
  *    Line number
  *  @param[in] format
  *    "snprintf"-style formatted UTF-8 log message
  *
  *  @return
  *    "true" to request debug break, else "false"
  */
  [[nodiscard]] virtual bool print(Type type, const char* attachment, const char* file, uint32_t line, const char* format, ...) = 0;

  // Protected methods
protected:
  inline ILog()
  {}

  inline virtual ~ILog()
  {}

  explicit ILog(const ILog&) = delete;
  ILog& operator=(const ILog&) = delete;

};

// Macros & definitions
/**
*  @brief
*    Ease-of-use log macro
*
*  @param[in] context
*    RHI context to ask for the log interface
*  @param[in] type
*    Log message type
*  @param[in] format
*    "snprintf"-style formatted UTF-8 log message
*
*  @note
*    - Example: RHI_LOG(mContext, DEBUG, "Direct3D 11 RHI implementation startup")
*    - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
*/
#define RHI_LOG(context, type, format, ...) \
		do \
		{ \
			if ((context).getLog().print(RECore::ILog::Type::type, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__)) \
			{ \
				DEBUG_BREAK; \
			} \
		} while (0);
#define RHI_LOG_ONCE(context, type, format, ...) \
		do \
		{ \
			static bool loggedOnce = false; \
			if (!loggedOnce) \
			{ \
				loggedOnce = true; \
				if ((context).getLog().print(RECore::ILog::Type::type, nullptr, __FILE__, static_cast<uint32_t>(__LINE__), format, ##__VA_ARGS__)) \
				{ \
					DEBUG_BREAK; \
				} \
			} \
		} while (0);



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore