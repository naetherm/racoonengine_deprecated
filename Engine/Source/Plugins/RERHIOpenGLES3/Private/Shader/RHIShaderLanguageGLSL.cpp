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
#include "RERHIOpenGLES3/Shader/RHIShaderLanguageGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIVertexShaderGLSL.h"
#include "RERHIOpenGLES3/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Helper.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

ShaderLanguageGlsl::ShaderLanguageGlsl(RHIDynamicRHI& openGLES3Rhi) :
RHIShaderLanguage(openGLES3Rhi)
  {}


const char* ShaderLanguageGlsl::getShaderLanguageName() const
{
return ::detail::GLSLES_NAME;
}

RERHI::RHIVertexShader* ShaderLanguageGlsl::createVertexShaderFromBytecode(const RERHI::VertexAttributes&, const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHIVertexShader* ShaderLanguageGlsl::createVertexShaderFromSourceCode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
// There's no need to check for "RERHI::Capabilities::vertexShader", we know there's vertex shader support
// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
return RHI_NEW(openGLES3Rhi.getContext(), VertexShaderGlsl)(openGLES3Rhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationControlShader* ShaderLanguageGlsl::createTessellationControlShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHITessellationControlShader* ShaderLanguageGlsl::createTessellationControlShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no tessellation control shader support")
return nullptr;
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageGlsl::createTessellationEvaluationShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageGlsl::createTessellationEvaluationShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no tessellation evaluation shader support")
return nullptr;
}

RERHI::RHIGeometryShader* ShaderLanguageGlsl::createGeometryShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHIGeometryShader* ShaderLanguageGlsl::createGeometryShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no geometry shader support")
return nullptr;
}

RERHI::RHIFragmentShader* ShaderLanguageGlsl::createFragmentShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHIFragmentShader* ShaderLanguageGlsl::createFragmentShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
// There's no need to check for "RERHI::Capabilities::fragmentShader", we know there's fragment shader support
// -> Monolithic shaders have no shader bytecode, only a monolithic program bytecode
RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
return RHI_NEW(openGLES3Rhi.getContext(), FragmentShaderGlsl)(openGLES3Rhi, shaderSourceCode.sourceCode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITaskShader* ShaderLanguageGlsl::createTaskShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders has no task shader support")
return nullptr;
}

RERHI::RHITaskShader* ShaderLanguageGlsl::createTaskShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no task shader support")
return nullptr;
}

RERHI::RHIMeshShader* ShaderLanguageGlsl::createMeshShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders has no mesh shader support")
return nullptr;
}

RERHI::RHIMeshShader* ShaderLanguageGlsl::createMeshShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no mesh shader support")
return nullptr;
}

RERHI::RHIComputeShader* ShaderLanguageGlsl::createComputeShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
// Error!
RHI_ASSERT(false, "OpenGL ES 3 monolithic shaders have no shader bytecode, only a monolithic program bytecode")
return nullptr;
}

RERHI::RHIComputeShader* ShaderLanguageGlsl::createComputeShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
RHI_ASSERT(false, "OpenGL ES 3 has no compute shader support")
return nullptr;
}

RERHI::RHIGraphicsProgram* ShaderLanguageGlsl::createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, [[maybe_unused]] RERHI::RHITessellationControlShader* tessellationControlShader, [[maybe_unused]] RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, [[maybe_unused]] RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());

// Sanity checks
// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
//    the name is safe because we know that we always reference to one and the same name address
// TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
RHI_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::GLSLES_NAME, "OpenGL ES 3 vertex shader language mismatch")
RHI_ASSERT(nullptr == tessellationControlShader, "OpenGL ES 3 has no tessellation control shader support")
RHI_ASSERT(nullptr == tessellationEvaluationShader, "OpenGL ES 3 has no tessellation evaluation shader support")
RHI_ASSERT(nullptr == geometryShader, "OpenGL ES 3 has no geometry shader support")
RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSLES_NAME, "OpenGL ES 3 fragment shader language mismatch")

// Create the graphics program
return RHI_NEW(openGLES3Rhi.getContext(), GraphicsProgramGlsl)(openGLES3Rhi, rootSignature, vertexAttributes, static_cast<VertexShaderGlsl*>(vertexShader), static_cast<FragmentShaderGlsl*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsProgram* ShaderLanguageGlsl::createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, [[maybe_unused]] RERHI::RHITaskShader* taskShader, [[maybe_unused]] RERHI::RHIMeshShader& meshShader, [[maybe_unused]] RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER)
{
  RHI_ASSERT(false, "OpenGL ES 3 has no mesh shader support")
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
