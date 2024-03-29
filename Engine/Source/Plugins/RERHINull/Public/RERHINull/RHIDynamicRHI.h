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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHINull/RERHINull.h"
#include <RERHI/RHIDynamicRHI.h>


extern "C" RERHINULL_API RERHI::RHIDynamicRHI* createNullRhiInstance(const RERHI::RHIContext& context);
RERHINULL_API RERHI::RHIDynamicRHI* createRhiInstance(const RERHI::RHIContext& context);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


class RootSignature;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null RHI class
*/
class RHIDynamicRHI final : public RERHI::RHIDynamicRHI
{


  //[-------------------------------------------------------]
  //[ Public data                                           ]
  //[-------------------------------------------------------]
public:
  RECore::MakeIdUInt16 VertexArrayMakeId;
  RECore::MakeIdUInt16 GraphicsPipelineStateMakeId;
  RECore::MakeIdUInt16 ComputePipelineStateMakeId;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] context
  *    RHI context, the RHI context instance must stay valid as long as the RHI instance exists
  *
  *  @note
  *    - Do never ever use a not properly initialized RHI. Use "RERHI::isInitialized()" to check the initialization state.
  */
  explicit RHIDynamicRHI(const RERHI::RHIContext& context);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~RHIDynamicRHI() override;

  void dispatchCommandBufferInternal(const RERHI::RHICommandBuffer& commandBuffer);

  //[-------------------------------------------------------]
  //[ Graphics                                              ]
  //[-------------------------------------------------------]
  void setGraphicsRootSignature(RERHI::RHIRootSignature* rootSignature);
  void setGraphicsPipelineState(RERHI::RHIGraphicsPipelineState* graphicsPipelineState);
  void setGraphicsResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup);
  void setGraphicsVertexArray(RERHI::RHIVertexArray* vertexArray);															// Input-assembler (IA) stage
  void setGraphicsViewports(RECore::uint32 numberOfViewports, const RERHI::Viewport* viewports);									// Rasterizer (RS) stage
  void setGraphicsScissorRectangles(RECore::uint32 numberOfScissorRectangles, const RERHI::ScissorRectangle* scissorRectangles);	// Rasterizer (RS) stage
  void setGraphicsRenderTarget(RERHI::RHIRenderTarget* renderTarget);															// Output-merger (OM) stage
  void clearGraphics(RECore::uint32 clearFlags, const float color[4], float z, RECore::uint32 stencil);
  void drawGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawIndexedGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawMeshTasksEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  //[-------------------------------------------------------]
  //[ Compute                                               ]
  //[-------------------------------------------------------]
  void setComputeRootSignature(RERHI::RHIRootSignature* rootSignature);
  void setComputePipelineState(RERHI::RHIComputePipelineState* computePipelineState);
  void setComputeResourceGroup(RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup);
  void dispatchCompute(RECore::uint32 groupCountX, RECore::uint32 groupCountY, RECore::uint32 groupCountZ);
  //[-------------------------------------------------------]
  //[ Resource                                              ]
  //[-------------------------------------------------------]
  void resolveMultisampleFramebuffer(RERHI::RHIRenderTarget& destinationRenderTarget, RERHI::RHIFramebuffer& sourceMultisampleFramebuffer);
  void copyResource(RERHI::RHIResource& destinationResource, RERHI::RHIResource& sourceResource);
  void generateMipmaps(RERHI::RHIResource& resource);
  //[-------------------------------------------------------]
  //[ Query                                                 ]
  //[-------------------------------------------------------]
  void resetQueryPool(RERHI::RHIQueryPool& queryPool, RECore::uint32 firstQueryIndex, RECore::uint32 numberOfQueries);
  void beginQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex, RECore::uint32 queryControlFlags);
  void endQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex);
  void writeTimestampQuery(RERHI::RHIQueryPool& queryPool, RECore::uint32 queryIndex);
  //[-------------------------------------------------------]
  //[ Debug                                                 ]
  //[-------------------------------------------------------]
#ifdef DEBUG
  void setDebugMarker(const char* name);
			void beginDebugEvent(const char* name);
			void endDebugEvent();
#endif


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIDynamicRHI methods                      ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual const char* getName() const override;
  [[nodiscard]] virtual bool isInitialized() const override;
  [[nodiscard]] virtual bool isDebugEnabled() override;
  //[-------------------------------------------------------]
  //[ Shader language                                       ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual RECore::uint32 getNumberOfShaderLanguages() const override;
  [[nodiscard]] virtual const char* getShaderLanguageName(RECore::uint32 index) const override;
  [[nodiscard]] virtual RERHI::RHIShaderLanguage* getShaderLanguage(const char* shaderLanguageName = nullptr) override;
  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual RERHI::RHIRenderPass* createRenderPass(RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat = RERHI::TextureFormat::UNKNOWN, RECore::uint8 numberOfMultisamples = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHIQueryPool* createQueryPool(RERHI::QueryType queryType, RECore::uint32 numberOfQueries = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHISwapChain* createSwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, bool useExternalContext = false RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHIFramebuffer* createFramebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHIBufferManager* createBufferManager() override;
  [[nodiscard]] virtual RERHI::RHITextureManager* createTextureManager() override;
  [[nodiscard]] virtual RERHI::RHIRootSignature* createRootSignature(const RERHI::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHIGraphicsPipelineState* createGraphicsPipelineState(const RERHI::GraphicsPipelineState& graphicsPipelineState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHIComputePipelineState* createComputePipelineState(RERHI::RHIRootSignature& rootSignature, RERHI::RHIComputeShader& computeShader RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  [[nodiscard]] virtual RERHI::RHISamplerState* createSamplerState(const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;
  //[-------------------------------------------------------]
  //[ Resource handling                                     ]
  //[-------------------------------------------------------]
  [[nodiscard]] virtual bool map(RERHI::RHIResource& resource, RECore::uint32 subresource, RERHI::MapType mapType, RECore::uint32 mapFlags, RERHI::MappedSubresource& mappedSubresource) override;
  virtual void unmap(RERHI::RHIResource& resource, RECore::uint32 subresource) override;
  [[nodiscard]] virtual bool getQueryPoolResults(RERHI::RHIQueryPool& queryPool, RECore::uint32 numberOfDataBytes, RECore::uint8* data, RECore::uint32 firstQueryIndex = 0, RECore::uint32 numberOfQueries = 1, RECore::uint32 strideInBytes = 0, RECore::uint32 queryResultFlags = 0) override;
  //[-------------------------------------------------------]
  //[ Operation                                             ]
  //[-------------------------------------------------------]
  virtual void dispatchCommandBuffer(const RERHI::RHICommandBuffer& commandBuffer) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RHIDynamicRHI(const RHIDynamicRHI& source) = delete;
  RHIDynamicRHI& operator =(const RHIDynamicRHI& source) = delete;

  /**
  *  @brief
  *    Initialize the capabilities
  */
  void initializeCapabilities();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RERHI::RHIShaderLanguage* mShaderLanguage;			///< Null shader language instance (we keep a reference to it), can be a null pointer
  RERHI::RHIRenderTarget*   mRenderTarget;			///< Currently set render target (we keep a reference to it), can be a null pointer
  RootSignature*		  mGraphicsRootSignature;	///< Currently set graphics root signature (we keep a reference to it), can be a null pointer
  RootSignature*		  mComputeRootSignature;	///< Currently set compute root signature (we keep a reference to it), can be a null pointer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
