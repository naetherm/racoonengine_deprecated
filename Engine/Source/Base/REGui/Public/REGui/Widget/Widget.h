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
#include "REGui/Widget/WidgetCreationSyntax.h"
#include "REGui/Widget/WidgetTypes.h"
#include <RECore/Reflect/Object.h>
#include <RECore/String/String.h>
#include <imgui.h>


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
 * Widget
 *
 * @brief
 * Abstract widget representation. All widgets must inherit in some way from this one.
 */
class Widget : public RECore::Object {

  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def(REGUI_API)
    // Signals
    re_signal_0_def(SignalClicked)
    re_signal_0_def(SignalDoubleClicked)
    re_signal_0_def(SignalContentChanged)
    re_signal_0_def(SignalEnterPressed)
  re_class_def_end

private:

  static RECore::uint64 SWidgetID;

public:

  /**
   * @brief
   * Default constructor.
   */
  Widget();

  /**
   * @brief
   * Destructor.
   */
  ~Widget() override;

public:

  /**
   * @brief
   * Called when the widget is updated.
   *
   * @param[in] deltaTime
   * The time between the this and the last update in seconds.
   */
  virtual void onUpdate(float deltaTime);

  /**
   * @brief
   * Called in the drawing process.
   */
  virtual void onDraw();

protected:
  /** Unique widget ID */
  RECore::String mWidgetId;
  /** Is this widget enabled, true by default */
  bool mEnabled;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
