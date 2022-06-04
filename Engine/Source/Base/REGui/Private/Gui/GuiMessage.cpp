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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

GuiMessage GuiMessage::onDraw(NativeWindow* nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnDraw);
}

GuiMessage GuiMessage::onShow(NativeWindow* nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnShow);
}

GuiMessage GuiMessage::onHide(NativeWindow* nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnHide);
}

GuiMessage GuiMessage::onMove(NativeWindow* nativeWindow, const RECore::Vec2i& position) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnMove);
  guiMessage.mPositionSize = position;
  return guiMessage;
}

GuiMessage GuiMessage::onSize(NativeWindow* nativeWindow, const RECore::Vec2i& size) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnSize);
  guiMessage.mPositionSize = size;
  return guiMessage;
}

GuiMessage GuiMessage::onMouseEnter(NativeWindow* nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnMouseEnter);
}

GuiMessage GuiMessage::onMouseLeave(NativeWindow* nativeWindow) {
  return GuiMessage(nativeWindow, EMessageType::MessageOnMouseLeave);
}

GuiMessage GuiMessage::onMouseMove(NativeWindow* nativeWindow, const RECore::Vec2i& position) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnMouseMove);
  guiMessage.mPositionSize = position;
  return guiMessage;
}

GuiMessage GuiMessage::onMouseButtonDown(NativeWindow* nativeWindow, EMouseButton button) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnMouseButtonDown);
  guiMessage.mDataBlock1.mMouseButton = button;
  return guiMessage;
}

GuiMessage GuiMessage::onMouseButtonUp(NativeWindow* nativeWindow, EMouseButton button) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnMouseButtonUp);
  guiMessage.mDataBlock1.mMouseButton = button;
  return guiMessage;
}

GuiMessage GuiMessage::onMouseWheel(NativeWindow* nativeWindow, int delta) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnMouseWheel);
  guiMessage.mDataBlock1.mDelta = delta;
  return guiMessage;
}

GuiMessage GuiMessage::onKeyDown(NativeWindow* nativeWindow, RECore::uint32 key, RECore::uint32 modifiers) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnKeyDown);
  guiMessage.mDataBlock1.mKey = key;
  guiMessage.mDataBlock2.mModifiers = modifiers;
  return guiMessage;
}

GuiMessage GuiMessage::onKeyUp(NativeWindow* nativeWindow, RECore::uint32 key, RECore::uint32 modifiers) {
  GuiMessage guiMessage(nativeWindow, EMessageType::MessageOnDestroy);
  guiMessage.mDataBlock1.mKey = key;
  guiMessage.mDataBlock2.mModifiers = modifiers;
  return guiMessage;
}


GuiMessage::GuiMessage(NativeWindow *nativeWindow, EMessageType messageType)
: mNativeWindow(nativeWindow)
, mMessageType(messageType) {

}

GuiMessage::GuiMessage(const GuiMessage &rhs)
: mNativeWindow(rhs.mNativeWindow)
, mMessageType(rhs.mMessageType)
, mDataBlock1(rhs.mDataBlock1)
, mDataBlock2(rhs.mDataBlock2)
, mPositionSize(rhs.mPositionSize) {

}

GuiMessage::~GuiMessage() {

}

bool GuiMessage::operator==(const GuiMessage &rhs) const {
  return ((mNativeWindow == rhs.mNativeWindow) &&
          (mMessageType == rhs.mMessageType) &&
          (mPositionSize == rhs.mPositionSize));
}

GuiMessage& GuiMessage::operator=(const GuiMessage &rhs) {
  mNativeWindow = rhs.mNativeWindow;
  mMessageType = rhs.mMessageType;

  mDataBlock1 = rhs.mDataBlock1;
  mDataBlock2 = rhs.mDataBlock2;

  mPositionSize = rhs.mPositionSize;

  return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
