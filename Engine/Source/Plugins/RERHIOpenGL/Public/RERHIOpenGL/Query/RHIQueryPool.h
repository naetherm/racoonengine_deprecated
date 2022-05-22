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
#include "RERHIOpenGL/RERHIOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OpenGL asynchronous query pool base class
*/
class QueryPool : public RERHI::RHIQueryPool
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] queryType
  *    Query type
  *  @param[in] numberOfQueries
  *    Number of queries
  */
  QueryPool(RHIDynamicRHI& openGLRhi, RERHI::QueryType queryType, RECore::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~QueryPool() override = default;

  /**
  *  @brief
  *    Return the query type
  *
  *  @return
  *    The query type
  */
  [[nodiscard]] inline RERHI::QueryType getQueryType() const
  {
    return mQueryType;
  }

  /**
  *  @brief
  *    Return the number of queries
  *
  *  @return
  *    The number of queries
  */
  [[nodiscard]] inline RECore::uint32 getNumberOfQueries() const
  {
    return mNumberOfQueries;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit QueryPool(const QueryPool& source) = delete;
  QueryPool& operator =(const QueryPool& source) = delete;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  RERHI::QueryType mQueryType;
  RECore::uint32	   mNumberOfQueries;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
