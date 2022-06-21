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
#include "REGui/Widget/Tab/TabBar.h"
#include "REGui/Widget/Tab/TabItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(TabBar, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(TabBar)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
TabBar::TabBar()
: mTabs(nullptr) {

}

TabBar::~TabBar() {
  delete mTabs;
}


void TabBar::construct(ConstructionArguments args) {
  mTabs = args.getTabs();
}

void TabBar::onUpdate(float deltaTime) {
  // Nothing to do here
}

void TabBar::onDraw() {
  if (ImGui::BeginTabBar("MyTabBar")) {
    for (RECore::sizeT i = 0; i < mTabs->getChildren().getNumOfChildren(); ++i) {
      // Get content as raw widget pointer
      TabItem *item = reinterpret_cast<TabItem *>(mTabs->getChildren().getChildAtIndex(i));

      if (ImGui::BeginTabItem(item->getTitle())) {
        item->onDraw();
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
