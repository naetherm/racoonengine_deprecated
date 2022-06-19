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
#include "REGui/Widget/Slider/DoubleSlider.h"
#include "REGui/Helper/ImGuiHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(DoubleSlider, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(DoubleSlider)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
DoubleSlider::DoubleSlider() {

}

DoubleSlider::~DoubleSlider() {

}


void DoubleSlider::construct(ConstructionArguments args) {
  mLabelText = args.getLabelText();
  mFormat = args.getFormat();
  mMin = args.getMin();
  mMax = args.getMax();
  mValue = args.getValue();
  mOrientation = args.getOrientation();

  // Post creation checks
  if (mMax < mMin) {
    mMax = mMin;
  }
  if (mValue < mMin) {
    mValue = mMin;
  } else if (mValue > mMax) {
    mValue = mMax;
  }
}

void DoubleSlider::onUpdate(float deltaTime) {

}

void DoubleSlider::onDraw() {
  bool valueChanged = false;

  switch (mOrientation) {
    case ESliderOrientation::HORIZONTAL:
    {
      valueChanged = ImGui::SliderScalar(
        mLabelText + mWidgetId,
        ImGuiDataType_::ImGuiDataType_S32,
        &mValue,
        &mMin,
        &mMax,
        mFormat);
      break;
    }
    case ESliderOrientation::VERTICAL:
    {
      valueChanged = ImGui::VSliderScalar(
        mLabelText + mWidgetId,
        ImGuiHelper::ToImVec2(mVerticalModeSize),
        ImGuiDataType_::ImGuiDataType_Double,
        &mValue,
        &mMin,
        &mMax,
        mFormat);
      break;
    }
  }

  if (valueChanged) {
    // Notification
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
