/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHI/RERHI.h"
#include "RERHI/RHIResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader interface
*/
class RHIShader : public RHIResource
{

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIShader() override
  {}

  // Public virtual Rhi::RHIShader methods
public:
  /**
  *  @brief
  *    Return the name of the shader language the shader is using
  *
  *  @return
  *    The ASCII name of the shader language the shader is using (for example "GLSL" or "HLSL"), always valid
  *
  *  @note
  *    - Do not free the memory the returned pointer is pointing to
  */
  [[nodiscard]] virtual const char* getShaderLanguageName() const = 0;

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] resourceType
  *    Resource type
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline RHIShader(ResourceType resourceType, RHIDynamicRHI& rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIResource(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {}

  explicit RHIShader(const RHIShader& source) = delete;
  RHIShader& operator =(const RHIShader& source) = delete;

};

typedef RECore::SmartRefCount<RHIShader> RHIShaderPtr;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
