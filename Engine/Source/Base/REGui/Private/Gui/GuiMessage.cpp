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
#include "REGui/Gui/GuiMessage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {

GuiMessage GuiMessage::onClose(NativeWindow *nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnClose);
}

GuiMessage GuiMessage::onCreate(NativeWindow *nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnCreate);
}

GuiMessage GuiMessage::onDestroy(NativeWindow *nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnDestroy);
}


GuiMessage::GuiMessage(NativeWindow *nativeWindow, EMessageType messageType)
: mNativeWindow(nativeWindow)
, mMessageType(messageType) {

}

GuiMessage::GuiMessage(const GuiMessage &rhs)
: mNativeWindow(rhs.mNativeWindow)
, mMessageType(rhs.mMessageType) {

}

GuiMessage::~GuiMessage() {

}

bool GuiMessage::operator==(const GuiMessage &rhs) const {
  return ((mNativeWindow == rhs.mNativeWindow) &&
    (mMessageType == rhs.mMessageType));
}

GuiMessage& GuiMessage::operator=(const GuiMessage &rhs) {
  mNativeWindow = rhs.mNativeWindow;
  mMessageType = rhs.mMessageType;

  return *this;
}

NativeWindow* GuiMessage::getWindow() const {
  return mNativeWindow;
}

EMessageType GuiMessage::getType() const {
  return mMessageType;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
