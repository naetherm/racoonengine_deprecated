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
#include "RERHI/RERHI.h"
#include "RERHI/Buffer/RHIBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract uniform buffer object (UBO, "constant buffer" in Direct3D terminology) interface; read only structured buffer
*
*  @remarks
*    General usage hint
*      - Maximum size:                 64KByte (or more)
*      - Memory access pattern:        Coherent access
*      - Memory storage:               Usually local memory which is considered to be faster than global memory
*    OpenGL - http://www.opengl.org/wiki/Uniform_Buffer_Object
*      - Core in version:              4.2
*      - Adopted into core in version: 3.1
*      - ARB extension:                GL_ARB_Uniform_Buffer_Object
*    Direct3D - "Shader Constants"-documentation - http://msdn.microsoft.com/en-us/library/windows/desktop/bb509581%28v=vs.85%29.aspx
*      - Direct3D version:             10 and 11
*      - Shader model:                 4
*
*  @note
*    - Structured performance tip: "Constant Buffers without Constant Pain" by Holger Gruen, posted Jan 14 2015 at 10:38AM - https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
*/
class RHIUniformBuffer : public RHIBuffer {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIUniformBuffer() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfUniformBuffers;
#endif
  }

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline explicit RHIUniformBuffer(RHIDynamicRHI &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIBuffer(ResourceType::UNIFORM_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedUniformBuffers;
        ++rhi.getStatistics().currentNumberOfUniformBuffers;
#endif
  }

  explicit RHIUniformBuffer(const RHIUniformBuffer &source) = delete;

  RHIUniformBuffer &operator=(const RHIUniformBuffer &source) = delete;

};

typedef RECore::SmartRefCount<RHIUniformBuffer> RHIUniformBufferPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
