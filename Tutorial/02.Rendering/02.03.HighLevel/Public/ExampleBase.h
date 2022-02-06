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
#include <RECore/Log/Log.h>
#include <RECore/String/String.h>
#include <RERenderer/Application/RendererApplication.h>


class Application;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Example base class
*/
class ExampleBase {
public:

  /**
   * @brief
   * Constructor.
   */
  inline ExampleBase(Application& cApplication, const RECore::String& name)
    : mApplication(&cApplication)
    , mExampleName(name) {

  }

  /**
   * @brief
   * Destructor.
   */
  inline virtual ~ExampleBase() {

  }

  [[nodiscard]] const RECore::String& getExampleName() const {
    return mExampleName;
  }

  inline void draw(RERHI::RHICommandBuffer& commandBuffer) {
    this->onDraw(commandBuffer);
  }

  [[nodiscard]] RERHI::RHIDynamicRHI* getRhi() const;

  [[nodiscard]]  RERHI::RHIRenderTarget* getMainRenderTarget() const;

  [[nodiscard]] RERenderer::IRenderer* getRenderer() const;

  [[nodiscard]] RERenderer::IRenderer& getRendererSafe() const;

public:

  inline virtual void onInitialization() {

  }

  inline virtual void onDeinitialization() {

  }

  inline virtual void onUpdate() {

  }

  inline virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) {

  }

  /**
   * @brief
   * Return if the example does the whole drawing process by itself.
   *
   * @return
   * True if example does its complete drawing handling, false otherwise.
   */
  [[nodiscard]] inline virtual bool doesCompleteOwnDrawing() const {
    return false;
  }

private:

  Application* mApplication;
  RECore::String mExampleName;
};