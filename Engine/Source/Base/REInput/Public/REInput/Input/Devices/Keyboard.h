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
#include "REInput/Input/Devices/Device.h"
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
*    Keyboard device
*
*  @remarks
*    This class supports the following device backend types:
*    - UpdateDevice
*/
class Keyboard final : public Device
{


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  Button	Backspace;			///< Backspace
  Button	Tab;				///< Tab
  Button	Clear;				///< Clear (not available everywhere)
  Button	Return;				///< Return (often the same as "Enter")
  Button	Shift;				///< Shift
  Button	Control;			///< Control ("Ctrl")
  Button	Alt;				///< Alt
  Button	Pause;				///< Pause
  Button	CapsLock;			///< Caps lock
  Button	Escape;				///< Escape
  Button	Space;				///< Space
  Button	PageUp;				///< Page up
  Button	PageDown;			///< Page down
  Button	End;				///< End
  Button	Home;				///< Home
  Button	Left;				///< Left arrow
  Button	Up;					///< Up arrow
  Button	Right;				///< Right arrow
  Button	Down;				///< Down arrow
  Button	Select;				///< Select (not available everywhere)
  Button	Execute;			///< Execute (not available everywhere)
  Button	Print;				///< Print screen
  Button	Insert;				///< Insert
  Button	Delete;				///< Delete
  Button	Help;				///< Help (not available everywhere)
  Button	Key0;				///< 0 (control name is "0")
  Button	Key1;				///< 1 (control name is "1")
  Button	Key2;				///< 2 (control name is "2")
  Button	Key3;				///< 3 (control name is "3")
  Button	Key4;				///< 4 (control name is "4")
  Button	Key5;				///< 5 (control name is "5")
  Button	Key6;				///< 6 (control name is "6")
  Button	Key7;				///< 7 (control name is "7")
  Button	Key8;				///< 8 (control name is "8")
  Button	Key9;				///< 9 (control name is "9")
  Button	A;					///< A
  Button	B;					///< B
  Button	C;					///< C
  Button	D;					///< D
  Button	E;					///< E
  Button	F;					///< F
  Button	G;					///< G
  Button	H;					///< H
  Button	I;					///< I
  Button	J;					///< J
  Button	K;					///< K
  Button	L;					///< L
  Button	M;					///< M
  Button	N;					///< N
  Button	O;					///< O
  Button	P;					///< P
  Button	Q;					///< Q
  Button	R;					///< R
  Button	S;					///< S
  Button	T;					///< T
  Button	U;					///< U
  Button	V;					///< V
  Button	W;					///< W
  Button	X;					///< X
  Button	Y;					///< Y
  Button	Z;					///< Z
  Button	Numpad0;			///< Numeric keypad 0
  Button	Numpad1;			///< Numeric keypad 1
  Button	Numpad2;			///< Numeric keypad 2
  Button	Numpad3;			///< Numeric keypad 3
  Button	Numpad4;			///< Numeric keypad 4
  Button	Numpad5;			///< Numeric keypad 5
  Button	Numpad6;			///< Numeric keypad 6
  Button	Numpad7;			///< Numeric keypad 7
  Button	Numpad8;			///< Numeric keypad 8
  Button	Numpad9;			///< Numeric keypad 9
  Button	NumpadMultiply;		///< Numeric keypad "Multiply"
  Button	NumpadAdd;			///< Numeric keypad "Add"
  Button	NumpadSeparator;	///< Numeric keypad "Separator"
  Button	NumpadSubtract;		///< Numeric keypad "Subtract"
  Button	NumpadDecimal;		///< Numeric keypad "Decimal"
  Button	NumpadDivide;		///< Numeric keypad "Divide"
  Button	F1;					///< F1
  Button	F2;					///< F2
  Button	F3;					///< F3
  Button	F4;					///< F4
  Button	F5;					///< F5
  Button	F6;					///< F6
  Button	F7;					///< F7
  Button	F8;					///< F8
  Button	F9;					///< F9
  Button	F10;				///< F10
  Button	F11;				///< F11
  Button	F12;				///< F12
  Button	NumLock;			///< Num lock
  Button	ScrollLock;			///< Scroll lock
  Button	Circumflex;			///< Circumflex (^)
  Button	LeftWindows;		///< Left Windows key (natural keyboard)
  Button	RightWindows;		///< Right Windows key (natural keyboard)
  Button	Applications;		///< Applications key (natural keyboard)
  Button	F13;				///< F13 key
  Button	F14;				///< F14 key
  Button	F15;				///< F15 key
  Button	F16;				///< F16 key
  Button	F17;				///< F17 key
  Button	F18;				///< F18 key
  Button	F19;				///< F19 key
  Button	F20;				///< F20 key
  Button	F21;				///< F21 key
  Button	F22;				///< F22 key
  Button	F23;				///< F23 key
  Button	F24;				///< F24 key
  Button	LeftShift;			///< Left shift key
  Button	RightShift;			///< Right shift key
  Button	LeftControl;		///< Left control key
  Button	RightControl;		///< Right control key
  Button	VolumeMute;			///< Volume mute key
  Button	VolumeDown;			///< Volume down key
  Button	VolumeUp;			///< Volume up key
  Button	MediaNextTrack;		///< Next track key
  Button	MediaPreviousTrack;	///< Previous track key
  Button	MediaStop;			///< Stop media key
  Button	MediaPlayPause;		///< Play/pause media key
  Button	Add;				///< For any country/region, the '+' key
  Button	Separator;			///< For any country/region, the ',' key
  Button	Subtract;			///< For any country/region, the '-' key
  Button	Decimal;			///< For any country/region, the '.' key
  Button	OEM1;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
  Button	OEM2;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
  Button	OEM3;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
  Button	OEM4;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
  Button	OEM5;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
  Button	OEM6;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
  Button	OEM7;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
  Button	OEM8;				///< Used for miscellaneous characters; it can vary by keyboard
  Button	OEM102;				///< Either the angle bracket key or the backslash key on the RT 102-key keyboard


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  *  @param[in] sName
  *    Device name
  *  @param[in] pImpl
  *    System specific device implementation, can, but shouldn't be a null pointer
  */
  REINPUT_API Keyboard(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~Keyboard() override
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
  Keyboard &operator =(const Keyboard &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Public virtual Controller functions                   ]
  //[-------------------------------------------------------]
public:
  REINPUT_API virtual void update() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
