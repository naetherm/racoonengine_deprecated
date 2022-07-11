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
#include "RERHIVulkan/RERHIVulkan.h"
#include "RERHIVulkan/Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    GLSL tessellation control shader ("hull shader" in Direct3D terminology) class
*/
class TessellationControlShaderGlsl final : public RERHI::RHITessellationControlShader
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader bytecode
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] shaderBytecode
  *    Shader bytecode
  */
  TessellationControlShaderGlsl(RHIDynamicRHI& vulkanRhi, const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Constructor for creating a tessellation control shader ("hull shader" in Direct3D terminology) shader from shader source code
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] sourceCode
  *    Shader ASCII source code, must be valid
  */
  TessellationControlShaderGlsl(RHIDynamicRHI& vulkanRhi, const char* sourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~TessellationControlShaderGlsl() override;

  /**
  *  @brief
  *    Return the Vulkan shader module
  *
  *  @return
  *    The Vulkan shader module
  */
  [[nodiscard]] inline VkShaderModule getVkShaderModule() const
  {
    return mVkShaderModule;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIShader methods                   ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual const char* getShaderLanguageName() const override
  {
    return ::detail::GLSL_NAME;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), TessellationControlShaderGlsl, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TessellationControlShaderGlsl(const TessellationControlShaderGlsl& source) = delete;
  TessellationControlShaderGlsl& operator =(const TessellationControlShaderGlsl& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VkShaderModule mVkShaderModule;	///< Vulkan shader module, destroy it if you no longer need it


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
