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
#include <RECore/Frontend/FrontendApplication.h>
#include <RECore/System/DynLib.h>
#include <RERHI/Rhi.h>
#include <RERenderer/Context.h>
#include <RERenderer/RendererImpl.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ExampleBase;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Application : public RECore::FrontendApplication {

  friend class ExampleBase;

  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def()
  re_class_def_end

public:

  /**
   * @brief
   * Constructor.
   *
   * @param frontend
   */
  Application(RECore::Frontend& frontend);

  /**
   * @brief
   * Destructor
   */
  ~Application() override;


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
*    Called to let the frontend update it's states
*
*  @remarks
*    The default implementation does the following tasks:
*    - Everything that PLCore::FrontendApplication::OnUpdate() does
*    - Update renderer context
*/
  void onUpdate() override;


  void onDraw() override;

private:

  void main() override;

protected:
/**
*  @brief
*    Function that is called to Create the application's renderer context
*
*  @note
*    - Part of the application framework initialization function "onStart()"
*/
  virtual void onCreateRendererContext();

private:
/** Pointer to the dynamic library, the dynamic rhi was loaded from. This is always valid! */
  RECore::DynLib  mRhiSharedLibrary;
/** The name of the library to load */
  RECore::String msLibraryName;
  RERHI::RHIContext *mpRHIContext;
  RERHI::RHIDynamicRHI*		   mRhi;			///< RHI instance, can be a null pointer, do not destroy the instance
  RERHI::RHISwapChain*   mMainSwapChain;	///< Main swap chain instance, can be a null pointer, release the instance if you no longer need it
  RERHI::RHICommandBuffer mCommandBuffer;	///< Command buffer
  RERenderer::Context*			mRendererContext;	///< Renderer context instance, can be a null pointer
  RERenderer::IRenderer* mRenderer;	///< Renderer instance, can be a null pointer

  ExampleBase* mExampleBase;
};