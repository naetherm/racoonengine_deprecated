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
#include "RERHINull/RERHINull.h"
#include <RERHI/Shader/RHIShaderLanguage.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null shader language class
*/
class ShaderLanguage final : public RERHI::RHIShaderLanguage
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] nullRhi
  *    Owner null RHI instance
  */
  explicit ShaderLanguage(RHIDynamicRHI& nullRhi);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~ShaderLanguage() override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIShaderLanguage methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual const char* getShaderLanguageName() const override;

  [[nodiscard]] virtual RERHI::RHIVertexShader* createVertexShaderFromBytecode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, [[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIVertexShader* createVertexShaderFromSourceCode([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, [[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationControlShader* createTessellationControlShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationControlShader* createTessellationControlShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationEvaluationShader* createTessellationEvaluationShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITessellationEvaluationShader* createTessellationEvaluationShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGeometryShader* createGeometryShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGeometryShader* createGeometryShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIFragmentShader* createFragmentShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIFragmentShader* createFragmentShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITaskShader* createTaskShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITaskShader* createTaskShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIMeshShader* createMeshShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIMeshShader* createMeshShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIComputeShader* createComputeShaderFromBytecode([[maybe_unused]] const RERHI::ShaderBytecode& shaderBytecode RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIComputeShader* createComputeShaderFromSourceCode([[maybe_unused]] const RERHI::ShaderSourceCode& shaderSourceCode, [[maybe_unused]] RERHI::ShaderBytecode* shaderBytecode = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGraphicsProgram* createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, [[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, RERHI::RHIVertexShader* vertexShader, RERHI::RHITessellationControlShader* tessellationControlShader, RERHI::RHITessellationEvaluationShader* tessellationEvaluationShader, RERHI::RHIGeometryShader* geometryShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIGraphicsProgram* createGraphicsProgram([[maybe_unused]] const RERHI::RHIRootSignature& rootSignature, RERHI::RHITaskShader* taskShader, RERHI::RHIMeshShader& meshShader, RERHI::RHIFragmentShader* fragmentShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ShaderLanguage(const ShaderLanguage& source) = delete;
  ShaderLanguage& operator =(const ShaderLanguage& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
