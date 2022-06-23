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
#include "REFrontend/Projects/ProjectsPanel.h"
#include <REGui/Widget/Tab/TabItem.h>
#include <REGui/Widget/Layout/VerticalBoxLayout.h>
#include <REGui/Widget/Layout/HorizontalBoxLayout.h>
#include <REGui/Widget/Layout/Form.h>
#include <REGui/Widget/Button/Button.h>
#include <REGui/Widget/Container/NullWidget.h>
#include <REGui/Widget/Input/TextInput.h>
#include <REGui/Widget/Modal/Modal.h>
#include <REGui/Widget/Text/Text.h>


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ProjectsPanel, "", REGui::Panel, "Application class")
  // Constructors
re_class_metadata_end(ProjectsPanel)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ProjectsPanel::ProjectsPanel()
: REGui::Panel()
, mShowCreateNewProject(false)
, mNewProjectModal(nullptr)
, mTabBar(nullptr) {

  mTabBar = gui_new(REGui::TabBar)
    .setTabs(gui_new(REGui::StackLayout)
    +REGui::StackLayout::StackLayoutSlot()
    [
       gui_new(REGui::TabItem)
         .setTitle("Current Projects")
           .setContent(gui_new(REGui::VerticalBoxLayout)
             +REGui::VerticalBoxLayout::Slot()
             [
                gui_new(REGui::Text)
                  .setText("Dummy")
             ]
           )

    ]
    +REGui::StackLayout::StackLayoutSlot()
    [
       gui_new(REGui::TabItem)
         .setTitle("New Project")
         .setContent(
           gui_new(REGui::VerticalBoxLayout)
             +REGui::VerticalBoxLayout::Slot()
             [
                gui_new(REGui::Form)
                +REGui::Form::FormSlot()
                  .left(gui_new(REGui::Text)
                          .setText("Name")
                  )
                  .right(gui_new(REGui::TextInput)
                           .setLabel("")
                  )
                +REGui::Form::FormSlot()
                  .left(gui_new(REGui::Text)
                          .setText("Description")
                  )
                  .right(gui_new(REGui::TextInput)
                           .setLabel("")
                  )
                +REGui::Form::FormSlot()
                  .left(gui_new(REGui::Text)
                          .setText("Project Path")
                  )
                  .right(gui_new(REGui::TextInput)
                           .setLabel("")
                  )
                +REGui::Form::FormSlot()
                  .left(gui_new(REGui::Text)
                          .setText("Modules")
                  )
                  .right(gui_new(REGui::NullWidget)
                  )
             ]
             +REGui::VerticalBoxLayout::Slot()
             [
               gui_new(REGui::Button)
               .setText("Create")
             ]
         )
    ]);

  REGui::Panel::ConstructionArguments args;
  args.setTitle("Projects");
  args.setShowFullscreen(true);
  args.setLayout(
    gui_new(REGui::VerticalBoxLayout)
      +REGui::VerticalBoxLayout::Slot()
      [
         mTabBar
      ]
  );

  construct(args);

}

ProjectsPanel::~ProjectsPanel() {
  delete mNewProjectModal;
  delete mTabBar;
}


void ProjectsPanel::onDraw() {
  REGui::Panel::onDraw();
}

void ProjectsPanel::onCreateNewProject() {
  mShowCreateNewProject = true;
}