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
#include "RERHIOpenGL/Shader/Monolithic/RHIShaderLanguageMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIComputeShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIVertexShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHITessellationControlShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHITessellationEvaluationShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIFragmentShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIGeometryShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHITaskShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIMeshShaderMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIGraphicsProgramMonolithic.h"
#include "RERHIOpenGL/Shader/Monolithic/RHIGraphicsProgramMonolithicDsa.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Helper.h"


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

ShaderLanguageMonolithic::ShaderLanguageMonolithic(RHIDynamicRHI& openGLRhi) :
RHIShaderLanguage(openGLRhi)
  {}

RERHI::RHIVertexShader* ShaderLanguageMonolithic::createVertexShaderFromBytecode(const RERHI::VertexAttributes&, const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHIVertexShader* ShaderLanguageMonolithic::createVertexShaderFromSourceCode(const RERHI::VertexAttributes& vertexAttributes, const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's vertex shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_vertex_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), VertexShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no vertex shader support!
    return nullptr;
  }
}

RERHI::RHITessellationControlShader* ShaderLanguageMonolithic::createTessellationControlShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHITessellationControlShader* ShaderLanguageMonolithic::createTessellationControlShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's tessellation support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_tessellation_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationControlShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageMonolithic::createTessellationEvaluationShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageMonolithic::createTessellationEvaluationShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's tessellation support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_tessellation_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationEvaluationShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

RERHI::RHIGeometryShader* ShaderLanguageMonolithic::createGeometryShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHIGeometryShader* ShaderLanguageMonolithic::createGeometryShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's geometry shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_geometry_shader4())
  {
    return RHI_NEW(openGLRhi.getContext(), GeometryShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no geometry shader support!
    return nullptr;
  }
}

RERHI::RHIFragmentShader* ShaderLanguageMonolithic::createFragmentShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHIFragmentShader* ShaderLanguageMonolithic::createFragmentShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's fragment shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_fragment_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), FragmentShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no fragment shader support!
    return nullptr;
  }
}

RERHI::RHITaskShader* ShaderLanguageMonolithic::createTaskShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHITaskShader* ShaderLanguageMonolithic::createTaskShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's mesh shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_NV_mesh_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TaskShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no task shader support!
    return nullptr;
  }
}

RERHI::RHIMeshShader* ShaderLanguageMonolithic::createMeshShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHIMeshShader* ShaderLanguageMonolithic::createMeshShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's mesh shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_NV_mesh_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), MeshShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no mesh shader support!
    return nullptr;
  }
}

RERHI::RHIComputeShader* ShaderLanguageMonolithic::createComputeShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  // Error!
  RHI_ASSERT(false, "OpenGL monolithic shaders have no shader bytecode, only a monolithic program bytecode")
  return nullptr;
}

RERHI::RHIComputeShader* ShaderLanguageMonolithic::createComputeShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  // Check whether or not there's compute shader support
  // -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (openGLRhi.getExtensions().isGL_ARB_compute_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), ComputeShaderMonolithic)(openGLRhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no compute shader support!
    return nullptr;
  }
}

RERHI::RHIGraphicsProgram* ShaderLanguageMonolithic::createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, RERHI::RHITessellationControlShader* tessellationControlShader, RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL vertex shader language mismatch")
  RHI_ASSERT(nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL tessellation control shader language mismatch")
  RHI_ASSERT(nullptr == tessellationEvaluationShader || tessellationEvaluationShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL tessellation evaluation shader language mismatch")
  RHI_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL geometry shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL fragment shader language mismatch")

  // Create the graphics program: Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramMonolithicDsa)(openGLRhi, rootSignature, vertexAttributes, static_cast<VertexShaderMonolithic*>(vertexShader), static_cast<TessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<TessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<GeometryShaderMonolithic*>(geometryShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramMonolithic)(openGLRhi, rootSignature, vertexAttributes, static_cast<VertexShaderMonolithic*>(vertexShader), static_cast<TessellationControlShaderMonolithic*>(tessellationControlShader), static_cast<TessellationEvaluationShaderMonolithic*>(tessellationEvaluationShader), static_cast<GeometryShaderMonolithic*>(geometryShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

RERHI::RHIGraphicsProgram* ShaderLanguageMonolithic::createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, RERHI::RHITaskShader* taskShader, RERHI::RHIMeshShader& meshShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL task shader language mismatch")
  RHI_ASSERT(meshShader.getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL mesh shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME, "OpenGL fragment shader language mismatch")

  // Create the graphics program: Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramMonolithicDsa)(openGLRhi, rootSignature, static_cast<TaskShaderMonolithic*>(taskShader), static_cast<MeshShaderMonolithic&>(meshShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramMonolithic)(openGLRhi, rootSignature, static_cast<TaskShaderMonolithic*>(taskShader), static_cast<MeshShaderMonolithic&>(meshShader), static_cast<FragmentShaderMonolithic*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
