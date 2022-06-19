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
#include "REGui/Widget/Container/Collapsable.h"
#include "REGui/Widget/Layout/Layout.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Collapsable, "REGui", REGui::Compound, "Application class")
  // Constructors
re_class_metadata_end(Collapsable)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Collapsable::Collapsable()
: Compound() {

}

Collapsable::~Collapsable() {

}

void Collapsable::onUpdate(float deltaTime) {

}

void Collapsable::construct(ConstructionArguments args) {
  mHeader = args.getHeader();
  mClosable = args.getClosable();
  mIsOpened = args.getIsOpen();
  mLayout = args.getLayout();
}

void Collapsable::onDraw() {
  bool previousOpened = mIsOpened;

  if (ImGui::CollapsingHeader(mHeader, mClosable ? &mIsOpened : nullptr)) {
    // TODO(naetherm): Draw input
    if (mLayout) {
      mLayout->onDraw();
    }
  }

  if (mIsOpened != previousOpened) {
    if (mIsOpened) {
      // Invoke OnOpened
    } else {
      // Invoke OnClosed
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
