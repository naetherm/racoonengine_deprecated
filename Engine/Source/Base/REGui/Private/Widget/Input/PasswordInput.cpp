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
#include "REGui/Widget/Input/PasswordInput.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(PasswordInput, "REGui", REGui::Widget, "Application class")
  // Constructors
re_class_metadata_end(PasswordInput)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
PasswordInput::PasswordInput() {

}

PasswordInput::~PasswordInput() {

}


void PasswordInput::construct(ConstructionArguments args) {
  mLabel = args.getLabel();
  mContent = args.getContent();
}

void PasswordInput::onUpdate(float deltaTime) {

}

void PasswordInput::onDraw() {
  RECore::String previousContent = mContent;

  mContent.resize(256, '\0');
  if (ImGui::InputText(
    mLabel + mWidgetId,
    &mContent[0],
    256,
    ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_Password)) {
    // Enter was pressed
  }
  mContent = mContent.cstr();

  if (mContent != previousContent) {
    // Content changed
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
