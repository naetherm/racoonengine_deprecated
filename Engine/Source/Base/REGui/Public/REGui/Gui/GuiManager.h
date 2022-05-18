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
#include <RECore/Core/Manager.h>
#include <RECore/String/StringId.h>

#include <vector>
#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct ImGuiContext;
namespace RERenderer {
class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
///< Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
typedef RECore::StringId			 AssetId;
typedef std::vector<AssetId> AssetIds;


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiManagerImpl;
class Theme;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiManager
 *
 * @brief
 * Platform independent gui manager.
 */
class GuiManager : public RECore::Manager {
public:

  /**
   *  @brief
   *    Set ImGui allocator functions
   *
   *  @param[in] allocator
   *    RHI allocator to use
   */
  static void setImGuiAllocatorFunctions();

  /**
   *  @brief
   *    Return the asset IDs of automatically generated dynamic default texture assets
   *
   *  @param[out] assetIds
   *    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
   *
   *  @remarks
   *    The debug GUI manager automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
   *    - "RacoonEngine/Texture/DynamicByCode/ImGuiGlyphMap2D"
   */
  static void getDefaultTextureAssetIds(AssetIds& assetIds);

public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] renderer
   * The renderer instance to use for rendering.
   */
  GuiManager(RERenderer::IRenderer& renderer);

  /**
   * @brief
   * Destructor.
   */
  ~GuiManager();


  void startUp();

  void initializeImGuiKeyMap();

  void onNewFrame(RERHI::RHIRenderTarget& renderTarget);


  [[nodiscard]] const RERHI::RHIVertexArrayPtr& getFillVertexArrayPtr(RERHI::RHICommandBuffer* commandBuffer = nullptr);

  void fillGraphicsCommandBuffer(RERHI::RHICommandBuffer& commandBuffer);

  void fillGraphicsCommandBufferUsingFixedBuiltinRhiConfiguration(RERHI::RHICommandBuffer& commandBuffer);


  void registerTexture(RERHI::RHITexture2D* texture);

  [[nodiscard]] RERHI::RHIResourceGroupPtr getResourceGroupByTexture(RERHI::RHITexture2D* texture);

public:

  void onWindowResize(RECore::uint32 width, RECore::uint32 height);

  void onKeyInput(RECore::uint32 keySym, char character, bool pressed);

  void onMouseMoveInput(int x, int y);

  void onMouseButtonInput(RECore::uint32 button, bool pressed);

  void onMouseWheelInput(bool scrollUp);
  
protected:

  void createFixedBuiltinRhiConfigurationResources();

protected:
  /** Renderer instance, this is always valid! */
  RERenderer::IRenderer& mRenderer;
  /** Pointer to the internally used platform dependent gui manager */
  GuiManagerImpl* mImpl;
  /** Pointer to the imgui context, always valid! */
  ImGuiContext* mImGuiContext;
  /** Pointer to the theme to use, always valid! */
  Theme* mTheme;

  bool mIsRunning;

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
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
