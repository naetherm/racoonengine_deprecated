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
#include "RERHIOpenGL/Shader/Separate/RHIShaderLanguageSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIVertexShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITessellationControlShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITessellationEvaluationShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIGeometryShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIFragmentShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIComputeShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHITaskShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIMeshShaderSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIGraphicsProgramSeparate.h"
#include "RERHIOpenGL/Shader/Separate/RHIGraphicsProgramSeparateDsa.h"
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
ShaderLanguageSeparate::ShaderLanguageSeparate(RHIDynamicRHI& openGLRhi) :
RHIShaderLanguage(openGLRhi)
  {}

/**
*  @brief
*    Destructor
*/
ShaderLanguageSeparate::~ShaderLanguageSeparate()
{
  // De-initialize glslang, if necessary
#ifdef RHI_OPENGL_GLSLTOSPIRV
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

RERHI::RHIVertexShader* ShaderLanguageSeparate::createVertexShaderFromBytecode(const RERHI::VertexAttributes& vertexAttributes, const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL vertex shader bytecode is invalid")

  // Check whether or not there's vertex shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_vertex_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), VertexShaderSeparate)(openGLRhi, vertexAttributes, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no vertex shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHIVertexShader* ShaderLanguageSeparate::createVertexShaderFromSourceCode(const RERHI::VertexAttributes& vertexAttributes, const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's vertex shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_vertex_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), VertexShaderSeparate)(openGLRhi, vertexAttributes, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no vertex shader support!
    return nullptr;
  }
}

RERHI::RHITessellationControlShader* ShaderLanguageSeparate::createTessellationControlShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation control shader bytecode is invalid")

  // Check whether or not there's tessellation support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationControlShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHITessellationControlShader* ShaderLanguageSeparate::createTessellationControlShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's tessellation support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_tessellation_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationControlShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageSeparate::createTessellationEvaluationShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL tessellation evaluation shader bytecode is invalid")

  // Check whether or not there's tessellation support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_tessellation_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationEvaluationShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHITessellationEvaluationShader* ShaderLanguageSeparate::createTessellationEvaluationShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's tessellation support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_tessellation_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TessellationEvaluationShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no tessellation support!
    return nullptr;
  }
}

RERHI::RHIGeometryShader* ShaderLanguageSeparate::createGeometryShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL geometry shader bytecode is invalid")

  // Check whether or not there's geometry shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_geometry_shader4() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), GeometryShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no geometry shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHIGeometryShader* ShaderLanguageSeparate::createGeometryShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's geometry shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_geometry_shader4())
  {
    return RHI_NEW(openGLRhi.getContext(), GeometryShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no geometry shader support!
    return nullptr;
  }
}

RERHI::RHIFragmentShader* ShaderLanguageSeparate::createFragmentShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL fragment shader bytecode is invalid")

  // Check whether or not there's fragment shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_fragment_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), FragmentShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no fragment shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHIFragmentShader* ShaderLanguageSeparate::createFragmentShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's fragment shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_fragment_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), FragmentShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no fragment shader support!
    return nullptr;
  }
}

RERHI::RHITaskShader* ShaderLanguageSeparate::createTaskShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL task shader bytecode is invalid")

  // Check whether or not there's task shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), TaskShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no task shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHITaskShader* ShaderLanguageSeparate::createTaskShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's task shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_NV_mesh_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), TaskShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no task shader support!
    return nullptr;
  }
}

RERHI::RHIMeshShader* ShaderLanguageSeparate::createMeshShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL mesh shader bytecode is invalid")

  // Check whether or not there's mesh shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_NV_mesh_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), MeshShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no mesh shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHIMeshShader* ShaderLanguageSeparate::createMeshShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's mesh shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_NV_mesh_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), MeshShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no mesh shader support!
    return nullptr;
  }
}

RERHI::RHIComputeShader* ShaderLanguageSeparate::createComputeShaderFromBytecode(const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(shaderBytecode.getNumberOfBytes() > 0 && nullptr != shaderBytecode.getBytecode(), "OpenGL compute shader bytecode is invalid")

  // Check whether or not there's compute shader support
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_compute_shader() && extensions.isGL_ARB_gl_spirv())
  {
    return RHI_NEW(openGLRhi.getContext(), ComputeShaderSeparate)(openGLRhi, shaderBytecode RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no compute shader support or no decent shader bytecode support!
    return nullptr;
  }
}

