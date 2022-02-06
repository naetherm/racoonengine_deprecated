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
#include "REInput/REInput.h"
#include "REInput/Input/Virtual/VirtualController.h"
#include "REInput/Input/Control/Axis.h"
#include "REInput/Input/Control/Button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Standard virtual input controller
*
*  @remarks
*    3D coordinate system:
*    @code
*      y = > Translation: Move up/down (+/-) => Rotation: Yaw (also called 'heading') change is turning to the left or right
*      |
*      |
*      *---x => Translation: Strafe left/right (+/-) => Rotation: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
*     /
*    z => Translation: Move forwards/backwards (+/-) => Rotation: Roll (also called 'attitude') change is moving one wingtip up and the other down
*    @endcode
**/
class VirtualStandardController final : public VirtualController
{


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  // Mouse
  Axis		MouseX;						///< X axis (movement data, no absolute data)
  Axis		MouseY;						///< Y axis (movement data, no absolute data)
  Axis		MouseWheel;					///< Mouse wheel (movement data, no absolute data)
  Button		MouseLeft;					///< Left mouse button (mouse button #0)
  Button		MouseRight;					///< Right mouse button (mouse button #1)
  Button		MouseMiddle;				///< Middle mouse button (mouse button #2)
  Button		MouseButton4;				///< Mouse button #4
  Button		MouseButton5;				///< Mouse button #5
  Button		MouseButton6;				///< Mouse button #6
  Button		MouseButton7;				///< Mouse button #7
  Button		MouseButton8;				///< Mouse button #8
  Button		MouseButton9;				///< Mouse button #9
  Button		MouseButton10;				///< Mouse button #10
  Button		MouseButton11;				///< Mouse button #11
  Button		MouseButton12;				///< Mouse button #12

  // Keyboard
  Button		KeyboardBackspace;			///< Backspace
  Button		KeyboardTab;				///< Tab
  Button		KeyboardClear;				///< Clear (not available everywhere)
  Button		KeyboardReturn;				///< Return (often the same as "Enter")
  Button		KeyboardShift;				///< Shift
  Button		KeyboardControl;			///< Control ("Ctrl")
  Button		KeyboardAlt;				///< Alt
  Button		KeyboardPause;				///< Pause
  Button		KeyboardCapsLock;			///< Caps lock
  Button		KeyboardEscape;				///< Escape
  Button		KeyboardSpace;				///< Space
  Button		KeyboardPageUp;				///< Page up
  Button		KeyboardPageDown;			///< Page down
  Button		KeyboardEnd;				///< End
  Button		KeyboardHome;				///< Home
  Button		KeyboardLeft;				///< Left arrow
  Button		KeyboardUp;					///< Up arrow
  Button		KeyboardRight;				///< Right arrow
  Button		KeyboardDown;				///< Down arrow
  Button		KeyboardSelect;				///< Select (not available everywhere)
  Button		KeyboardExecute;			///< Execute (not available everywhere)
  Button		KeyboardPrint;				///< Print screen
  Button		KeyboardInsert;				///< Insert
  Button		KeyboardDelete;				///< Delete
  Button		KeyboardHelp;				///< Help (not available everywhere)
  Button		Keyboard0;					///< 0
  Button		Keyboard1;					///< 1
  Button		Keyboard2;					///< 2
  Button		Keyboard3;					///< 3
  Button		Keyboard4;					///< 4
  Button		Keyboard5;					///< 5
  Button		Keyboard6;					///< 6
  Button		Keyboard7;					///< 7
  Button		Keyboard8;					///< 8
  Button		Keyboard9;					///< 9
  Button		KeyboardA;					///< A
  Button		KeyboardB;					///< B
  Button		KeyboardC;					///< C
  Button		KeyboardD;					///< D
  Button		KeyboardE;					///< E
  Button		KeyboardF;					///< F
  Button		KeyboardG;					///< G
  Button		KeyboardH;					///< H
  Button		KeyboardI;					///< I
  Button		KeyboardJ;					///< J
  Button		KeyboardK;					///< K
  Button		KeyboardL;					///< L
  Button		KeyboardM;					///< M
  Button		KeyboardN;					///< N
  Button		KeyboardO;					///< O
  Button		KeyboardP;					///< P
  Button		KeyboardQ;					///< Q
  Button		KeyboardR;					///< R
  Button		KeyboardS;					///< S
  Button		KeyboardT;					///< T
  Button		KeyboardU;					///< U
  Button		KeyboardV;					///< V
  Button		KeyboardW;					///< W
  Button		KeyboardX;					///< X
  Button		KeyboardY;					///< Y
  Button		KeyboardZ;					///< Z
  Button		KeyboardNumpad0;			///< Numeric keypad 0
  Button		KeyboardNumpad1;			///< Numeric keypad 1
  Button		KeyboardNumpad2;			///< Numeric keypad 2
  Button		KeyboardNumpad3;			///< Numeric keypad 3
  Button		KeyboardNumpad4;			///< Numeric keypad 4
  Button		KeyboardNumpad5;			///< Numeric keypad 5
  Button		KeyboardNumpad6;			///< Numeric keypad 6
  Button		KeyboardNumpad7;			///< Numeric keypad 7
  Button		KeyboardNumpad8;			///< Numeric keypad 8
  Button		KeyboardNumpad9;			///< Numeric keypad 9
  Button		KeyboardNumpadMultiply;		///< Numeric keypad "Multiply"
  Button		KeyboardNumpadAdd;			///< Numeric keypad "Add"
  Button		KeyboardNumpadSeparator;	///< Numeric keypad "Separator"
  Button		KeyboardNumpadSubtract;		///< Numeric keypad "Subtract"
  Button		KeyboardNumpadDecimal;		///< Numeric keypad "Decimal"
  Button		KeyboardNumpadDivide;		///< Numeric keypad "Divide"
  Button		KeyboardF1;					///< F1
  Button		KeyboardF2;					///< F2
  Button		KeyboardF3;					///< F3
  Button		KeyboardF4;					///< F4
  Button		KeyboardF5;					///< F5
  Button		KeyboardF6;					///< F6
  Button		KeyboardF7;					///< F7
  Button		KeyboardF8;					///< F8
  Button		KeyboardF9;					///< F9
  Button		KeyboardF10;				///< F10
  Button		KeyboardF11;				///< F11
  Button		KeyboardF12;				///< F12
  Button		KeyboardNumLock;			///< Num lock
  Button		KeyboardScrollLock;			///< Scroll lock
  Button		KeyboardCircumflex;			///< Circumflex (^)
  Button		KeyboardLeftWindows;		///< Left Windows key (natural keyboard)
  Button		KeyboardRightWindows;		///< Right Windows key (natural keyboard)
  Button		KeyboardApplications;		///< Applications key (natural keyboard)
  Button		KeyboardF13;				///< F13 key
  Button		KeyboardF14;				///< F14 key
  Button		KeyboardF15;				///< F15 key
  Button		KeyboardF16;				///< F16 key
  Button		KeyboardF17;				///< F17 key
  Button		KeyboardF18;				///< F18 key
  Button		KeyboardF19;				///< F19 key
  Button		KeyboardF20;				///< F20 key
  Button		KeyboardF21;				///< F21 key
  Button		KeyboardF22;				///< F22 key
  Button		KeyboardF23;				///< F23 key
  Button		KeyboardF24;				///< F24 key
  Button		KeyboardLeftShift;			///< Left shift key
  Button		KeyboardRightShift;			///< Right shift key
  Button		KeyboardLeftControl;		///< Left control key
  Button		KeyboardRightControl;		///< Right control key
  Button		KeyboardVolumeMute;			///< Volume mute key
  Button		KeyboardVolumeDown;			///< Volume down key
  Button		KeyboardVolumeUp;			///< Volume up key
  Button		KeyboardMediaNextTrack;		///< Next track key
  Button		KeyboardMediaPreviousTrack;	///< Previous track key
  Button		KeyboardMediaStop;			///< Stop media key
  Button		KeyboardMediaPlayPause;		///< Play/pause media key
  Button		KeyboardAdd;				///< For any country/region, the '+' key
  Button		KeyboardSeparator;			///< For any country/region, the ',' key
  Button		KeyboardSubtract;			///< For any country/region, the '-' key
  Button		KeyboardDecimal;			///< For any country/region, the '.' key
  Button		KeyboardOEM1;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
  Button		KeyboardOEM2;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
  Button		KeyboardOEM3;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
  Button		KeyboardOEM4;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
  Button		KeyboardOEM5;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
  Button		KeyboardOEM6;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
  Button		KeyboardOEM7;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
  Button		KeyboardOEM8;				///< Used for miscellaneous characters; it can vary by keyboard
  Button		KeyboardOEM102;				///< Either the angle bracket key or the backslash key on the RT 102-key keyboard

