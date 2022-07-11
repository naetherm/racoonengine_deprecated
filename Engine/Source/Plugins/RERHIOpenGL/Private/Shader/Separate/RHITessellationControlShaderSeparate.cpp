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
#include "RERHIOpenGL/Shader/Separate/RHITessellationControlShaderSeparate.h"
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
TessellationControlShaderSeparate::TessellationControlShaderSeparate(RHIDynamicRHI& openGLRhi, const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHITessellationControlShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLShaderProgram(Helper::loadShaderProgramFromBytecode(openGLRhi.getContext(), GL_TESS_CONTROL_SHADER, shaderBytecode))
{
  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
				}
#endif
}

TessellationControlShaderSeparate::TessellationControlShaderSeparate(RHIDynamicRHI& openGLRhi, const char* sourceCode, RERHI::ShaderBytecode* shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHITessellationControlShader(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLShaderProgram(Helper::loadShaderProgramFromSourceCode(openGLRhi.getContext(), GL_TESS_CONTROL_SHADER, sourceCode))
{
  // Return shader bytecode, if requested do to so
  if (nullptr != shaderBytecode)
  {
    Helper::shaderSourceCodeToShaderBytecode(openGLRhi.getContext(), GL_TESS_CONTROL_SHADER, sourceCode, *shaderBytecode);
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLShaderProgram && openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TCS", 6)	// 6 = "TCS: " including terminating zero
					glObjectLabel(GL_PROGRAM, mOpenGLShaderProgram, -1, detailedDebugName);
				}
#endif
}

TessellationControlShaderSeparate::~TessellationControlShaderSeparate()
{
  // Destroy the OpenGL shader program
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteProgram(mOpenGLShaderProgram);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
