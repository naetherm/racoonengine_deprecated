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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REGui/REGui.h"
#include "REGui/Widget/Layout/BoxLayout.h"
#include "REGui/Layout/MultiSlot.h"
#include "REGui/Layout/PanelChildren.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Form
 *
 * @brief
 */
class Form : public Layout {

  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
re_class_def(REGUI_API)
re_class_def_end

public:

  class FormSlot : public TMultiSlot<FormSlot> {
  public:

    /**
     * @brief
     * Default constructor.
     */
    FormSlot();

    ~FormSlot() override;

    FormSlot& left(Widget* widget);

    FormSlot& right(Widget* widget);

    Widget* left();

    Widget* right();
  };

public:

regui_begin_construction_args(Form)
    {}
  regui_slots(FormSlot)
  regui_end_construction_args()

public:

  /**
   * @brief
   * Constructor.
   */
  Form();

  /**
   * @brief
   * Destructor.
   */
  ~Form() override;


  /**
   * @brief
   * Construct this widget.
   *
   * @param[in] args
   * The declaration data for this widget.
   */
  void construct(ConstructionArguments args);

public:

  /**
   * @brief
   * Called in the drawing process.
   */
  void onDraw() override;

protected:
  /** All slots */
  PanelChildren<FormSlot> mChildren;
  /** Widgets on the left side of the form */
  std::vector<Widget*> mLeft;
  /** Widgets on the right side of the form */
  std::vector<Widget*> mRight;
  /** Deprecated? */
  std::vector<float> mWidths;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui