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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ GUI messages                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Message type
*/
enum EMessageType {
  MessageOnUnknown = 0,				/**< Unknown message type */
  MessageOnInternalMessage,			/**< Internal message that is not passed on to widgets and modifiers */
  MessageOnWakeup,					/**< Wakeup message loop */
  MessageOnExit,						/**< Exit GUI */
  MessageOnTimer,						/**< Timer message */
  MessageOnThemeChanged,				/**< Theme has been changed */
  MessageOnUpdateContent,				/**< Widget content has been changed */
  MessageOnUpdateChildWidget,			/**< Child widget has been changed */
  MessageOnAddChildWidget,			/**< Child widget has been added */
  MessageOnRemoveChildWidget,			/**< Child widget has been removed */
  MessageOnClose,						/**< Widget shall be closed (ALT+F4 or X-Button pressed) */
  MessageOnCreate,					/**< Widget has just been created */
  MessageOnDestroy,					/**< Widget is going to be destroyed */
  MessageOnShow,						/**< Widget gets shown */
  MessageOnHide,						/**< Widget gets hidden */
  MessageOnEnable,					/**< Widget gets enabled */
  MessageOnDisable,					/**< Widget gets disabled */
  MessageOnGetFocus,					/**< Widget gets focus */
  MessageOnLooseFocus,				/**< Widget looses focus */
  MessageOnActivate,					/**< Widget has been activated or deactivated (focus-widget has changed) */
  MessageOnDrawBackground,			/**< Widget background gets drawn, graphics object used for painting as parameter */
  MessageOnDraw,						/**< Widget gets drawn, graphics object used for painting as parameter */
  MessageOnMove,						/**< Widget gets moved, new widget position as parameter */
  MessageOnSize,						/**< Widget gets resized, new widget size as parameter */
  MessageOnWindowState,				/**< Window state has changed, new window state as parameter */
  MessageOnEnterFullscreen,			/**< Widget enters fullscreen mode */
  MessageOnLeaveFullscreen,			/**< Widget leaves fullscreen mode */
  MessageOnPreferredSize,				/**< Widget has calculated it's preferred size */
  MessageOnAdjustContent,				/**< Widget content has to be adjusted */
  MessageOnMouseEnter,				/**< Mouse enters widget */
  MessageOnMouseLeave,				/**< Mouse leaves widget */
  MessageOnMouseOver,					/**< Mouse-over widget has changed */
  MessageOnMouseMove,					/**< Mouse moves inside the widget, mouse position within the widget as parameter */
  MessageOnMouseHover,				/**< Mouse hovers over the widget */
  MessageOnMousePosUpdate,			/**< Mouse position inside the widget has changed due to the movement of widget */
  MessageOnMouseButtonDown,			/**< Mouse button is pressed, mouse button and mouse position within the widget as parameters */
  MessageOnMouseButtonUp,				/**< Mouse button is released, mouse button and mouse position within the widget as parameters */
  MessageOnMouseButtonClick,			/**< Mouse button has been clicked, mouse button and mouse position within the widget as parameters */
  MessageOnMouseButtonDoubleClick,	/**< Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters */
  MessageOnMouseWheel,				/**< Mouse wheel moved, mouse wheel movement as parameter */
  MessageOnKeyDown,					/**< Key gets pressed, pressed key and modifier keys pressed as parameters */
  MessageOnKeyUp,						/**< Key gets released, released key and modifier keys pressed as parameters */
  MessageOnHotkey,					/**< Hotkey pressed, hotkey ID as parameter */
  MessageOnDrop,						/**< Data has been dropped onto the widget */
  MessageOnUserMessage				/**< User message */
};


//[-------------------------------------------------------]
//[ Mouse definitions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse buttons
*/
enum EMouseButton {
  LeftButton = 1,		/**< Left mouse button */
  MiddleButton,		/**< Middle mouse button */
  RightButton,		/**< Right mouse button */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
