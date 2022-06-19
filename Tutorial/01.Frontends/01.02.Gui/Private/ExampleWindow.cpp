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
#include "ExampleWindow.h"
#include <REGui/Widget/Widgets.h>
#include <imgui.h>
#include <imgui_internal.h>

class TestCompound : public REGui::Compound {
public:


  regui_begin_construction_args(TestCompound)
    {}
    regui_value(RECore::String, LabelText)
    regui_value(RECore::String, ButtonText)
  regui_end_construction_args()

public:

  TestCompound(): mDummy(0.05f) {}

  ~TestCompound() override {}


  void construct(ConstructionArguments args) {
    mLabelText = args.getLabelText();
    mButtonText = args.getButtonText();

    mLayout = gui_new(REGui::VerticalBoxLayout)
      +REGui::VerticalBoxLayout::Slot()
      [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::Text)
            .setText(mLabelText)
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::Button)
              .setText(mButtonText)
              .setDisabled(false)
        ]
      ]
      +REGui::VerticalBoxLayout::Slot()
      [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::Button)
           .setText("Increment")
           .setSlotOnClicked(RECore::EventHandler<>(&TestCompound::onIncrement, this))
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
          gui_new(REGui::ProgressBar)
           .setLabelText("Fill me up")
           .setSize(RECore::Vec2i(100, 20))
           //.setFraction(mDummy)
           .setFraction(RECore::Functor<float>(&TestCompound::getDummy, this))
        ]
     ];
  }

public:

  void onUpdate() override {}

public:

  void onIncrement() {
    printf("onIncrement\n");
    mDummy += 0.01f;
  }

  float getDummy() {
    return mDummy;
  }

protected:

  RECore::String mLabelText;
  RECore::String mButtonText;

  float mDummy;
};

