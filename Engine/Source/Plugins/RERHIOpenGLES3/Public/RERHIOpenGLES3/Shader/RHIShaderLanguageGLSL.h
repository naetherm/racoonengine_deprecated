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


/**
*  @brief
*    GLSL shader language class
*/
class ShaderLanguageGlsl final : public RERHI::RHIShaderLanguage
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
  */
  explicit ShaderLanguageGlsl(RHIDynamicRHI& openGLES3Rhi);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~ShaderLanguageGlsl() override
  {}


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIShaderLanguage methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual const char* getShaderLanguageName() const override;

  [[nodiscard]] virtual RERHI::RHIVertexShader* createVertexShaderFromBytecode(const RERHI::VertexAttributes&, const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIVertexShader* createVertexShaderFromSourceCode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationControlShader* createTessellationControlShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationControlShader* createTessellationControlShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGeometryShader* createGeometryShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGeometryShader* createGeometryShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIFragmentShader* createFragmentShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIFragmentShader* createFragmentShaderFromSourceCode(const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITaskShader* createTaskShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITaskShader* createTaskShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* = nullptr RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIMeshShader* createMeshShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIMeshShader* createMeshShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* = nullptr RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIComputeShader* createComputeShaderFromBytecode(const RERHI::ShaderBytecode& RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIComputeShader* createComputeShaderFromSourceCode(const RERHI::ShaderSourceCode&, RERHI::ShaderBytecode* = nullptr RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGraphicsProgram* createGraphicsProgram(const RERHI::RHIRootSignature& rootSignature, const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, [[maybe_unused]] RERHI::RHITessellationControlShader* tessellationControlShader, [[maybe_unused]] RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, [[maybe_unused]] RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGraphicsProgram* createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, [[maybe_unused]] RERHI::RHITaskShader* taskShader, [[maybe_unused]] RERHI::RHIMeshShader& meshShader, [[maybe_unused]] RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_MAYBE_UNUSED_PARAMETER);


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), ShaderLanguageGlsl, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ShaderLanguageGlsl(const ShaderLanguageGlsl& source) = delete;
  ShaderLanguageGlsl& operator =(const ShaderLanguageGlsl& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
