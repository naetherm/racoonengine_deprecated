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
#include "REGui/Widget/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Widget, "REGui", RECore::Object, "Application class")
  // Constructors
  // Signals
  re_signal_0_metadata(SignalClicked, "Signal that is emitted when an element was clicked", "")
  re_signal_0_metadata(SignalDoubleClicked, "Signal that is emitted when an element was double-clicked", "")
  re_signal_0_metadata(SignalContentChanged, "Signal that is emitted when the content changed", "")
  re_signal_0_metadata(SignalEnterPressed, "Signal that is emitted when enter was pressed", "")
re_class_metadata_end(Widget)


RECore::uint64 Widget::SWidgetID = 0;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Widget::Widget()
: mWidgetId("##" + Widget::SWidgetID++){

}

Widget::~Widget() {

}

void Widget::onUpdate() {

}

void Widget::onDraw() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