ExampleWindow::ExampleWindow(REGui::Gui* gui)
: REGui::DockableMainWindow(gui)
, mMainMenuBar(nullptr)
, mLayout(nullptr)
, mHLayout(nullptr)
, mCLayout(nullptr)
, mCompound(nullptr)
, mForm(nullptr) {
  //mButton.SlotOnClicked(&ExampleWindow::calledOnButtonClicked, this);
  //mMainMenuBar.createWidget<REGui::Menu>("File")
  //  .createWidget<REGui::MenuItem>("New");
  mMainMenuBar = gui_new(REGui::MainMenuBar)
    +REGui::MainMenuBar::MainMenuSlot()
    [
      gui_new(REGui::Menu)
        .setLabel("Menu")
        +REGui::Menu::MenuSlot()
        [
           gui_new(REGui::MenuItem)
            .setLabel("New")
        ]
        +REGui::Menu::MenuSlot()
        [
          gui_new(REGui::Menu)
          .setLabel("Subs")
          +REGui::Menu::MenuSlot()
          [
              gui_new(REGui::MenuItem)
              .setLabel("Sub-Item 1")
          ]
          +REGui::Menu::MenuSlot()
          [
              gui_new(REGui::MenuItem)
              .setLabel("Sub-Item 2")
          ]
        ]
        +REGui::Menu::MenuSlot()
        [
           gui_new(REGui::MenuSeparator)
        ]
        +REGui::Menu::MenuSlot()
        [
           gui_new(REGui::MenuItem)
            .setLabel("Exit")
        ]
    ];
  setMainMenuBar(mMainMenuBar);
  makeMainMenuBarVisible(true);


  mLayout = gui_new(REGui::VerticalBoxLayout)
    +REGui::VerticalBoxLayout::Slot()
    [
      gui_new(REGui::Button)
        .setText("LayoutButton")
        .setDisabled(false)
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
      gui_new(REGui::Text)
        .setText("LayoutText")
    ];
  mHLayout = gui_new(REGui::HorizontalBoxLayout)
    +REGui::HorizontalBoxLayout::Slot()
    [
        gui_new(REGui::Button)
          .setText("HLayoutButton")
          .setDisabled(false)
    ]
    +REGui::HorizontalBoxLayout::Slot()
    [
        gui_new(REGui::Text)
        .setText("HLayoutText")
    ];
  mCLayout = gui_new(REGui::VerticalBoxLayout)
    +REGui::VerticalBoxLayout::Slot()
    [
         gui_new(REGui::HorizontalBoxLayout)
         +REGui::HorizontalBoxLayout::Slot()
         [
             gui_new(REGui::Button)
               .setText("CLayoutButton1")
               .setDisabled(false)
         ]
         +REGui::HorizontalBoxLayout::Slot()
         [
             gui_new(REGui::Text)
              .setText("CLayoutText1")
         ]
     ]
     +REGui::VerticalBoxLayout::Slot()
     [
         gui_new(REGui::HorizontalBoxLayout)
         +REGui::HorizontalBoxLayout::Slot()
         [
            gui_new(REGui::CheckBox)
              .setText("Active Checkbox")
              .setValue(true)
         ]
         +REGui::HorizontalBoxLayout::Slot()
         [
            gui_new(REGui::CheckBox)
              .setText("Disabled Checkbox")
              .setValue(false)
         ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
          gui_new(REGui::HorizontalBoxLayout)
          +REGui::HorizontalBoxLayout::Slot()
          [
              gui_new(REGui::Button)
                .setText("CLayoutButton2")
                .setDisabled(true)
          ]
          +REGui::HorizontalBoxLayout::Slot()
          [
              gui_new(REGui::Text)
              .setText("CLayoutText2")
          ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::ComboBox)
              .setSelected(0)
              .setChoices({"Choice A", "Choice B", "Choice C"})
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::NewLine)
        ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::ArrowButton)
              .setDirection(REGui::EArrowDirection::LEFT)
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
           gui_new(REGui::ArrowButton)
           .setDirection(REGui::EArrowDirection::RIGHT)
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
           gui_new(REGui::ArrowButton)
           .setDirection(REGui::EArrowDirection::UP)
        ]
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::ArrowButton)
            .setDirection(REGui::EArrowDirection::DOWN)
        ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
      gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::IntSlider)
            .setOrientation(REGui::ESliderOrientation::HORIZONTAL)
            .setMin(0)
            .setMax(100)
            .setValue(10)
            .setLabelText("Slider")
            .setFormat("%d")
        ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
      gui_new(REGui::Collapsable)
        .setHeader("Collapsable")
        .setClosable(true)
        .setIsOpen(true)
        .setLayout(
          gui_new(REGui::HorizontalBoxLayout)
          +REGui::HorizontalBoxLayout::Slot()
          [
              gui_new(REGui::IntSlider)
              .setOrientation(REGui::ESliderOrientation::HORIZONTAL)
              .setMin(0)
              .setMax(100)
              .setValue(10)
              .setLabelText("Slider")
              .setFormat("%d")
          ]
        )
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
        gui_new(REGui::HorizontalBoxLayout)
        +REGui::HorizontalBoxLayout::Slot()
        [
            gui_new(REGui::TextAreaInput)
              .setContent("Example Content")
              .setSize(RECore::Vec2i(500, 10))
        ]
    ]
    +REGui::VerticalBoxLayout::Slot()
    [
       gui_new(REGui::HorizontalBoxLayout)
       +REGui::HorizontalBoxLayout::Slot()
       [
         gui_new(REGui::ColorPicker)
           .setColor(RECore::Color4::RED)
           .setEnableAlpha(true)
       ]
     ]
     +REGui::VerticalBoxLayout::Slot()
     [
       gui_new(REGui::HorizontalBoxLayout)
       +REGui::HorizontalBoxLayout::Slot()
       [
         gui_new(REGui::ColorEdit)
           .setColor(RECore::Color4::GREEN)
           .setEnableAlpha(true)
       ]
    ];
  mCompound = gui_new(TestCompound)
    .setLabelText("Compound Label")
    .setButtonText("Compound Button");
  mForm = gui_new(REGui::Form)
    +REGui::Form::FormSlot()
    .left(
      gui_new(REGui::Text)
      .setText("Left 1"))
    .right(
      gui_new(REGui::Text)
      .setText("Right 1"))
     +REGui::Form::FormSlot()
       .left(
        gui_new(REGui::Text)
         .setText("Some very long tile for testing"))
       .right(
        gui_new(REGui::Text)
         .setText("Right 2"));
}

ExampleWindow::~ExampleWindow() {
  delete mLayout;
  delete mHLayout;
  delete mCLayout;
  delete mCompound;
  delete mMainMenuBar;
}

void ExampleWindow::onDraw() {
  REGui::DockableMainWindow::onDraw();
  // Draw code goes right here
  ImGui::ShowDemoWindow();

  if (ImGui::Begin("DummyWindow")) {
    mLayout->onDraw();
    mHLayout->onDraw();
    mCLayout->onDraw();
    mCompound->onDraw();
    mForm->onDraw();
    ImGui::End();
  }
}


void ExampleWindow::calledOnButtonClicked() {
  printf("calledOnButtonClicked\n");
}