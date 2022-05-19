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
#include <RECore/String/String.h>
#include <map>
#include <vector>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiImpl;
class Screen;
class GuiContext;
class GuiMessage;
class MainWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Gui
 *
 * @brief
 * Platform independent Gui implementation.
 */
class Gui {

  friend class GuiApplication;

public:

  static Gui& instance();

public:

  /**
   * @brief
   * Constructor.
   */
  REGUI_API Gui();

  /**
   * @brief
   * Destructor.
   */
  REGUI_API ~Gui();


  /**
   * @brief
   * Get platform dependent gui implementation.
   *
   * @return
   * Platform dependent gui implementation.
   */
  [[nodiscard]] GuiImpl* getImpl() const;

  [[nodiscard]] GuiContext* getGuiContext() const;

  [[nodiscard]] bool isActive() const;


  void update();

  //[-------------------------------------------------------]
  //[ Message processing                                    ]
  //[-------------------------------------------------------]
  void processMessages();


  //[-------------------------------------------------------]
  //[ Windows                                               ]
  //[-------------------------------------------------------]
  void addWindow(MainWindow* nativeWindow);

  void removeWindow(MainWindow* nativeWindow);

  [[nodiscard]] MainWindow* getWindow(RECore::handle nativeWindowHandle) const;

  MainWindow* getMainWindow() const;


  //[-------------------------------------------------------]
  //[ Gui Messages                                          ]
  //[-------------------------------------------------------]
  void sendMessage(const GuiMessage& guiMessage);


  //[-------------------------------------------------------]
  //[ Screens                                               ]
  //[-------------------------------------------------------]
  /**
   * @brief
   * Gets vector of all screens/monitors.
   *
   * @return
   * Vector of screens.
   */
  REGUI_API const std::vector<Screen*>& getScreens() const;

  /**
   * @brief
   * Gets pointer to screen by its name.
   *
   * @param[in] name
   * Name of the screen.
   *
   * @return
   * Pointer to screen, this can be a nullptr.
   */
  REGUI_API Screen* getScreen(const RECore::String& name) const;

  /**
   * @brief
   * Gets pointer to the default screen.
   *
   * @return
   * Pointer to the default screen.
   */
  REGUI_API Screen* getDefaultScreen() const;

protected:

  /**
   * @brief
   * Responsible for initializing the gui.
   *
   * @param[in] guiContext
   * Pointer to the gui context.
   */
  void initialize(GuiContext* guiContext);

private:

  void createFixedBuildInRhiConfigurationResources();

  const RERHI::RHIVertexArrayPtr& getFillVertexArrayPtr(RERHI::RHICommandBuffer* commandBuffer);

  void fillGraphicsCommandBuffer(RERHI::RHICommandBuffer& commandBuffer);

  void fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(RERHI::RHICommandBuffer& commandBuffer);

  RERHI::RHIResourceGroupPtr getResourceGroupByTexture(RERHI::RHITexture2D* texture);

protected:
  /** Platform dependent gui implementation */
  GuiImpl* mpImpl;
  /** Pointer to the gui context, always valid! */
  GuiContext* mGuiContext;

  ImGuiContext*	   mImGuiContext;
  /** List of available screens/monitors */
  std::vector<Screen*> mScreens;
  /** Pointer to the default screen, always valid! */
  Screen* mDefaultScreen;
  /** Map containing all active windows */
  std::map<RECore::handle, MainWindow*> mWindows;
  /** Pointer to main window */
  MainWindow* mMainWindow;


  RERHI::RHICommandBuffer mCommandBuffer;

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
