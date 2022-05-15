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
#include "REGui/Gui/Screen.h"
#include "REGui/Gui/Gui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


Screen::Screen(Gui *gui)
: mGui(gui) {

}

Screen::~Screen() {

}

[[nodiscard]] Gui *Screen::getGui() const {
  return mGui;
}

[[nodiscard]] const RECore::String &Screen::getName() const {
  return mName;
}

void Screen::setName(const RECore::String &name) {
  mName = name;
}

[[nodiscard]] const RECore::Vec2i &Screen::getPosition() const {
  return mPosition;
}

void Screen::setPosition(const RECore::Vec2i &position) {
  mPosition = position;
}

[[nodiscard]] const RECore::Vec2i &Screen::getSize() const {
  return mSize;
}

void Screen::setSize(const RECore::Vec2i &size) {
  mSize = size;
}

void Screen::setDefault(bool defaultScreen) {
  mDefaultScreen = defaultScreen;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
