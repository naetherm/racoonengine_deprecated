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
#include "RERHIOpenGL/Helper.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include <RERHI/Texture/RHITexture1D.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace {
namespace detail {
#ifdef RHI_OPENGL_GLSLTOSPIRV
static bool GlslangInitialized = false;

			// Settings from "glslang/StandAlone/ResourceLimits.cpp"
			static constexpr TBuiltInResource DefaultTBuiltInResource =
			{
				32,		///< MaxLights
				6,		///< MaxClipPlanes
				32,		///< MaxTextureUnits
				32,		///< MaxTextureCoords
				64,		///< MaxVertexAttribs
				4096,	///< MaxVertexUniformComponents
				64,		///< MaxVaryingFloats
				32,		///< MaxVertexTextureImageUnits
				80,		///< MaxCombinedTextureImageUnits
				32,		///< MaxTextureImageUnits
				4096,	///< MaxFragmentUniformComponents
				32,		///< MaxDrawBuffers
				128,	///< MaxVertexUniformVectors
				8,		///< MaxVaryingVectors
				16,		///< MaxFragmentUniformVectors
				16,		///< MaxVertexOutputVectors
				15,		///< MaxFragmentInputVectors
				-8,		///< MinProgramTexelOffset
				7,		///< MaxProgramTexelOffset
				8,		///< MaxClipDistances
				65535,	///< MaxComputeWorkGroupCountX
				65535,	///< MaxComputeWorkGroupCountY
				65535,	///< MaxComputeWorkGroupCountZ
				1024,	///< MaxComputeWorkGroupSizeX
				1024,	///< MaxComputeWorkGroupSizeY
				64,		///< MaxComputeWorkGroupSizeZ
				1024,	///< MaxComputeUniformComponents
				16,		///< MaxComputeTextureImageUnits
				8,		///< MaxComputeImageUniforms
				8,		///< MaxComputeAtomicCounters
				1,		///< MaxComputeAtomicCounterBuffers
				60,		///< MaxVaryingComponents
				64,		///< MaxVertexOutputComponents
				64,		///< MaxGeometryInputComponents
				128,	///< MaxGeometryOutputComponents
				128,	///< MaxFragmentInputComponents
				8,		///< MaxImageUnits
				8,		///< MaxCombinedImageUnitsAndFragmentOutputs
				8,		///< MaxCombinedShaderOutputResources
				0,		///< MaxImageSamples
				0,		///< MaxVertexImageUniforms
				0,		///< MaxTessControlImageUniforms
				0,		///< MaxTessEvaluationImageUniforms
				0,		///< MaxGeometryImageUniforms
				8,		///< MaxFragmentImageUniforms
				8,		///< MaxCombinedImageUniforms
				16,		///< MaxGeometryTextureImageUnits
				256,	///< MaxGeometryOutputVertices
				1024,	///< MaxGeometryTotalOutputComponents
				1024,	///< MaxGeometryUniformComponents
				64,		///< MaxGeometryVaryingComponents
				128,	///< MaxTessControlInputComponents
				128,	///< MaxTessControlOutputComponents
				16,		///< MaxTessControlTextureImageUnits
				1024,	///< MaxTessControlUniformComponents
				4096,	///< MaxTessControlTotalOutputComponents
				128,	///< MaxTessEvaluationInputComponents
				128,	///< MaxTessEvaluationOutputComponents
				16,		///< MaxTessEvaluationTextureImageUnits
				1024,	///< MaxTessEvaluationUniformComponents
				120,	///< MaxTessPatchComponents
				32,		///< MaxPatchVertices
				64,		///< MaxTessGenLevel
				16,		///< MaxViewports
				0,		///< MaxVertexAtomicCounters
				0,		///< MaxTessControlAtomicCounters
				0,		///< MaxTessEvaluationAtomicCounters
				0,		///< MaxGeometryAtomicCounters
				8,		///< MaxFragmentAtomicCounters
				8,		///< MaxCombinedAtomicCounters
				1,		///< MaxAtomicCounterBindings
				0,		///< MaxVertexAtomicCounterBuffers
				0,		///< MaxTessControlAtomicCounterBuffers
				0,		///< MaxTessEvaluationAtomicCounterBuffers
				0,		///< MaxGeometryAtomicCounterBuffers
				1,		///< MaxFragmentAtomicCounterBuffers
				1,		///< MaxCombinedAtomicCounterBuffers
				16384,	///< MaxAtomicCounterBufferSize
				4,		///< MaxTransformFeedbackBuffers
				64,		///< MaxTransformFeedbackInterleavedComponents
				8,		///< MaxCullDistances
				8,		///< MaxCombinedClipAndCullDistances
				4,		///< MaxSamples
				256,	///< MaxMeshOutputVerticesNV
				512,	///< MaxMeshOutputPrimitivesNV
				32,		///< MaxMeshWorkGroupSizeX_NV
				1,		///< MaxMeshWorkGroupSizeY_NV
				1,		///< MaxMeshWorkGroupSizeZ_NV
				32,		///< MaxTaskWorkGroupSizeX_NV
				1,		///< MaxTaskWorkGroupSizeY_NV
				1,		///< MaxTaskWorkGroupSizeZ_NV
				4,		///< MaxMeshViewCountNV
        35068,
				{		///< limits
					1,	///< nonInductiveForLoops
					1,	///< whileLoops
					1,	///< doWhileLoops
					1,	///< generalUniformIndexing
					1,	///< generalAttributeMatrixVectorIndexing
					1,	///< generalVaryingIndexing
					1,	///< generalSamplerIndexing
					1,	///< generalVariableIndexing
					1,	///< generalConstantMatrixVectorIndexing
				}
			};
#endif
}
}


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
void Helper::updateWidthHeight(RECore::uint32 mipmapIndex, RECore::uint32 textureWidth, RECore::uint32 textureHeight, RECore::uint32 &width,
                               RECore::uint32 &height) {
  RERHI::RHITexture::getMipmapSize(mipmapIndex, textureWidth, textureHeight);
  if (width > textureWidth)
  {
    width = textureWidth;
  }
  if (height > textureHeight)
  {
    height = textureHeight;
  }
}

