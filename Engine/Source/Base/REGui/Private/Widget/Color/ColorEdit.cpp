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
#include "REGui/Widget/Color/ColorEdit.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ColorEdit, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(ColorEdit)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ColorEdit::ColorEdit() {

}

ColorEdit::~ColorEdit() {

}


void ColorEdit::construct(ConstructionArguments args) {
  mColor = args.getColor();
  mEnableAlpha = args.getEnableAlpha();
  SlotOnColorChanged = args.mEventSlotOnColorChanged;
}

void ColorEdit::onUpdate(float deltaTime) {

}

void ColorEdit::onDraw() {
  int flags = !mEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (mEnableAlpha) {
		valueChanged = ImGui::ColorEdit4(mWidgetId, mColor.value, flags);
	} else {
		valueChanged = ImGui::ColorEdit3(mWidgetId, mColor.value, flags);
  }
	if (valueChanged)
	{
		// ColorChangedEvent.Invoke(color);
		// this->NotifyChange();
    //SlotOnColorChanged(mColor);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
