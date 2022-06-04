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
#include "REGui/REGui.h"
#include <RERenderer/IRenderer.h>
#include <RERenderer/Context.h>
#include <imgui.h>
#include <map>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class GuiRendererResources;
class GuiTextureManager;
class MainWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiRenderer {
public:

  GuiRenderer(Gui* gui);

  virtual ~GuiRenderer();



  void startup(RECore::handle windowHandle);

  void update();


  [[nodiscard]] RERHI::RHIContext* getRhiContext() const;

  [[nodiscard]] RERHI::RHIDynamicRHI* getRhi() const;

  [[nodiscard]] RERenderer::Context* getRendererContext() const;

  [[nodiscard]] RERenderer::IRenderer* getRenderer() const;

  [[nodiscard]] GuiRendererResources* getGuiRendererResources() const;

  [[nodiscard]] GuiTextureManager* getGuiTextureManager() const;


  void startFrame(MainWindow* mainWindow);

  void endFrame();

public:

  [[nodiscard]] const RERHI::RHIVertexArrayPtr& getFillVertexArrayPtr(RERHI::RHICommandBuffer* commandBuffer = nullptr);
  void fillGraphicsCommandBuffer(RERHI::RHICommandBuffer& commandBuffer);
  void fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(RERHI::RHICommandBuffer& commandBuffer, RERHI::RHITexture2D* tex = nullptr);


  void registerTexture(RERHI::RHITexture2D* texture);

  RERHI::RHIResourceGroupPtr getResourceGroupByTexture(RERHI::RHITexture2D* texture) {
    auto res = this->mTextures.find(texture);
    if (res != this->mTextures.end()) {
      return res->second;
    }
    return this->mTextures.find(this->mDefaultFontTexture)->second;
  }
private:

  void createFixedBuildInRhiConfigurationResources();

  void initializeKeyMap();

protected:
  /** Pointer to platform independent gui implementation, must be valid */
  Gui* mGui;
  /** Pointer to rhi context, must be valid */
  RERHI::RHIContext* mRhiContext;
  /** Pointer to rhi implementation, must be valid */
  RERHI::RHIDynamicRHI* mRhi;
  /** Pointer to renderer context,must be valid! */
  RERenderer::Context* mRendererContext;
  /** Pointer to renderer implementation, must be valid! */
  RERenderer::IRenderer* mRenderer;

  /** Pointer to internal used general rendering resource management */
  GuiRendererResources* mGuiRendererResources;
  /** Pointer to internal used texture management implementation */
  GuiTextureManager* mGuiTextureManager;


  // Collected resources
  std::map<RERHI::RHITexture2D*, RERHI::RHIResourceGroupPtr> mTextures;

  // Fixed builtin RHI Configuration resources
  /** Pointer to the default font texture for imgui */
  RERHI::RHITexture2DPtr mDefaultFontTexture;
  RERHI::RHIRootSignaturePtr mRootSignature;
  RERHI::RHIGraphicsProgramPtr mGraphicsProgram;
  RERHI::RHIGraphicsPipelineStatePtr mGraphicsPipelineState;
  RERHI::RHIUniformBufferPtr mVertexShaderUniformBuffer;
  RECore::handle mObjectSpaceToClipSpaceMatrixUniformHandle;
  RERHI::RHIResourceGroupPtr mResourceGroup;
  RERHI::RHIResourceGroupPtr mSamplerStateGroup;
  // Vertex and index buffers
  RERHI::RHIVertexBufferPtr mVertexBuffer;
  RECore::uint32 mNumberOfAllocatedVertices;
  RERHI::RHIIndexBufferPtr mIndexBuffer;
  RECore::uint32 mNumberOfAllocatedIndices;
  RERHI::RHIVertexArrayPtr mVertexArray;


  RECore::uint64 mTime;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
