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
#include <RECore/System/DynLib.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer {
class Context;
class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiContext
 *
 * @brief
 * Gui context implementation.
 */
class GuiContext {

  friend class GuiApplication;

public:

  /**
   * @brief
   * Default constructor
   */
  REGUI_API GuiContext();

  /**
   * @brief
   * Destructor
   */
  REGUI_API virtual ~GuiContext();


  void initialize(const RECore::String& rhiName);


  /**
   * @brief
   * Sets the rhi name.
   *
   * @param[in] rhiName
   * The rhi name.
   */
  void setRhiName(const RECore::String& rhiName);

  /**
   * @brief
   * Returns the rhi name.
   *
   * @return
   * The rhi name.
   */
  [[nodiscard]] const RECore::String& getRhiName() const;

  [[nodiscard]] const RERHI::RHIContext* getRhiContext() const;

  [[nodiscard]] const RECore::String& getSharedLibraryName() const;

  [[nodiscard]] const RECore::DynLib& getRhiSharedLibrary() const;

  [[nodiscard]] const RERHI::RHIDynamicRHI* getRhi() const;

  [[nodiscard]] const RERenderer::Context* getRendererContext() const;

  [[nodiscard]] const RERenderer::IRenderer* getRenderer() const;

protected:
  /** The name of the RHI instance */
  RECore::String mRhiName;
  /** RHI context */
  RERHI::RHIContext* mRhiContext;
  /** The shared library name of the RHI library */
  RECore::String mSharedLibraryName;
  /** The shared library */
  RECore::DynLib mRhiSharedLibrary;
  /** RHI instance */
  RERHI::RHIDynamicRHI* mRhi;
  /** Renderer context instance */
  RERenderer::Context* mRendererContext;
  /** Renderer instance */
  RERenderer::IRenderer* mRenderer;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
