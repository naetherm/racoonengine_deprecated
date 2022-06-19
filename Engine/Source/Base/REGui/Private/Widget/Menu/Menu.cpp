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
#include "REGui/Widget/Menu/Menu.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Menu, "REGui", REGui::Container, "Application class")
  // Constructors
re_class_metadata_end(Menu)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Menu::MenuSlot::MenuSlot() {

}

Menu::MenuSlot::~MenuSlot() {

}



Menu::MenuSlot& Menu::MenuSlot::operator[](Widget* widget) {
  TSlot<MenuSlot>::operator[](widget);

  return *this;
}

Menu::Menu()
: mOpened(false) {

}

Menu::~Menu() {

}

void Menu::onUpdate(float deltaTime) {

}

void Menu::construct(ConstructionArguments args) {
  mLabel = args.getLabel();
  mLocked = args.getLocked();
  for (RECore::uint32 i = 0; i < args.getSlots().size(); ++i) {
    mChildren.add(args.getSlotByIndex(i));
  }
}

void Menu::onDraw() {
  if (ImGui::BeginMenu(mLabel, !mLocked)) {

    if (!mOpened) {
      mOpened = true;
    }
    // Draw children
    //Container::onDraw();
    for (RECore::uint32 i = 0; i < mChildren.getNumOfChildren(); ++i) {
      mChildren[i].getWidget()->onDraw();
    }

    ImGui::EndMenu();
  } else {
    mOpened = false;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
