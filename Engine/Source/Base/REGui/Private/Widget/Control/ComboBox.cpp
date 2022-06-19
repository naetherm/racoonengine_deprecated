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
#include "REGui/Widget/Control/ComboBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ComboBox, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(ComboBox)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ComboBox::ComboBox() {

}

ComboBox::~ComboBox() {

}

void ComboBox::construct(ConstructionArguments args) {
  mChoices = args.getChoices();
  mSelected = args.getSelected();
}

void ComboBox::onUpdate(float deltaTime) {

}

void ComboBox::onDraw() {
  // If the selected one is invalid just select the first one
  if (mSelected < 0 || mSelected >= mChoices.size()) {
    mSelected = 0;
  }

  if (ImGui::BeginCombo(mWidgetId, mChoices[mSelected])) {
    for (RECore::uint32 i = 0; i < mChoices.size(); ++i) {
      bool isSelected = i == mSelected;

      if (ImGui::Selectable(mChoices[i], isSelected)) {
        if (!isSelected) {
          ImGui::SetItemDefaultFocus();
          mSelected = i;
          // Notify on change
        }
      }
    }

    ImGui::EndCombo();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
