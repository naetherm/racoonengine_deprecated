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
#include "RERHIVulkan/Shader/RHIShaderLanguageGLSL.h"
#include "RERHIVulkan/Shader/RHIComputeShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIGeometryShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIVulkan/Shader/RHIMeshShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITaskShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationEvaluationShaderGLSL.h"
#include "RERHIVulkan/Shader/RHITessellationControlShaderGLSL.h"
#include "RERHIVulkan/Shader/RHIVertexShaderGLSL.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

ShaderLanguageGlsl::ShaderLanguageGlsl(RHIDynamicRHI &vulkanRhi) :
  RHIShaderLanguage(vulkanRhi) {}

/**
*  @brief
*    Destructor
*/
ShaderLanguageGlsl::~ShaderLanguageGlsl() {
// De-initialize glslang, if necessary
#ifdef RHI_VULKAN_GLSLTOSPIRV
  if (::detail::GlslangInitialized)
          {
            // TODO(naetherm) Fix glslang related memory leaks. See also
            //		    - "Fix a few memory leaks #916" - https://github.com/KhronosGroup/glslang/pull/916
            //		    - "FreeGlobalPools is never called in glslang::FinalizeProcess()'s path. #928" - https://github.com/KhronosGroup/glslang/issues/928
            glslang::FinalizeProcess();
            ::detail::GlslangInitialized = false;
          }
#endif
}

const char *ShaderLanguageGlsl::getShaderLanguageName() const {
  return ::detail::GLSL_NAME;
}

RERHI::RHIVertexShader *
ShaderLanguageGlsl::createVertexShaderFromBytecode([[maybe_unused]] const RERHI::VertexAttributes &vertexAttributes,
                                                   const RERHI::ShaderBytecode &shaderBytecode
                                                   RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan vertex shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), VertexShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIVertexShader *
ShaderLanguageGlsl::createVertexShaderFromSourceCode([[maybe_unused]] const RERHI::VertexAttributes &vertexAttributes,
                                                     const RERHI::ShaderSourceCode &shaderSourceCode,
                                                     RERHI::ShaderBytecode *shaderBytecode
                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), VertexShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                           RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationControlShader *
ShaderLanguageGlsl::createTessellationControlShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                                RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan tessellation control shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), TessellationControlShaderGlsl)(vulkanRhi, shaderBytecode
                                                                        RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationControlShader *
ShaderLanguageGlsl::createTessellationControlShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                                  RERHI::ShaderBytecode *shaderBytecode
                                                                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), TessellationControlShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode,
                                                                        shaderBytecode
                                                                        RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationEvaluationShader *
ShaderLanguageGlsl::createTessellationEvaluationShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                                   RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan tessellation evaluation shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), TessellationEvaluationShaderGlsl)(vulkanRhi, shaderBytecode
                                                                           RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITessellationEvaluationShader *
ShaderLanguageGlsl::createTessellationEvaluationShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                                     RERHI::ShaderBytecode *shaderBytecode
                                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), TessellationEvaluationShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode,
                                                                           shaderBytecode
                                                                           RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGeometryShader *
ShaderLanguageGlsl::createGeometryShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan geometry shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), GeometryShaderGlsl)(vulkanRhi, shaderBytecode
                                                             RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGeometryShader *
ShaderLanguageGlsl::createGeometryShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                       RERHI::ShaderBytecode *shaderBytecode
                                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), GeometryShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                             RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFragmentShader *
ShaderLanguageGlsl::createFragmentShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan fragment shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), FragmentShaderGlsl)(vulkanRhi, shaderBytecode
                                                             RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIFragmentShader *
ShaderLanguageGlsl::createFragmentShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                       RERHI::ShaderBytecode *shaderBytecode
                                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), FragmentShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                             RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITaskShader *ShaderLanguageGlsl::createTaskShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan task shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), TaskShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITaskShader *
