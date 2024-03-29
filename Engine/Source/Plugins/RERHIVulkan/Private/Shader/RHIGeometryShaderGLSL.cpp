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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/Shader/RHIGeometryShaderGLSL.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

GeometryShaderGlsl::GeometryShaderGlsl(RHIDynamicRHI& vulkanRhi, const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGeometryShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mVkShaderModule(::detail::createVkShaderModuleFromBytecode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), shaderBytecode))
{
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (RECore::uint64)mVkShaderModule, detailedDebugName);
				}
#endif
}

/**
*  @brief
*    Constructor for creating a geometry shader from shader source code
*
*  @param[in] vulkanRhi
*    Owner Vulkan RHI instance
*  @param[in] sourceCode
*    Shader ASCII source code, must be valid
*/
GeometryShaderGlsl::GeometryShaderGlsl(RHIDynamicRHI& vulkanRhi, const char* sourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIGeometryShader(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mVkShaderModule(::detail::createVkShaderModuleFromSourceCode(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(), vulkanRhi.getVulkanContext().getVkDevice(), VK_SHADER_STAGE_GEOMETRY_BIT, sourceCode, shaderBytecode))
{
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "GS", 5)	// 5 = "GS: " including terminating zero
					Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (RECore::uint64)mVkShaderModule, detailedDebugName);
				}
#endif
}

/**
*  @brief
*    Destructor
*/
GeometryShaderGlsl::~GeometryShaderGlsl()
{
if (VK_NULL_HANDLE != mVkShaderModule)
{
const RHIDynamicRHI& vulkanRhi = static_cast<RHIDynamicRHI&>(getRhi());
vkDestroyShaderModule(vulkanRhi.getVulkanContext().getVkDevice(), mVkShaderModule, vulkanRhi.getVkAllocationCallbacks());
}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
