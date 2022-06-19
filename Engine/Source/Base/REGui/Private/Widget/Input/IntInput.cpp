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
#include "REGui/Widget/Input/IntInput.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(IntInput, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(IntInput)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
IntInput::IntInput() {

}

IntInput::~IntInput() {

}


void IntInput::construct(ConstructionArguments args) {
  mLabel = args.getLabel();
  mFormat = args.getFormat();
  mValue = args.getDefaultValue();
  mStep = args.getStep();
  mFastStep = args.getFastStep();
  mSelectAllOnClick = args.getSelectAllOnClick();
}

void IntInput::onUpdate(float deltaTime) {

}

void IntInput::onDraw() {
  RECore::int32 previousValue = mValue;

  ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
  if (mSelectAllOnClick) {
    flags |= ImGuiInputTextFlags_AutoSelectAll;
  }

  if (ImGui::InputScalar(
    mLabel + mWidgetId,
    ImGuiDataType_::ImGuiDataType_S32,
    &mValue,
    mStep != 0 ? &mStep : nullptr,
    mFastStep != 0 ? &mFastStep : nullptr,
    mFormat,
    flags)) {
    // On Enter pressed
  }

  if (previousValue != mValue) {
    // Value changed
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
