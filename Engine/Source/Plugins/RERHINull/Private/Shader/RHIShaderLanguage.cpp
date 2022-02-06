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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHINull/Shader/RHIShaderLanguage.h"
#include "RERHINull/Shader/RHIFragmentShader.h"
#include "RERHINull/Shader/RHIGeometryShader.h"
#include "RERHINull/Shader/RHIGraphicsProgram.h"
#include "RERHINull/Shader/RHITaskShader.h"
#include "RERHINull/Shader/RHIMeshShader.h"
#include "RERHINull/Shader/RHITessellationControlShader.h"
#include "RERHINull/Shader/RHITessellationEvaluationShader.h"
#include "RERHINull/Shader/RHIVertexShader.h"
#include "RERHINull/Shader/RHIComputeShader.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] nullRhi
*    Owner null RHI instance
*/
ShaderLanguage::ShaderLanguage(RHIDynamicRHI& nullRhi) :
RHIShaderLanguage(nullRhi)
  {}

/**
*  @brief
*    Destructor
*/
ShaderLanguage::~ShaderLanguage()
{}


const char* ShaderLanguage::getShaderLanguageName() const
{
  return ::detail::NULL_NAME;
}

RERHI::RHIVertexShader* ShaderLanguage::createVertexShaderFromBytecode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, [[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::vertexShader", we know there's vertex shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), VertexShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIVertexShader* ShaderLanguage::createVertexShaderFromSourceCode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, [[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::vertexShader", we know there's vertex shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), VertexShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationControlShader* ShaderLanguage::createTessellationControlShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation control shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TessellationControlShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationControlShader* ShaderLanguage::createTessellationControlShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation control shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TessellationControlShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationEvaluationShader* ShaderLanguage::createTessellationEvaluationShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation evaluation shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TessellationEvaluationShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationEvaluationShader* ShaderLanguage::createTessellationEvaluationShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfPatchVertices", we know there's tessellation evaluation shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TessellationEvaluationShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGeometryShader* ShaderLanguage::createGeometryShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfGsOutputVertices", we know there's geometry shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), GeometryShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGeometryShader* ShaderLanguage::createGeometryShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::maximumNumberOfGsOutputVertices", we know there's geometry shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), GeometryShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFragmentShader* ShaderLanguage::createFragmentShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::fragmentShader", we know there's fragment shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), FragmentShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFragmentShader* ShaderLanguage::createFragmentShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::fragmentShader", we know there's fragment shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), FragmentShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITaskShader* ShaderLanguage::createTaskShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::meshShader", we know there's mesh shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TaskShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITaskShader* ShaderLanguage::createTaskShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::meshShader", we know there's mesh shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), TaskShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIMeshShader* ShaderLanguage::createMeshShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::meshShader", we know there's mesh shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), MeshShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIMeshShader* ShaderLanguage::createMeshShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::meshShader", we know there's mesh shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), MeshShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIComputeShader* ShaderLanguage::createComputeShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::computeShader", we know there's compute shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), ComputeShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIComputeShader* ShaderLanguage::createComputeShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // There's no need to check for "RERHI::Capabilities::computeShader", we know there's compute shader support
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());
  return RHI_NEW(nullRhi.getContext(), ComputeShader)(nullRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsProgram* ShaderLanguage::createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, [[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, RERHI::RHITessellationControlShader* tessellationControlShader, RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null vertex shader language mismatch")
  RHI_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null tessellation control shader language mismatch")
  RHI_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null tessellation evaluation shader language mismatch")
  RHI_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null geometry shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null fragment shader language mismatch")

  // Create the graphics program
  return RHI_NEW(nullRhi.getContext(), GraphicsProgram)(nullRhi, static_cast<VertexShader*>(vertexShader), static_cast<TessellationControlShader*>(tessellationControlShader), static_cast<TessellationEvaluationShader*>(tessellationEvaluationShader), static_cast<GeometryShader*>(geometryShader), static_cast<FragmentShader*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsProgram* ShaderLanguage::createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, RERHI::RHITaskShader* taskShader, RERHI::RHIMeshShader& meshShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null task shader language mismatch")
  RHI_ASSERT(meshShader.getShaderLanguageName() == ::detail::NULL_NAME, "Null mesh shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::NULL_NAME, "Null fragment shader language mismatch")

  // Create the graphics program
  return RHI_NEW(nullRhi.getContext(), GraphicsProgram)(nullRhi, static_cast<TaskShader*>(taskShader), static_cast<MeshShader&>(meshShader), static_cast<FragmentShader*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


void ShaderLanguage::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), ShaderLanguage, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
