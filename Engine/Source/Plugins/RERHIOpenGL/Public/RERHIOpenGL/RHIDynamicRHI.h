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
#include "RERHIOpenGL/RERHIOpenGL.h"
#include <RERHI/RHIDynamicRHI.h>

// RERHIOPENGL_API extern "C"
RERHIOPENGL_API RERHI::RHIDynamicRHI* createOpenGLRhiInstance(const RERHI::RHIContext& context);
extern "C"  RERHIOPENGL_API RERHI::RHIDynamicRHI* createRhiInstance(const RERHI::RHIContext& context);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class OpenGLRuntimeLinking;
class RHIOpenGLContext;
class Extensions;
class RootSignature;
class GraphicsPipelineState;
class ComputePipelineState;
class VertexArray;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL RHI class
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
  *    Return the OpenGL context instance
  *
  *  @return
  *    The OpenGL context instance, do not free the memory the reference is pointing to
  */
  [[nodiscard]] inline const RHIOpenGLContext& getOpenGLContext() const
  {
    return *mOpenGLContext;
  }

  /**
  *  @brief
  *    Return the available extensions
  *
  *  @return
  *    The available extensions, do not free the memory the reference is pointing to
  */
  [[nodiscard]] inline const Extensions& getExtensions() const
  {
    return *mExtensions;
  }

  /**
  *  @brief
  *    Return the available extensions
  *
  *  @return
  *    The available extensions, do not free the memory the reference is pointing to
  */
  [[nodiscard]] inline Extensions& getExtensions()
  {
    return *mExtensions;
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
  void drawGraphics(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawIndexedGraphics(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawIndexedGraphicsEmulated(const RECore::uint8* emulationData, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
  void drawMeshTasks(const RERHI::RHIIndirectBuffer& indirectBuffer, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1);
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
  *    Debug message callback function called by the "GL_ARB_debug_output"-extension
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
  static void CALLBACK debugMessageCallback(RECore::uint32 source, RECore::uint32 type, RECore::uint32 id, RECore::uint32 severity, int length, const char* message, const void* userParam);


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
  *    Unset the currently used vertex array
  */
  void unsetGraphicsVertexArray();

  /**
  *  @brief
  *    Set resource group, method used by graphics and compute
  *
  *  @param[in] rootSignature
  *    Used root signature
  *  @param[in] rootParameterIndex
  *    Root parameter index
  *  @param[in] resourceGroup
  *    Resource group to set, can be a null pointer
  */
  void setResourceGroup(const RootSignature& rootSignature, RECore::uint32 rootParameterIndex, RERHI::RHIResourceGroup* resourceGroup);

  /**
  *  @brief
  *    Set OpenGL graphics program
  *
  *  @param[in] graphicsProgram
  *    Graphics program to set
  */
  void setOpenGLGraphicsProgram(RERHI::RHIGraphicsProgram* graphicsProgram);

  /**
  *  @brief
  *    Set OpenGL compute pipeline state
  *
  *  @param[in] computePipelineState
  *    Compute pipeline state to set
  */
  void setOpenGLComputePipelineState(ComputePipelineState* computePipelineState);

  /**
  *  @brief
  *    Update "GL_ARB_base_instance" emulation
  *
  *  @param[in] startInstanceLocation
  *    Start instance location
  */
  void updateGL_ARB_base_instanceEmulation(RECore::uint32 startInstanceLocation);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  OpenGLRuntimeLinking* mOpenGLRuntimeLinking;			///< OpenGL runtime linking instance, always valid
  RHIOpenGLContext*		  mOpenGLContext;					///< OpenGL context instance, always valid
  Extensions*			  mExtensions;						///< Extensions instance, always valid
  RERHI::RHIShaderLanguage* mShaderLanguage;					///< Shader language instance (we keep a reference to it), can be a null pointer
  RootSignature*		  mGraphicsRootSignature;			///< Currently set graphics root signature (we keep a reference to it), can be a null pointer
  RootSignature*		  mComputeRootSignature;			///< Currently set compute root signature (we keep a reference to it), can be a null pointer
  RERHI::RHISamplerState*   mDefaultSamplerState;				///< Default rasterizer state (we keep a reference to it), can be a null pointer
  GLuint				  mOpenGLCopyResourceFramebuffer;	///< OpenGL framebuffer ("container" object, not shared between OpenGL contexts) used by "OpenGLRhi::OpenGLRhi::copyResource()" if the "GL_ARB_copy_image"-extension isn't available, can be zero if no resource is allocated
  GLuint				  mDefaultOpenGLVertexArray;		///< Default OpenGL vertex array ("container" object, not shared between OpenGL contexts) to enable attribute-less rendering, can be zero if no resource is allocated
  // States
  GraphicsPipelineState* mGraphicsPipelineState;	///< Currently set graphics pipeline state (we keep a reference to it), can be a null pointer
  ComputePipelineState*  mComputePipelineState;	///< Currently set compute pipeline state (we keep a reference to it), can be a null pointer
  // Input-assembler (IA) stage
  VertexArray* mVertexArray;				///< Currently set vertex array (we keep a reference to it), can be a null pointer
  GLenum		 mOpenGLPrimitiveTopology;	///< OpenGL primitive topology describing the type of primitive to render
  GLint		 mNumberOfVerticesPerPatch;	///< Number of vertices per patch
  // Output-merger (OM) stage
  RERHI::RHIRenderTarget* mRenderTarget;	///< Currently set render target (we keep a reference to it), can be a null pointer
  // State cache to avoid making redundant OpenGL calls
  GLenum mOpenGLClipControlOrigin;	///< Currently set OpenGL clip control origin
  GLuint mOpenGLProgramPipeline;		///< Currently set OpenGL program pipeline, can be zero if no resource is set
  GLuint mOpenGLProgram;				///< Currently set OpenGL program, can be zero if no resource is set
  GLuint mOpenGLIndirectBuffer;		///< Currently set OpenGL indirect buffer, can be zero if no resource is set
  // Draw ID uniform location for "GL_ARB_base_instance"-emulation (see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html)
  GLuint	 mOpenGLVertexProgram;			///< Currently set OpenGL vertex program, can be zero if no resource is set
  GLint	 mDrawIdUniformLocation;		///< Draw ID uniform location
  RECore::uint32 mCurrentStartInstanceLocation;	///< Currently set start instance location


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
