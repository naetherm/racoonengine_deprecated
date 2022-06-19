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
#include "REGui/Widget/Widget.h"
#include <RECore/Math/Vec2i.h>


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
 * FloatSlider
 *
 * @brief
 */
class FloatSlider : public Widget {

  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def(REGUI_API)
  re_class_def_end


  regui_begin_construction_args(FloatSlider)
    : mValueLabelText("")
    , mValueFormat("%g")
    , mValueMin(0.0f)
    , mValueMax(1.0f)
    , mValueOrientation(REGui::ESliderOrientation::HORIZONTAL) {}
    regui_value(RECore::String, LabelText)
    regui_value(RECore::String, Format)
    regui_value(float, Min)
    regui_value(float, Max)
    regui_value(float, Value)
    regui_value(REGui::ESliderOrientation, Orientation)
  regui_end_construction_args()

public:

  /**
   * @brief
   * Default constructor.
   */
  FloatSlider();

  /**
   * @brief
   * Destructor.
   */
  ~FloatSlider() override;


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
   * Called when the widget is updated.
   *
   * @param[in] deltaTime
   * The time between the this and the last update in seconds.
   */
  void onUpdate(float deltaTime) override;

  /**
   * @brief
   * Called in the drawing process.
   */
  void onDraw() override;

protected:

  RECore::String mLabelText;
  RECore::String mFormat;
  float mMin;
  float mMax;
  float mValue;
  REGui::ESliderOrientation mOrientation;
  RECore::Vec2i mVerticalModeSize;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
