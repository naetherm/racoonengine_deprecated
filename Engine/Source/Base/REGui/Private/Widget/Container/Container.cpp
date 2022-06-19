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
#include "REGui/Widget/Container/Container.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Container, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(Container)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Container::Container() {

}

Container::~Container() {
  // Clears all widgets
  clear();
}

void Container::clear() {
  for (RECore::uint32 i = 0; i < mChildren.size(); ++i) {
    delete mChildren[i];
  }
  mChildren.clear();
}

void Container::addWidget(Widget *widget) {
  mChildren.push_back(widget);
}

void Container::removeWidget(Widget *widget) {
  auto iter = std::find(mChildren.begin(), mChildren.end(), widget);
  if (iter != mChildren.end()) {
    mChildren.erase(iter);
  }
}

void Container::onUpdate(float deltaTime) {

}

void Container::onDraw() {
  // Loop through all widgets and call their onDraw-method
  for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
    (*iter)->onDraw();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
