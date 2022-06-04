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
#include "REGui/Gui/NativeWindowImpl.h"
#include <RECore/Linux/X11Includes.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class NativeWindow;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class NativeWindowLinux : public NativeWindowImpl {
public:

  NativeWindowLinux(NativeWindow* nativeWindow);

  ~NativeWindowLinux() override;


  [[nodiscard]] ::Window getWindow() const;

  void handleEvent(XEvent& event);

public:

  RECore::handle getWindowHandle() const override;

  void redraw() override;

  void ping() override;

  void setTitle(const RECore::String& title) override;

  void getWindowSize(RECore::int32& width, RECore::int32& height) override;

  void setWindowSize(RECore::int32 width, RECore::int32 height) override;

  [[nodiscard]] bool isDestroyed() const override;

protected:
  ::Display* mDisplay;
  ::Window mWindowHandle;
  ::Atom WM_DELETE_WINDOW;
  ::Atom UTF8_STRING;
  ::Atom WM_NAME;
  ::Atom _NET_WM_NAME;
  ::Atom _NET_WM_VISIBLE_NAME;
  bool mDestroyed;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
