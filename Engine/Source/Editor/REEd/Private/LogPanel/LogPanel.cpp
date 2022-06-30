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
#include "REEd/LogPanel/LogPanel.h"
#include <REGui/Widget/Layout/HorizontalBoxLayout.h>
#include <REGui/Widget/Layout/VerticalBoxLayout.h>
#include <REGui/Widget/Button/Button.h>
#include <REGui/Widget/Control/CheckBox.h>
#include <REGui/Widget/Text/Text.h>


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(LogPanel, "", REGui::Panel, "Application class")
  // Constructors
re_class_metadata_end(LogPanel)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
LogPanel::LogPanel()
: REGui::Panel() {
  REGui::Panel::ConstructionArguments args;
  args.setTitle("Log");
  args.setLayout(
    gui_new(REGui::VerticalBoxLayout)
      +REGui::VerticalBoxLayout::Slot()
      [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::Button)
            .setText("Clear")
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::CheckBox)
            .setText("Info")
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::CheckBox)
            .setText("Warning")
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::CheckBox)
            .setText("Error")
        ]
      ]
      +REGui::VerticalBoxLayout::Slot()
      [
        gui_new(REGui::Text)
          .setText("Text Output")
      ]
  );

  construct(args);
}

LogPanel::~LogPanel() {

}


void LogPanel::onDraw() {
  REGui::Panel::onDraw();
}