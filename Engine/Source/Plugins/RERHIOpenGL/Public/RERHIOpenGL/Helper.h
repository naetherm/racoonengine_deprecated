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
#include "RERHIOpenGL/RERHIOpenGL.h"
#include "RERHI/RHIRootSignatureTypes.h"
#include <RERHI/RHIContext.h>
#include <RERHI/Shader/RHIShaderTypes.h>
#include <RERHI/Buffer/RHIVertexArrayTypes.h>


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
class Helper {
public:

  static void updateWidthHeight(RECore::uint32 mipmapIndex, RECore::uint32 textureWidth, RECore::uint32 textureHeight, RECore::uint32& width, RECore::uint32& height);

  static void printOpenGLShaderInformationIntoLog(const RERHI::RHIContext& context, GLuint openGLShader);

  static void printOpenGLShaderInformationIntoLog(const RERHI::RHIContext& context, GLuint openGLShader, const char* sourceCode);

  static void printOpenGLProgramInformationIntoLog(const RERHI::RHIContext& context, GLuint openGLProgram);

  static void printOpenGLProgramInformationIntoLog(const RERHI::RHIContext& context, GLuint openGLProgram, const char* sourceCode);

  /**
  *  @brief
  *    Create and load a shader from bytecode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] shaderType
  *    Shader type (for example "GL_VERTEX_SHADER_ARB")
  *  @param[in] shaderBytecode
  *    Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
  *
  *  @return
  *    The OpenGL shader, 0 on error, destroy the resource if you no longer need it
  */
  [[nodiscard]] static GLuint loadShaderFromBytecode(const RERHI::RHIContext& context, GLenum shaderType, const RERHI::ShaderBytecode& shaderBytecode);

  /**
  *  @brief
  *    Create and load a shader program from bytecode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] shaderType
  *    Shader type (for example "GL_VERTEX_SHADER_ARB")
  *  @param[in] shaderBytecode
  *    Shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
  *
  *  @return
  *    The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
  */
  [[nodiscard]] static GLuint loadShaderProgramFromBytecode(const RERHI::RHIContext& context, GLenum shaderType, const RERHI::ShaderBytecode& shaderBytecode);

  /**
  *  @brief
  *    Create, load and compile a shader program from source code
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] shaderType
  *    Shader type (for example "GL_VERTEX_SHADER_ARB")
  *  @param[in] sourceCode
  *    Shader ASCII source code, must be a valid pointer
  *
  *  @return
  *    The OpenGL shader program, 0 on error, destroy the resource if you no longer need it
  */
  [[nodiscard]] static GLuint loadShaderProgramFromSourceCode(const RERHI::RHIContext& context, GLenum shaderType, const GLchar* sourceCode);

  [[nodiscard]] static GLuint createShaderProgramObject(const RERHI::RHIContext& context, GLuint openGLShader, const RERHI::VertexAttributes& vertexAttributes);

  [[nodiscard]] static GLuint loadShaderProgramFromBytecode(const RERHI::RHIContext& context, const RERHI::VertexAttributes& vertexAttributes, GLenum shaderType, const RERHI::ShaderBytecode& shaderBytecode);

  /**
  *  @brief
  *    Creates, loads and compiles a shader from source code
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] shaderType
  *    Shader type (for example "GL_VERTEX_SHADER_ARB")
  *  @param[in] sourceCode
  *    Shader ASCII source code, must be a valid pointer
  *
  *  @return
  *    The OpenGL shader, 0 on error, destroy the resource if you no longer need it
  */
  [[nodiscard]] static GLuint loadShaderFromSourcecode(const RERHI::RHIContext& context, GLenum shaderType, const GLchar* sourceCode);

  [[nodiscard]] static GLuint loadShaderProgramFromSourcecode(const RERHI::RHIContext& context, const RERHI::VertexAttributes& vertexAttributes, GLenum type, const char* sourceCode);

  /**
  *  @brief
  *    Compile shader source code to shader bytecode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] shaderType
  *    Shader type (for example "GL_VERTEX_SHADER_ARB")
  *  @param[in] sourceCode
  *    Shader ASCII source code, must be a valid pointer
  *  @param[out] shaderBytecode
  *    Receives the shader SPIR-V bytecode ("GL_ARB_gl_spirv"-extension) compressed via SMOL-V
  */
  // TODO(naetherm) Visual Studio 2017 compile settings: For some reasons I need to disable optimization for the following method or else "glslang::TShader::parse()" will output the error "ERROR: 0:1: '�' : unexpected token" (glslang (latest commit c325f4364666eedb94c20a13670df058a38a14ab - April 20, 2018), Visual Studio 2017 15.7.1)
  static void shaderSourceCodeToShaderBytecode(const RERHI::RHIContext& context, GLenum shaderType, const GLchar* sourceCode, RERHI::ShaderBytecode& shaderBytecode);

  static void bindUniformBlock(const RERHI::DescriptorRange& descriptorRange, RECore::uint32 openGLProgram, RECore::uint32 uniformBlockBindingIndex);

  static void bindUniformLocation(const RERHI::DescriptorRange& descriptorRange, RECore::uint32 openGLProgramPipeline, RECore::uint32 openGLProgram);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
