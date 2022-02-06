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
#include "RERenderer/RERenderer.h"
#include <RECore/Frontend/FrontendApplication.h>
#include <RECore/System/DynLib.h>
#include <RERHI/Rhi.h>
#include <RERenderer/Context.h>
#include <RERenderer/RendererImpl.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERHI {
class RhiInstance;
}
namespace RERenderer {
class Context;
class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class RendererApplication : public RECore::FrontendApplication {
  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def(RERENDERER_API)
  re_class_def_end

public:

  /**
   * @brief
   * Constructor.
   *
   * @param frontend
   */
  RendererApplication(RECore::Frontend& frontend);

  /**
   * @brief
   * Destructor
   */
  ~RendererApplication() override;


public:

  [[nodiscard]] RERHI::RHIDynamicRHI* getRhi() const;

  [[nodiscard]] RERHI::RHIRenderTarget* getMainRenderTarget() const;

  [[nodiscard]] RERenderer::IRenderer* getRenderer() const;

  [[nodiscard]] RERenderer::IRenderer& getRendererSafe() const;


protected:
  /**
  *  @brief
  *    Initialization function that is called prior to OnInit()
  *
  *  @return
  *    'true' if all went fine, else 'false' which will stop the application
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that PLCore::CoreApplication::OnStart() does
  *    - Call OnCreateRPIContext()
  *    - Call OnCreatePainter()
  *    - Return and go on with OnInit()
  */
  bool onStart() override;

  /**
  *  @brief
  *    De-initialization function that is called after OnDeInit()
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Save renderer related configuration
  *    - Destroy renderer context
  *    - Everything that FrontendApplication::OnStop() does
  */
  void onStop() override;


  //[-------------------------------------------------------]
  //[ Protected virtual CoreApplication functions           ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Called when application should initialize it's configuration
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that CoreApplication::OnInitConfig() does
  *    - Read frontend configuration and set frontend position and size of the previous session
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  void onInitConfig() override;

  /**
  *  @brief
  *    Called when application should set it's data paths
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Set '.' as base path in LoadableManager
  *    - Scan for packages in "Data/" directory
  *    - Set application directory as base path in LoadableManager
  *    - Scan for packages in application directory "Data/" directory
  *    - Get current language and load RacoonEngine localization file, if no language is defined, English is used as default
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  RECORE_API void onInitData() override;

protected:
  /**
  *  @brief
  *    Called when the window size has been changed
  *
  *  @note
  *    - The default implementation is empty
  */
  void onSize() override;

  /**
  *  @brief
  *    Called when the fullscreen mode was changed
  *
  *  @remarks
  *    This method just says "something related to fullscreen mode has been changed". Whether we
  *    changed from window mode into fullscreen mode or changed e.g. the resolution used in
  *    fullscreen mode is not really interesting in here.
  *
  *  @note
  *    - The default implementation is empty
  */
  void onFullscreenMode() override;

  /**
  *  @brief
  *    Called to let the frontend draw into it's window
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that PLCore::FrontendApplication::OnDraw() does
  *    - Draw renderer surface
  */
  void onDraw() override;

  /**
  *  @brief
  *    Called to let the frontend update it's states
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Everything that PLCore::FrontendApplication::OnUpdate() does
  *    - Update renderer context
  */
  void onUpdate() override;

protected:

  virtual void drawInternal(RERHI::RHICommandBuffer& commandBuffer);

private:
/**
*  @brief
*    Function that is called to Create the application's renderer context
*
*  @note
*    - Part of the application framework initialization function "onStart()"
*/
  virtual void onCreateRendererContext();

protected:
  RECore::String			   mRhiName;	///< Case sensitive ASCII name of the RHI to instance
  RECore::IFileManager*		mFileManager;		///< File manager instance, can be a null pointer
  RERHI::RHIContext*	   mRhiContext;		///< RHI context, can be a null pointer
  RECore::String mSharedLibraryName;
  RECore::DynLib  mRhiSharedLibrary;
  RERHI::RHIDynamicRHI*		   mRhi;			///< RHI instance, can be a null pointer, do not destroy the instance
  RERHI::RHISwapChain*   mMainSwapChain;	///< Main swap chain instance, can be a null pointer, release the instance if you no longer need it
  RERHI::RHICommandBuffer mCommandBuffer;	///< Command buffer
  RERenderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
  RERenderer::IRenderer* mRenderer;	///< Renderer instance, can be a null pointer

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERenderer
