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
#include "REGui/Widget/Button/ColoredButton.h"
#include "REGui/Helper/ImGuiHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ColoredButton, "REGui", REGui::AbstractButton, "Application class")
  // Constructors
re_class_metadata_end(ColoredButton)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ColoredButton::ColoredButton() {

}

ColoredButton::~ColoredButton() {

}

void ColoredButton::construct(ConstructionArguments args) {
  mLabel = args.getText();
  mColor = args.getColor();
  mSize = args.getSize();
  mEnableAlpha = args.getEnableAlpha();
  SlotOnClicked = args.mEventSlotOnClicked;
}

void ColoredButton::onUpdate(float deltaTime) {
  // Nothing to do here
  SignalClicked();
}

void ColoredButton::onDraw() {
  ImVec4 color = ImGuiHelper::ToImVec4(mColor);

  if (ImGui::ColorButton(
    mLabel + mWidgetId,
    color,
    !mEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0,
    ImGuiHelper::ToImVec2(mSize))) {
    // On Clicked
  }

  mColor = ImGuiHelper::ToColor4(color);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
