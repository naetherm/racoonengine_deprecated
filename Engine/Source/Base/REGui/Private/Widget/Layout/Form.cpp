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
#include "REGui/Widget/Layout/Form.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Form, "REGui", REGui::Layout, "Application class")
  // Constructors
re_class_metadata_end(Form)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Form::FormSlot::FormSlot() {
  mWidgets.resize(2);
}

Form::FormSlot::~FormSlot() {
  // Clear
}


Form::FormSlot& Form::FormSlot::left(Widget* widget) {
  mWidgets[0] = widget;

  return *this;
}

Form::FormSlot& Form::FormSlot::right(Widget* widget) {
  mWidgets[1] = widget;

  return *this;
}

Widget* Form::FormSlot::left() {
  return mWidgets[0];
}

Widget* Form::FormSlot::right() {
  return mWidgets[1];
}


Form::Form() {

}

Form::~Form() {

}


void Form::construct(ConstructionArguments args) {
  RECore::uint32 numSlots = args.getSlots().size();
  mWidths.resize(numSlots);
  mLeft.resize(numSlots);
  mRight.resize(numSlots);
  for (RECore::uint32 i = 0; i < numSlots; ++i) {
    mChildren.add(args.getSlotByIndex(i));
    mLeft[i] = args.getSlotByIndex(i)->left();
    mRight[i] = args.getSlotByIndex(i)->right();
  }
}

void Form::onDraw() {
  // Act, dependent on number of children
  RECore::uint32 numChildren = mChildren.getNumOfChildren();
  ImGui::Columns(2, ("##" + mWidgetId), false);
  ImGui::SetColumnWidth(0, 200);

  for (RECore::uint32 i = 0; i < numChildren; ++i) {
    mLeft[i]->onDraw();
    //ImGui::SetColumnWidth(i % 2, mWidths[i % 2]);
    ImGui::NextColumn();
    mRight[i]->onDraw();
    //ImGui::SetColumnWidth((i + 1) % 2, mWidths[(i+1)% 2]);

    ImGui::NextColumn();
  }

  // Necessary to not break the layout for following widget
  ImGui::Columns(1);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
