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
#include <limits>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract profiler interface
*/
class IProfiler
{


  //[-------------------------------------------------------]
  //[ Public virtual RERenderer::IProfiler methods            ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Begin profiler CPU sample section
  *
  *  @param[in] name
  *    Section name
  *  @param[in] hashCache
  *    Hash cache, can be a null pointer (less efficient)
  */
  virtual void beginCpuSample(const char* name, uint32* hashCache) = 0;

  /**
  *  @brief
  *    End profiler CPU sample section
  */
  virtual void endCpuSample() = 0;

  /**
  *  @brief
  *    Begin profiler GPU sample section
  *
  *  @param[in] name
  *    Section name
  *  @param[in] hashCache
  *    Hash cache, can be a null pointer (less efficient)
  */
  virtual void beginGpuSample(const char* name, uint32* hashCache) = 0;

  /**
  *  @brief
  *    End profiler GPU sample section
  */
  virtual void endGpuSample() = 0;


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  inline IProfiler()
  {
    // Nothing here
  }

  inline virtual ~IProfiler()
  {
    // Nothing here
  }

  explicit IProfiler(const IProfiler&) = delete;
  IProfiler& operator=(const IProfiler&) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore