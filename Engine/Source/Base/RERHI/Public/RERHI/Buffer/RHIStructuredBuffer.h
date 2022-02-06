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
*    Abstract structured buffer object (SBO) interface; array of structured data
*
*  @remarks
*    General usage hint
*      - Maximum size:                 128MByte (or more)
*      - Memory access pattern:        Random access
*      - Memory storage:               Global texture memory which is considered to be slower than local memory
*    OpenGL - Shader Storage Buffer Object (SSBO) - https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
*      - Core in version:              4.6
*      - Adopted into core in version: 4.3
*      - ARB extension:                GL_ARB_shader_storage_buffer_object
*    Direct3D - "New Resource Types"-documentation - https://docs.microsoft.com/en-gb/windows/desktop/direct3d11/direct3d-11-advanced-stages-cs-resources
*      - Direct3D version:             11
*      - Shader model:                 5
*
*  @note
*    - There's no support for Direct3D 11 style "append" or "counter" structured buffer by intent since neither OpenGL nor Vulkan support something similar, use atomic shader intrinsics instead
*    - Structured performance tip: "Structured buffer, 128-bit stride, coherent access patterns, and under 64 KB of data = Use uniform buffer instead" - "How About Constant Buffers?" by Evan Hart, posted May 04 2015 at 11:34AM - https://developer.nvidia.com/content/how-about-constant-buffers
*    - Structured performance tip: "Use 128-bit stride" -- "Understanding Structured Buffer Performance" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance
*    - Structured performance tip: "Redundancy and Latency in Structured Buffer Use" by Evan Hart, posted Apr 30 2015 at 06:15PM - https://developer.nvidia.com/content/redundancy-and-latency-structured-buffer-use
*/
class RHIStructuredBuffer : public RHIBuffer {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIStructuredBuffer() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfStructuredBuffers;
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
  inline explicit RHIStructuredBuffer(RHIDynamicRHI &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIBuffer(ResourceType::STRUCTURED_BUFFER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedStructuredBuffers;
        ++rhi.getStatistics().currentNumberOfStructuredBuffers;
#endif
  }

  explicit RHIStructuredBuffer(const RHIStructuredBuffer &source) = delete;

  RHIStructuredBuffer &operator=(const RHIStructuredBuffer &source) = delete;

};

typedef RECore::SmartRefCount<RHIStructuredBuffer> RHIStructuredBufferPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
