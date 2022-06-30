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
#include <RECore/Log/Log.h>
#include <RECore/Platform/Platform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


GuiContext::GuiContext()
: mRhiLibrary(nullptr) {

}

GuiContext::~GuiContext() {
  if (mRhiLibrary) {
    delete mRhiLibrary;
  }
}


bool GuiContext::initialize(const RECore::String &rhiName) {
  mRhiName = rhiName;
  mRhiLibraryName = RECore::Platform::instance().getSharedLibraryPrefix()
    + "RERHI" + mRhiName + "."
    + RECore::Platform::instance().getSharedLibraryExtension();

  mRhiLibrary = new RECore::DynLib();
  // Load shared dynamic library
  if (mRhiLibrary->load(mRhiLibraryName)) {
    RE_LOG(Info, "Successfully loaded library" + mRhiLibraryName)
    return true;
  }

  RE_LOG(Critical, "Unable to load library" + mRhiLibraryName)

  // Clean library
  if (mRhiLibrary) {
    delete mRhiLibrary;
    mRhiLibrary = nullptr;
  }

  return false;
}

const RECore::String& GuiContext::getRhiName() const {
  return mRhiName;
}

const RECore::String & GuiContext::getRhiLibraryName() const {
  return mRhiLibraryName;
}

RECore::DynLib& GuiContext::getRhiLibrary() const {
  return *mRhiLibrary;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
