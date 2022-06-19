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
#include <REGui/Widget/Window/DockableMainWindow.h>
#include <REGui/Widget/Text/Text.h>
#include <REGui/Widget/Text/ColoredText.h>
#include <REGui/Widget/Text/DisabledText.h>
#include <REGui/Widget/Button/Button.h>
#include <REGui/Widget/Menu/MainMenuBar.h>
#include <REGui/Widget/Menu/Menu.h>
#include <REGui/Widget/Menu/MenuItem.h>
#include <REGui/Widget/Container/Compound.h>
#include <REGui/Widget/Layout/VerticalBoxLayout.h>
#include <REGui/Widget/Layout/HorizontalBoxLayout.h>
#include <REGui/Widget/Layout/Form.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class ExampleWindow : public REGui::DockableMainWindow {
public:

  ExampleWindow(REGui::Gui* gui);

  ~ExampleWindow() override;

public:

  void onDraw() override;

public:

protected:

  void calledOnButtonClicked();

protected:

  REGui::MainMenuBar* mMainMenuBar;
  REGui::Layout* mLayout;
  REGui::Layout* mHLayout;
  REGui::Layout* mCLayout;
  REGui::Compound * mCompound;
  REGui::Form* mForm;
};