  // Main character controls
  Axis		TransX;						///< X translation axis: Strafe left/right (+/-)
  Axis		TransY;						///< Y translation axis: Move up/down (+/-)
  Axis		TransZ;						///< Z translation axis: Move forwards/backwards (+/-)
  Button		Pan;						///< Keep pressed to pan
  Axis		PanX;						///< X pan translation axis: Strafe left/right (+/-)
  Axis		PanY;						///< Y pan translation axis: Move up/down (+/-)
  Axis		PanZ;						///< Z pan translation axis: Move forwards/backwards (+/-)
  Axis		RotX;						///< X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
  Axis		RotY;						///< Y rotation axis: Yaw (also called 'heading') change is turning to the left or right
  Axis		RotZ;						///< Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down
  Button		Rotate;						///< Keep pressed to rotate
  Button		RotateSlow;					///< Keep pressed to rotate slowly
  Button		Forward;					///< Move forwards
  Button		Backward;					///< Move backwards
  Button		Left;						///< Move (rotate) left
  Button		Right;						///< Move (rotate) right
  Button		StrafeLeft;					///< Strafe left
  Button		StrafeRight;				///< Strafe right
  Button		Up;							///< Move up
  Button		Down;						///< Move down
  Button		Run;						///< Keep pressed to run
  Button		Sneak;						///< Keep pressed to sneak
  Button		Crouch;						///< Keep pressed to crouch
  Button		Jump;						///< Jump
  Button		Zoom;						///< Keep pressed to zoom
  Axis		ZoomAxis;					///< Zoom axis to zoom in or out (+/-)
  //Button		Button1;					///< Button for action #1
  //Button		Button2;					///< Button for action #2
  //Button		Button3;					///< Button for action #3
  //Button		Button4;					///< Button for action #4
  //Button		Button5;					///< Button for action #5

  // Interaction
  Button		Pickup;						///< Keep pressed to pickup
  Button		Throw;						///< Throw the picked object
  Button		IncreaseForce;				///< Keep pressed to increase the force applied to the picked object
  Button		DecreaseForce;				///< Keep pressed to decrease the force applied to the picked object
  Axis		PushPull;					///< Used to push/pull the picked object


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  VirtualStandardController() = delete;

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  */
  REINPUT_API explicit VirtualStandardController(InputManager& inputManager);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~VirtualStandardController() override
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] cSource
  *    Source to copy from
  *
  *  @return
  *    This instance
  */
  VirtualStandardController &operator =(const VirtualStandardController &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Public virtual VirtualController functions            ]
  //[-------------------------------------------------------]
public:
  REINPUT_API virtual void connectToDevices() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
