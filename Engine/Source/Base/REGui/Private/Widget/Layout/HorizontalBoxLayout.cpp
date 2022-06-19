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
#include "REGui/Widget/Layout/HorizontalBoxLayout.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(HorizontalBoxLayout, "REGui", REGui::BoxLayout, "Application class")
  // Constructors
re_class_metadata_end(HorizontalBoxLayout)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
HorizontalBoxLayout::Slot::Slot() {

}

HorizontalBoxLayout::Slot::~Slot() {

}



HorizontalBoxLayout::Slot& HorizontalBoxLayout::Slot::operator[](Widget* widget) {
  BoxLayout::BoxLayoutSlot::operator[](widget);

  return *this;
}


HorizontalBoxLayout::HorizontalBoxLayout() {

}

HorizontalBoxLayout::~HorizontalBoxLayout() {

}


void HorizontalBoxLayout::construct(ConstructionArguments args) {
  for (RECore::uint32 i = 0; i < args.getSlots().size(); ++i) {
    mChildren.add(args.getSlotByIndex(i));
  }
}

void HorizontalBoxLayout::onDraw() {
  // Act, dependent on number of children
  RECore::uint32 numChildren = mChildren.getNumOfChildren();

  if (numChildren == 1) {
    mChildren[0].getWidget()->onDraw();
  } else if (numChildren > 1) {
    for (RECore::uint32 i = 0; i < numChildren - 1; ++i) {
      mChildren[i].getWidget()->onDraw();
      ImGui::SameLine();
    }
    mChildren[numChildren-1].getWidget()->onDraw();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