ShaderLanguageGlsl::createTaskShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                   RERHI::ShaderBytecode *shaderBytecode
                                                   RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), TaskShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                         RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIMeshShader *ShaderLanguageGlsl::createMeshShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan mesh shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), MeshShaderGlsl)(vulkanRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIMeshShader *
ShaderLanguageGlsl::createMeshShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                   RERHI::ShaderBytecode *shaderBytecode
                                                   RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), MeshShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                         RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIComputeShader *ShaderLanguageGlsl::createComputeShaderFromBytecode(const RERHI::ShaderBytecode &shaderBytecode
                                                                             RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(),
             "Vulkan compute shader bytecode is invalid")

// Create shader instance
  return RHI_NEW(vulkanRhi.getContext(), ComputeShaderGlsl)(vulkanRhi, shaderBytecode
                                                            RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIComputeShader *
ShaderLanguageGlsl::createComputeShaderFromSourceCode(const RERHI::ShaderSourceCode &shaderSourceCode,
                                                      RERHI::ShaderBytecode *shaderBytecode
                                                      RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  return RHI_NEW(vulkanRhi.getContext(), ComputeShaderGlsl)(vulkanRhi, shaderSourceCode.sourceCode, shaderBytecode
                                                            RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsProgram *
ShaderLanguageGlsl::createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature &rootSignature,
                                          [[maybe_unused]] const RERHI::VertexAttributes &vertexAttributes,
                                          RERHI::RHIVertexShader *vertexShader,
                                          RERHI::RHITessellationControlShader *tessellationControlShader,
                                          RERHI::RHITessellationEvaluationShader *tessellationEvaluationShader,
                                          RERHI::RHIGeometryShader *geometryShader,
                                          RERHI::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity checks
// -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
// -> Optimization: Comparing the shader language name by directly comparing the pointer address of
//    the name is safe because we know that we always reference to one and the same name address
// TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == vertexShader || vertexShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan vertex shader language mismatch")
  RHI_ASSERT(
    nullptr == tessellationControlShader || tessellationControlShader->getShaderLanguageName() == ::detail::GLSL_NAME,
    "Vulkan tessellation control shader language mismatch")
  RHI_ASSERT(nullptr == tessellationEvaluationShader ||
             tessellationEvaluationShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan tessellation evaluation shader language mismatch")
  RHI_ASSERT(nullptr == geometryShader || geometryShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan geometry shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan fragment shader language mismatch")

// Create the graphics program
  return RHI_NEW(vulkanRhi.getContext(), GraphicsProgramGlsl)(vulkanRhi, static_cast<VertexShaderGlsl *>(vertexShader),
                                                              static_cast<TessellationControlShaderGlsl *>(tessellationControlShader),
                                                              static_cast<TessellationEvaluationShaderGlsl *>(tessellationEvaluationShader),
                                                              static_cast<GeometryShaderGlsl *>(geometryShader),
                                                              static_cast<FragmentShaderGlsl *>(fragmentShader)
                                                              RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHIGraphicsProgram *
ShaderLanguageGlsl::createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature &rootSignature,
                                          RERHI::RHITaskShader *taskShader, RERHI::RHIMeshShader &meshShader,
                                          RERHI::RHIFragmentShader *fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

  // Sanity checks
  // -> A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  RHI_ASSERT(nullptr == taskShader || taskShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan task shader language mismatch")
  RHI_ASSERT(meshShader.getShaderLanguageName() == ::detail::GLSL_NAME, "Vulkan mesh shader language mismatch")
  RHI_ASSERT(nullptr == fragmentShader || fragmentShader->getShaderLanguageName() == ::detail::GLSL_NAME,
             "Vulkan fragment shader language mismatch")

  // Create the graphics program
  return RHI_NEW(vulkanRhi.getContext(), GraphicsProgramGlsl)(vulkanRhi, static_cast<TaskShaderGlsl *>(taskShader),
                                                              static_cast<MeshShaderGlsl &>(meshShader),
                                                              static_cast<FragmentShaderGlsl *>(fragmentShader)
                                                              RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