RERHI::RHIComputeShader* ShaderLanguageSeparate::createComputeShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // Check whether or not there's compute shader support
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
  const Extensions& extensions = openGLRhi.getExtensions();
  if (extensions.isGL_ARB_compute_shader())
  {
    return RHI_NEW(openGLRhi.getContext(), ComputeShaderSeparate)(openGLRhi, shaderSourceCode.sourceCode, (extensions.isGL_ARB_gl_spirv() ? shaderBytecode : nullptr) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Error! There's no compute shader support!
    return nullptr;
  }
}

RERHI::RHIGraphicsProgram* ShaderLanguageSeparate::createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, [[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, RERHI::RHITessellationControlShader* tessellationControlShader, RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  if (nullptr != vertexShader && vertexShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Vertex shader language mismatch!
  }
  else if (nullptr != tessellationControlShader && tessellationControlShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Tessellation control shader language mismatch!
  }
  else if (nullptr != tessellationEvaluationShader && tessellationEvaluationShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Tessellation evaluation shader language mismatch!
  }
  else if (nullptr != geometryShader && geometryShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Geometry shader language mismatch!
  }
  else if (nullptr != fragmentShader && fragmentShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Fragment shader language mismatch!
  }

    // Is "GL_EXT_direct_state_access" there?
  if (openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramSeparateDsa)(openGLRhi, rootSignature, static_cast<VertexShaderSeparate*>(vertexShader), static_cast<TessellationControlShaderSeparate*>(tessellationControlShader), static_cast<TessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<GeometryShaderSeparate*>(geometryShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramSeparate)(openGLRhi, rootSignature, static_cast<VertexShaderSeparate*>(vertexShader), static_cast<TessellationControlShaderSeparate*>(tessellationControlShader), static_cast<TessellationEvaluationShaderSeparate*>(tessellationEvaluationShader), static_cast<GeometryShaderSeparate*>(geometryShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error! Shader language mismatch!
  // -> Ensure a correct reference counter behaviour, even in the situation of an error
  if (nullptr != vertexShader)
  {
    vertexShader->AddReference();
    vertexShader->Release();
  }
  if (nullptr != tessellationControlShader)
  {
    tessellationControlShader->AddReference();
    tessellationControlShader->Release();
  }
  if (nullptr != tessellationEvaluationShader)
  {
    tessellationEvaluationShader->AddReference();
    tessellationEvaluationShader->Release();
  }
  if (nullptr != geometryShader)
  {
    geometryShader->AddReference();
    geometryShader->Release();
  }
  if (nullptr != fragmentShader)
  {
    fragmentShader->AddReference();
    fragmentShader->Release();
  }

  // Error!
  return nullptr;
}

RERHI::RHIGraphicsProgram* ShaderLanguageSeparate::createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, RERHI::RHITaskShader* taskShader, RERHI::RHIMeshShader& meshShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // A shader can be a null pointer, but if it's not the shader and graphics program language must match
  // -> Optimization: Comparing the shader language name by directly comparing the pointer address of
  //    the name is safe because we know that we always reference to one and the same name address
  // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
  if (nullptr != taskShader && taskShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Vertex shader language mismatch!
  }
  else if (meshShader.getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Fragment shader language mismatch!
  }
  else if (nullptr != fragmentShader && fragmentShader->getShaderLanguageName() != ::detail::GLSL_NAME)
  {
    // Error! Vertex shader language mismatch!
  }

    // Is "GL_EXT_direct_state_access" there?
  else if (openGLRhi.getExtensions().isGL_EXT_direct_state_access() || openGLRhi.getExtensions().isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramSeparateDsa)(openGLRhi, rootSignature, static_cast<TaskShaderSeparate*>(taskShader), static_cast<MeshShaderSeparate&>(meshShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), GraphicsProgramSeparate)(openGLRhi, rootSignature, static_cast<TaskShaderSeparate*>(taskShader), static_cast<MeshShaderSeparate&>(meshShader), static_cast<FragmentShaderSeparate*>(fragmentShader) RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }

  // Error! Shader language mismatch!
  // -> Ensure a correct reference counter behaviour, even in the situation of an error
  if (nullptr != taskShader)
  {
    taskShader->AddReference();
    taskShader->Release();
  }
  meshShader.AddReference();
  meshShader.Release();
  if (nullptr != fragmentShader)
  {
    fragmentShader->AddReference();
    fragmentShader->Release();
  }

  // Error!
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