void Helper::printOpenGLShaderInformationIntoLog(const RERHI::RHIContext &context, GLuint openGLShader) {
  // Get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1)
  {
    // Allocate memory for the information
    char* informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

    // Get the information
    glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

    // Output the debug string
    RE_LOG(Critical, informationLog)

    // Cleanup information memory
    RHI_FREE(context, informationLog);
  }
}

void Helper::printOpenGLShaderInformationIntoLog(const RERHI::RHIContext &context, GLuint openGLShader,
                                                 const char *sourceCode) {
  // Get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1)
  {
    // Allocate memory for the information
    char* informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

    // Get the information
    glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

    // Output the debug string
    RE_LOG(Critical, std::string(sourceCode))
    //if (context.getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), informationLog))
    {
      //	DEBUG_BREAK;
    }

    // Cleanup information memory
    RHI_FREE(context, informationLog);
  }
}

void Helper::printOpenGLProgramInformationIntoLog(const RERHI::RHIContext &context, GLuint openGLProgram) {
  // Get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1)
  {
    // Allocate memory for the information
    char* informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

    // Get the information
    glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

    // Output the debug string
    RE_LOG(Critical, informationLog)

    // Cleanup information memory
    RHI_FREE(context, informationLog);
  }
}

void Helper::printOpenGLProgramInformationIntoLog(const RERHI::RHIContext &context, GLuint openGLProgram,
                                                  const char *sourceCode) {
  // Get the length of the information (including a null termination)
  GLint informationLength = 0;
  glGetProgramiv(openGLProgram, GL_INFO_LOG_LENGTH, &informationLength);
  if (informationLength > 1)
  {
    // Allocate memory for the information
    char* informationLog = RHI_MALLOC_TYPED(context, char, informationLength);

    // Get the information
    glGetProgramInfoLog(openGLProgram, informationLength, nullptr, informationLog);

    // Output the debug string
    RE_LOG(Critical, std::string(sourceCode))
    //if (context.getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), informationLog))
    {
      //	DEBUG_BREAK;
    }

    // Cleanup information memory
    RHI_FREE(context, informationLog);
  }
}

GLuint Helper::loadShaderFromBytecode(const RERHI::RHIContext &context, GLenum shaderType,
                                      const RERHI::ShaderBytecode &shaderBytecode) {
  // Create the shader object
  const GLuint openGLShader = glCreateShader(shaderType);

  // Load the SPIR-V module into the shader object
  // -> "glShaderBinary" is OpenGL 4.1
  {
    // Decode from SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
    // -> https://github.com/aras-p/smol-v
    // -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
    const size_t spirvOutputBufferSize = smolv::GetDecodedBufferSize(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes());
    RECore::uint8* spirvOutputBuffer = RHI_MALLOC_TYPED(context, RECore::uint8, spirvOutputBufferSize);
    smolv::Decode(shaderBytecode.getBytecode(), shaderBytecode.getNumberOfBytes(), spirvOutputBuffer, spirvOutputBufferSize);
    glShaderBinary(1, &openGLShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirvOutputBuffer, static_cast<GLsizei>(spirvOutputBufferSize));
    RHI_FREE(context, spirvOutputBuffer);
  }

  // Done
  return openGLShader;
}

