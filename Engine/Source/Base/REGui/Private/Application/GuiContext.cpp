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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REGui/Application/GuiContext.h"
#include <RECore/Platform/Platform.h>
#include <RERHI/Rhi.h>
#if defined(LINUX)
#include <RERHI/Linux/X11Context.h>
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


GuiContext::GuiContext()
: mRhiContext(nullptr)
, mRhi(nullptr)
, mRendererContext(nullptr)
, mRenderer(nullptr) {

}

GuiContext::~GuiContext() {
  delete mRenderer;
  delete mRendererContext;
  delete mRhi;
  delete mRhiContext;
}

void GuiContext::initialize(const RECore::String& rhiName) {
  mRhiName = rhiName;

  this->mSharedLibraryName = RECore::Platform::instance().getSharedLibraryPrefix() + "RERHI" + mRhiName + "." +
                             RECore::Platform::instance().getSharedLibraryExtension();

  // Create the RHI context
}

void GuiContext::setRhiName(const RECore::String &rhiName) {
  mRhiName = rhiName;
}

const RECore::String &GuiContext::getRhiName() const {
  return mRhiName;
}

RERHI::RHIContext *GuiContext::getRhiContext() const {
  return mRhiContext;
}

const RECore::String &GuiContext::getSharedLibraryName() const {
  return mSharedLibraryName;
}

const RECore::DynLib &GuiContext::getRhiSharedLibrary() const {
  return mRhiSharedLibrary;
}

RERHI::RHIDynamicRHI *GuiContext::getRhi() const {
  return mRhi;
}

RERenderer::Context *GuiContext::getRendererContext() const {
  return mRendererContext;
}

RERenderer::IRenderer *GuiContext::getRenderer() const {
  return mRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
