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
#include "RERHIOpenGLES3/Shader/RHIFragmentShaderGLSL.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Helper.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

FragmentShaderGlsl::FragmentShaderGlsl(RHIDynamicRHI& openGLES3Rhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIFragmentShader(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLES3Shader(Helper::loadShaderFromSourcecode(openGLES3Rhi, GL_FRAGMENT_SHADER, sourceCode))
{
  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (0 != mOpenGLES3Shader && openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FS", 5)	// 5 = "FS: " including terminating zero
					glObjectLabelKHR(GL_SHADER_KHR, mOpenGLES3Shader, -1, detailedDebugName);
				}
#endif
}

/**
*  @brief
*    Destructor
*/
FragmentShaderGlsl::~FragmentShaderGlsl()
{
// Destroy the OpenGL ES 3 shader
// -> Silently ignores 0's and names that do not correspond to existing buffer objects
glDeleteShader(mOpenGLES3Shader);
}

const char* FragmentShaderGlsl::getShaderLanguageName() const
{
return ::detail::GLSLES_NAME;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