GLuint Helper::loadShaderProgramFromBytecode(const RERHI::RHIContext &context, GLenum shaderType,
                                             const RERHI::ShaderBytecode &shaderBytecode) {
  // Create and load the shader object
  const GLuint openGLShader = loadShaderFromBytecode(context, shaderType, shaderBytecode);

  // Specialize the shader
  // -> Before this shader the isn't compiled, after this shader is supposed to be compiled
  glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

  // Check the compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled)
  {
    // All went fine, create and return the program
    const GLuint openGLProgram = glCreateProgram();
    glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(openGLProgram, openGLShader);
    glLinkProgram(openGLProgram);
    glDetachShader(openGLProgram, openGLShader);
    glDeleteShader(openGLShader);

    // Check the link status
    GLint linked = GL_FALSE;
    glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
    if (GL_TRUE != linked)
    {
      // Error, program link failed!
      printOpenGLProgramInformationIntoLog(context, openGLProgram, nullptr);
    }

    // Done
    return openGLProgram;
  }
  else
  {
    // Error, failed to compile the shader!
    printOpenGLShaderInformationIntoLog(context, openGLShader, nullptr);

    // Destroy the OpenGL shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0u;
  }
}

GLuint
Helper::loadShaderProgramFromSourceCode(const RERHI::RHIContext &context, GLenum shaderType, const GLchar *sourceCode) {
  // Create the shader program
  const GLuint openGLProgram = glCreateShaderProgramv(shaderType, 1, &sourceCode);

  // Check the link status
  GLint linked = GL_FALSE;
  glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
  if (GL_TRUE == linked)
  {
    // All went fine, return the program
    return openGLProgram;
  }
  else
  {
    // Error, failed to compile the shader!
    printOpenGLProgramInformationIntoLog(context, openGLProgram, sourceCode);

    // Destroy the program
    // -> A value of 0 for shader will be silently ignored
    glDeleteProgram(openGLProgram);

    // Error!
    return 0u;
  }
}

