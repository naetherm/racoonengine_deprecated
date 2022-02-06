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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class VertexShaderGlsl;
class FragmentShaderGlsl;


/**
*  @brief
*    GLSL graphics program class
*/
class GraphicsProgramGlsl final : public RERHI::RHIGraphicsProgram
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] vertexAttributes
  *    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
  *  @param[in] vertexShaderGlsl
  *    Vertex shader the graphics program is using, can be a null pointer
  *  @param[in] fragmentShaderGlsl
  *    Fragment shader the graphics program is using, can be a null pointer
  *
  *  @note
  *    - The graphics program keeps a reference to the provided shaders and releases it when no longer required
  */
  GraphicsProgramGlsl(RHIDynamicRHI& openGLES3Rhi, const RERHI::RHIRootSignature& rootSignature, const RERHI::VertexAttributes& vertexAttributes, VertexShaderGlsl* vertexShaderGlsl, FragmentShaderGlsl* fragmentShaderGlsl RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~GraphicsProgramGlsl() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 program
  *
  *  @return
  *    The OpenGL ES 3 program, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3Program() const
  {
    return mOpenGLES3Program;
  }

  /**
  *  @brief
  *    Return the draw ID uniform location
  *
  *  @return
  *    Draw ID uniform location, -1 if there's no such uniform
  */
  [[nodiscard]] inline GLint getDrawIdUniformLocation() const
  {
    return mDrawIdUniformLocation;
  }

  //[-------------------------------------------------------]
  //[ Setters                                               ]
  //[-------------------------------------------------------]
  void setUniform1i(RECore::handle uniformHandle, int value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniform1i(static_cast<GLint>(uniformHandle), value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniform1i(static_cast<GLint>(uniformHandle), value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniform1i(static_cast<GLint>(uniformHandle), value);
#endif
  }

  void setUniform1f(RECore::handle uniformHandle, float value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniform1f(static_cast<GLint>(uniformHandle), value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniform1f(static_cast<GLint>(uniformHandle), value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniform1f(static_cast<GLint>(uniformHandle), value);
#endif
  }

  void setUniform2fv(RECore::handle uniformHandle, const float* value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniform2fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
  }

  void setUniform3fv(RECore::handle uniformHandle, const float* value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniform3fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
  }

  void setUniform4fv(RECore::handle uniformHandle, const float* value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniform4fv(static_cast<GLint>(uniformHandle), 1, value);
#endif
  }

  void setUniformMatrix3fv(RECore::handle uniformHandle, const float* value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniformMatrix3fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
  }

  void setUniformMatrix4fv(RECore::handle uniformHandle, const float* value)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently used OpenGL ES 3 program
				GLint openGLES3ProgramBackup = 0;
				glGetIntegerv(GL_CURRENT_PROGRAM, &openGLES3ProgramBackup);
				if (openGLES3ProgramBackup == static_cast<GLint>(mOpenGLES3Program))
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
				}
				else
				{
					// Set uniform, please note that for this our program must be the currently used one
					glUseProgram(mOpenGLES3Program);
					glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);

					// Be polite and restore the previous used OpenGL ES 3 program
					glUseProgram(static_cast<GLuint>(openGLES3ProgramBackup));
				}
#else
    // Set uniform, please note that for this our program must be the currently used one
    glUseProgram(mOpenGLES3Program);
    glUniformMatrix4fv(static_cast<GLint>(uniformHandle), 1, GL_FALSE, value);
#endif
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIGraphicsProgram methods          ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual RECore::handle getUniformHandle(const char* uniformName) override
  {
    return static_cast<RECore::handle>(glGetUniformLocation(mOpenGLES3Program, uniformName));
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), GraphicsProgramGlsl, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit GraphicsProgramGlsl(const GraphicsProgramGlsl& source) = delete;
  GraphicsProgramGlsl& operator =(const GraphicsProgramGlsl& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  uint32_t mNumberOfRootSignatureParameters;	///< Number of root signature parameters
  GLuint	 mOpenGLES3Program;					///< OpenGL ES 3 program, can be zero if no resource is allocated
  GLint	 mDrawIdUniformLocation;			///< Draw ID uniform location, used for "GL_EXT_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
