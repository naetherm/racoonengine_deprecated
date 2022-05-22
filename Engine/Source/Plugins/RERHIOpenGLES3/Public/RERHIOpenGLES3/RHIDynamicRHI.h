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

// RERHIVULKAN_API extern "C"
RERHI::RHIDynamicRHI* createOpenGLES3RhiInstance(const RERHI::RHIContext& context);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class IOpenGLES3Context;
class GraphicsPipelineState;
class RootSignature;
class VertexArray;

/**
*  @brief
*    OpenGL ES 3 RHI class
*/
class RHIDynamicRHI final : public RERHI::RHIDynamicRHI
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class GraphicsPipelineState;


  //[-------------------------------------------------------]
  //[ Public data                                           ]
  //[-------------------------------------------------------]
public:
  RECore::MakeIdUInt16 VertexArrayMakeId;
  RECore::MakeIdUInt16 GraphicsPipelineStateMakeId;


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
  *    - Do never ever use a not properly initialized RHI. Use "RERHI::RHIRERHI::isInitialized()" to check the initialization state.
  */
  explicit RHIDynamicRHI(const RERHI::RHIContext& context);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~RHIDynamicRHI() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 context instance
  *
  *  @return
  *    The OpenGL ES 3 context instance, do not free the memory the reference is pointing to
  */
  [[nodiscard]] inline IOpenGLES3Context& getOpenGLES3Context() const
  {
    return *mOpenGLES3Context;
  }

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
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(mContext, RHIDynamicRHI, this);
  }


  //[-------------------------------------------------------]
  //[ Private static methods                                ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Debug message callback function called by the "GL_KHR_debug"-extension
  *
  *  @param[in] source
  *    Source of the debug message
  *  @param[in] type
  *    Type of the debug message
  *  @param[in] id
  *    ID of the debug message
  *  @param[in] severity
  *    Severity of the debug message
  *  @param[in] length
  *    Length of the debug message
  *  @param[in] message
  *    The debug message
  *  @param[in] userParam
  *    Additional user parameter of the debug message
  */
  static void GL_APIENTRY debugMessageCallback(RECore::uint32 source, RECore::uint32 type, RECore::uint32 id, RECore::uint32 severity, int length, const char* message, const void* userParam);


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

  /**
  *  @brief
  *    Set graphics program
  *
  *  @param[in] graphicsProgram
  *    Graphics program to set
  */
  void setGraphicsProgram(RERHI::RHIGraphicsProgram* graphicsProgram);

  /**
  *  @brief
  *    Update "GL_EXT_base_instance" emulation
  *
  *  @param[in] startInstanceLocation
  *    Start instance location
  */
  void updateGL_EXT_base_instanceEmulation(RECore::uint32 startInstanceLocation);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  IOpenGLES3Context*	  mOpenGLES3Context;					///< OpenGL ES 3 context instance, always valid
  RERHI::RHIShaderLanguage* mShaderLanguageGlsl;					///< GLSL shader language instance (we keep a reference to it), can be a null pointer
  RootSignature*		  mGraphicsRootSignature;				///< Currently set graphics root signature (we keep a reference to it), can be a null pointer
  RERHI::RHISamplerState*   mDefaultSamplerState;					///< Default rasterizer state (we keep a reference to it), can be a null pointer
  GLuint				  mOpenGLES3CopyResourceFramebuffer;	///< OpenGL ES 3 framebuffer ("container" object, not shared between OpenGL ES 3 contexts) used by "RHIDynamicRHI::RHIDynamicRHI::copyResource()", can be zero if no resource is allocated
  GLuint				  mDefaultOpenGLES3VertexArray;			///< Default OpenGL ES 3 vertex array ("container" object, not shared between OpenGL contexts) to enable attribute-less rendering, can be zero if no resource is allocated
  // States
  GraphicsPipelineState* mGraphicsPipelineState;	///< Currently set graphics pipeline state (we keep a reference to it), can be a null pointer
  // Input-assembler (IA) stage
  VertexArray* mVertexArray;					///< Currently set vertex array (we keep a reference to it), can be a null pointer
  GLenum		 mOpenGLES3PrimitiveTopology;	///< OpenGL ES 3 primitive topology describing the type of primitive to render
  // Output-merger (OM) stage
  RERHI::RHIRenderTarget* mRenderTarget;	///< Currently set render target (we keep a reference to it), can be a null pointer
  // State cache to avoid making redundant OpenGL ES 3 calls
  GLenum	 mOpenGLES3ClipControlOrigin;	///< Currently set OpenGL ES 3 clip control origin
  GLuint	 mOpenGLES3Program;				///< Currently set OpenGL ES 3 program, can be zero if no resource is set
  // Draw ID uniform location for "GL_EXT_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
  GLint	 mDrawIdUniformLocation;		///< Draw ID uniform location
  RECore::uint32 mCurrentStartInstanceLocation;	///< Currently set start instance location


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