GLuint Helper::createShaderProgramObject(const RERHI::RHIContext &context, GLuint openGLShader,
                                         const RERHI::VertexAttributes &vertexAttributes) {
  if (openGLShader > 0)
  {
    // Create the OpenGL program
    const GLuint openGLProgram = glCreateProgram();
    if (openGLProgram > 0)
    {
      glProgramParameteri(openGLProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

      // Attach the shader to the program
      glAttachShader(openGLProgram, openGLShader);

      // Define the vertex array attribute binding locations ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 & 12 terminology)
      // -> Crucial code that glCreateShaderProgram doesn't do
      {
        const RECore::uint32 numberOfVertexAttributes = vertexAttributes.numberOfAttributes;
        for (RECore::uint32 vertexAttribute = 0; vertexAttribute < numberOfVertexAttributes; ++vertexAttribute)
        {
          glBindAttribLocation(openGLProgram, vertexAttribute, vertexAttributes.attributes[vertexAttribute].name);
        }
      }

      // Link the program
      glLinkProgram(openGLProgram);

      // Detach the shader from the program
      glDetachShader(openGLProgram, openGLShader);
    }

    // Destroy the OpenGL shader
    glDeleteShader(openGLShader);

    // Check the link status
    if (openGLProgram > 0)
    {
      GLint linked = GL_FALSE;
      glGetProgramiv(openGLProgram, GL_LINK_STATUS, &linked);
      if (GL_TRUE == linked)
      {
        // Done
        return openGLProgram;
      }
      else
      {
        // Error, program link failed!
        printOpenGLProgramInformationIntoLog(context, openGLProgram);
      }
    }
  }

  // Error!
  return 0;
}

GLuint
Helper::loadShaderProgramFromBytecode(const RERHI::RHIContext &context, const RERHI::VertexAttributes &vertexAttributes,
                                      GLenum shaderType, const RERHI::ShaderBytecode &shaderBytecode) {
  // Create and load the shader object
  const GLuint openGLShader = loadShaderFromBytecode(context, shaderType, shaderBytecode);

  // Specialize the shader
  // -> Before this shader the isn't compiled, after this shader is supposed to be compiled
  glSpecializeShaderARB(openGLShader, "main", 0, nullptr, nullptr);

  // Check the compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled)
  {
    // All went fine, create and return the program
    return createShaderProgramObject(context, openGLShader, vertexAttributes);
  }
  else
  {
    // Error, failed to compile the shader!
    printOpenGLShaderInformationIntoLog(context, openGLShader);

    // Destroy the OpenGL shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0;
  }
}

GLuint Helper::loadShaderFromSourcecode(const RERHI::RHIContext &context, GLenum shaderType, const GLchar *sourceCode) {
  // Create the shader object
  const GLuint openGLShader = glCreateShader(shaderType);

  // Load the shader source
  glShaderSource(openGLShader, 1, &sourceCode, nullptr);

  // Compile the shader
  glCompileShader(openGLShader);

  // Check the compile status
  GLint compiled = GL_FALSE;
  glGetShaderiv(openGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if (GL_TRUE == compiled)
  {
    // All went fine, return the shader
    return openGLShader;
  }
  else
  {
    // Error, failed to compile the shader!

    { // Get the length of the information
      GLint informationLength = 0;
      glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &informationLength);
      if (informationLength > 1)
      {
        // Allocate memory for the information
        GLchar* informationLog = RHI_MALLOC_TYPED(context, GLchar, informationLength);

        // Get the information
        glGetShaderInfoLog(openGLShader, informationLength, nullptr, informationLog);

        // Output the debug string
        RE_LOG(Critical, std::string(sourceCode))
        //if (context.getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), informationLog))
        {
          //	DEBUG_BREAK;
        }

        // Cleanup information memory
        RHI_FREE(context, informationLog);
      }
    }

    // Destroy the shader
    // -> A value of 0 for shader will be silently ignored
    glDeleteShader(openGLShader);

    // Error!
    return 0u;
  }
}

GLuint Helper::loadShaderProgramFromSourcecode(const RERHI::RHIContext &context,
                                               const RERHI::VertexAttributes &vertexAttributes, GLenum type,
                                               const char *sourceCode) {
  return createShaderProgramObject(context, loadShaderFromSourcecode(context, type, sourceCode), vertexAttributes);
}

#ifdef _MSC_VER
#pragma optimize("", off)
#endif
void
Helper::shaderSourceCodeToShaderBytecode(const RERHI::RHIContext &context, GLenum shaderType, const GLchar *sourceCode,
                                         RERHI::ShaderBytecode &shaderBytecode) {
#ifdef RHI_OPENGL_GLSLTOSPIRV
  // Initialize glslang, if necessary
				if (!GlslangInitialized)
				{
					glslang::InitializeProcess();
					GlslangInitialized = true;
				}

				// GLSL to intermediate
				// -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
				// TODO(naetherm) OpenGL GLSL 430 instead of 410 for e.g. "GL_ARB_shader_image_load_store" build in support. Apply dropped OpenGL support so we can probably drop Apple support. If at once point RacoonEngine should run on Apple hardware, we probably will use MoltenVK for Vulkan (yet another RHI API Metal just for Apple hardware is probably to much work for a spare time project).
				// const int glslVersion = 410;
				const int glslVersion = 430;
				EShLanguage shLanguage = EShLangCount;
				switch (shaderType)
				{
					case GL_VERTEX_SHADER_ARB:
						shLanguage = EShLangVertex;
						break;

					case GL_TESS_CONTROL_SHADER:
						shLanguage = EShLangTessControl;
						break;

					case GL_TESS_EVALUATION_SHADER:
						shLanguage = EShLangTessEvaluation;
						break;

					case GL_GEOMETRY_SHADER_ARB:
						shLanguage = EShLangGeometry;
						break;

					case GL_FRAGMENT_SHADER_ARB:
						shLanguage = EShLangFragment;
						break;

					case GL_COMPUTE_SHADER:
						shLanguage = EShLangCompute;
						break;
				}
				glslang::TShader shader(shLanguage);
				shader.setEnvInput(glslang::EShSourceGlsl, shLanguage, glslang::EShClientOpenGL, glslVersion);
				shader.setEntryPoint("main");
				{
					const char* sourcePointers[] = { sourceCode };
					shader.setStrings(sourcePointers, 1);
				}
				const EShMessages shMessages = static_cast<EShMessages>(EShMsgDefault);
				if (shader.parse(&DefaultTBuiltInResource, glslVersion, false, shMessages))
				{
					glslang::TProgram program;
					program.addShader(&shader);
					if (program.link(shMessages))
					{
						// Intermediate to SPIR-V
						const glslang::TIntermediate* intermediate = program.getIntermediate(shLanguage);
						if (nullptr != intermediate)
						{
							std::vector<unsigned int> spirv;
							glslang::GlslangToSpv(*intermediate, spirv);

							// Encode to SMOL-V: like Vulkan/Khronos SPIR-V, but smaller
							// -> https://github.com/aras-p/smol-v
							// -> http://aras-p.info/blog/2016/09/01/SPIR-V-Compression/
							// -> Don't apply "spv::spirvbin_t::remap()" or the SMOL-V result will be bigger
							smolv::ByteArray byteArray;
							smolv::Encode(spirv.data(), sizeof(unsigned int) * spirv.size(), byteArray, smolv::kEncodeFlagStripDebugInfo);

							// Done
							shaderBytecode.setBytecodeCopy(static_cast<RECore::uint32>(byteArray.size()), reinterpret_cast<RECore::uint8*>(byteArray.data()));
						}
					}
					else
					{
						// Failed to link the program
            RE_LOG(Critical, std::string(sourceCode))
						//if (context.getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), "Failed to link the GLSL program: %s", program.getInfoLog()))
						{
						//	DEBUG_BREAK;
						}
					}
				}
				else
				{
					// Failed to parse the shader source code
          RE_LOG(Critical, std::string(sourceCode))
					//if (context.getLog().print(RECore::ILog::Type::CRITICAL, sourceCode, __FILE__, static_cast<RECore::uint32>(__LINE__), "Failed to parse the GLSL shader source code: %s", shader.getInfoLog()))
					{
					//	DEBUG_BREAK;
					}
				}
#endif
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

void Helper::bindUniformBlock(const RERHI::DescriptorRange &descriptorRange, RECore::uint32 openGLProgram,
                              RECore::uint32 uniformBlockBindingIndex) {
  // Explicit binding points ("layout(binding = 0)" in GLSL shader) requires OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension,
  // for backward compatibility, ask for the uniform block index
  const GLuint uniformBlockIndex = glGetUniformBlockIndex(openGLProgram, descriptorRange.baseShaderRegisterName);
  if (GL_INVALID_INDEX != uniformBlockIndex)
  {
    // Associate the uniform block with the given binding point
    glUniformBlockBinding(openGLProgram, uniformBlockIndex, uniformBlockBindingIndex);
  }
}

void Helper::bindUniformLocation(const RERHI::DescriptorRange &descriptorRange, RECore::uint32 openGLProgramPipeline,
                                 RECore::uint32 openGLProgram) {
  const GLint uniformLocation = glGetUniformLocation(openGLProgram, descriptorRange.baseShaderRegisterName);
  if (uniformLocation >= 0)
  {
    // OpenGL/GLSL is not automatically assigning texture units to samplers, so, we have to take over this job
    // -> When using OpenGL or OpenGL ES 3 this is required
    // -> OpenGL 4.2 or the "GL_ARB_explicit_uniform_location"-extension supports explicit binding points ("layout(binding = 0)"
    //    in GLSL shader) , for backward compatibility we don't use it in here
    // -> When using Direct3D 9, 10, 11 or 12, the texture unit
    //    to use is usually defined directly within the shader by using the "register"-keyword
    // -> Use the "GL_ARB_direct_state_access" or "GL_EXT_direct_state_access" extension if possible to not change OpenGL states
    if (nullptr != glProgramUniform1i)
    {
      glProgramUniform1i(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
    }
    else if (nullptr != glProgramUniform1iEXT)
    {
      glProgramUniform1iEXT(openGLProgram, uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
    }
    else
    {
      // TODO(naetherm) There's room for binding API call related optimization in here (will certainly be no huge overall efficiency gain)
#ifdef RHI_OPENGL_STATE_CLEANUP
      // Backup the currently used OpenGL program
						GLint openGLProgramBackup = 0;
						glGetProgramPipelineiv(openGLProgramPipeline, GL_ACTIVE_PROGRAM, &openGLProgramBackup);
						if (static_cast<RECore::uint32>(openGLProgramBackup) == openGLProgram)
						{
							// Set uniform, please note that for this our program must be the currently used one
							glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
						}
						else
						{
							// Set uniform, please note that for this our program must be the currently used one
							glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
							glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));

							// Be polite and restore the previous used OpenGL program
							glActiveShaderProgram(openGLProgramPipeline, static_cast<GLuint>(openGLProgramBackup));
						}
#else
      glActiveShaderProgram(openGLProgramPipeline, openGLProgram);
      glUniform1i(uniformLocation, static_cast<GLint>(descriptorRange.baseShaderRegister));
#endif
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
