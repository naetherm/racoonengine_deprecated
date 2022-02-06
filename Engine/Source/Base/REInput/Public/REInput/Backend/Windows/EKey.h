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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual keys
*/
enum EKey {
  KeyBackspace			= 0x08,		///< Backspace
  KeyTab					= 0x09,		///< Tab
  KeyClear				= 0x0C,		///< Clear (not available everywhere)
  KeyReturn				= 0x0D,		///< Return (often the same as "Enter")
  KeyShift				= 0x10,		///< Shift
  KeyControl				= 0x11,		///< Control ("Ctrl")
  KeyAlt					= 0x12,		///< Alt
  KeyPause				= 0x13,		///< Pause
  KeyCapsLock				= 0x14,		///< Caps lock
  KeyEscape				= 0x1B,		///< Escape
  KeySpace				= 0x20,		///< Space
  KeyPageUp				= 0x21,		///< Page up
  KeyPageDown				= 0x22,		///< Page down
  KeyEnd					= 0x23,		///< End
  KeyHome					= 0x24,		///< Home
  KeyLeft					= 0x25,		///< Left arrow
  KeyUp					= 0x26,		///< Up arrow
  KeyRight				= 0x27,		///< Right arrow
  KeyDown					= 0x28,		///< Down arrow
  KeySelect				= 0x29,		///< Select (not available everywhere)
  KeyExecute				= 0x2B,		///< Execute (not available everywhere)
  KeyPrint				= 0x2C,		///< Print screen
  KeyInsert				= 0x2D,		///< Insert
  KeyDelete				= 0x2E,		///< Delete
  KeyHelp					= 0x2F,		///< Help (not available everywhere)
  Key0					= 0x30,		///< 0
  Key1					= 0x31,		///< 1
  Key2					= 0x32,		///< 2
  Key3					= 0x33,		///< 3
  Key4					= 0x34,		///< 4
  Key5					= 0x35,		///< 5
  Key6					= 0x36,		///< 6
  Key7					= 0x37,		///< 7
  Key8					= 0x38,		///< 8
  Key9					= 0x39,		///< 9
  KeyA					= 0x41,		///< A
  KeyB					= 0x42,		///< B
  KeyC					= 0x43,		///< C
  KeyD					= 0x44,		///< D
  KeyE					= 0x45,		///< E
  KeyF					= 0x46,		///< F
  KeyG					= 0x47,		///< G
  KeyH					= 0x48,		///< H
  KeyI					= 0x49,		///< I
  KeyJ					= 0x4A,		///< J
  KeyK					= 0x4B,		///< K
  KeyL					= 0x4C,		///< L
  KeyM					= 0x4D,		///< M
  KeyN					= 0x4E,		///< N
  KeyO					= 0x4F,		///< O
  KeyP					= 0x50,		///< P
  KeyQ					= 0x51,		///< Q
  KeyR					= 0x52,		///< R
  KeyS					= 0x53,		///< S
  KeyT					= 0x54,		///< T
  KeyU					= 0x55,		///< U
  KeyV					= 0x56,		///< V
  KeyW					= 0x57,		///< W
  KeyX					= 0x58,		///< X
  KeyY					= 0x59,		///< Y
  KeyZ					= 0x5A,		///< Z
  KeyNumpad0				= 0x60,		///< Numeric keypad 0
  KeyNumpad1				= 0x61,		///< Numeric keypad 1
  KeyNumpad2				= 0x62,		///< Numeric keypad 2
  KeyNumpad3				= 0x63,		///< Numeric keypad 3
  KeyNumpad4				= 0x64,		///< Numeric keypad 4
  KeyNumpad5				= 0x65,		///< Numeric keypad 5
  KeyNumpad6				= 0x66,		///< Numeric keypad 6
  KeyNumpad7				= 0x67,		///< Numeric keypad 7
  KeyNumpad8				= 0x68,		///< Numeric keypad 8
  KeyNumpad9				= 0x69,		///< Numeric keypad 9
  KeyNumpadMultiply		= 0x6A,		///< Numeric keypad "Multiply"
  KeyNumpadAdd			= 0x6B,		///< Numeric keypad "Add"
  KeyNumpadSeparator		= 0x6C,		///< Numeric keypad "Separator"
  KeyNumpadSubtract		= 0x6D,		///< Numeric keypad "Subtract"
  KeyNumpadDecimal		= 0x6E,		///< Numeric keypad "Decimal"
  KeyNumpadDivide			= 0x6F,		///< Numeric keypad "Divide"
  KeyF1					= 0x70,		///< F1
  KeyF2					= 0x71,		///< F2
  KeyF3					= 0x72,		///< F3
  KeyF4					= 0x73,		///< F4
  KeyF5					= 0x74,		///< F5
  KeyF6					= 0x75,		///< F6
  KeyF7					= 0x76,		///< F7
  KeyF8					= 0x77,		///< F8
  KeyF9					= 0x78,		///< F9
  KeyF10					= 0x79,		///< F10
  KeyF11					= 0x7A,		///< F11
  KeyF12					= 0x7B,		///< F12
  KeyNumLock				= 0x90,		///< Num lock
  KeyScrollLock			= 0x91,		///< Scroll lock
  KeyCircumflex			= 0xDC,		///< Circumflex (^)
  KeyLeftWindows			= 0x5B,		///< Left Windows key (natural keyboard)
  KeyRightWindows			= 0x5C,		///< Right Windows key (natural keyboard)
  KeyApplications			= 0x5D,		///< Applications key (natural keyboard)
  KeyF13					= 0x7C,		///< F13 key
  KeyF14					= 0x7D,		///< F14 key
  KeyF15					= 0x7E,		///< F15 key
  KeyF16					= 0x7F,		///< F16 key
  KeyF17					= 0x80,		///< F17 key
  KeyF18					= 0x81,		///< F18 key
  KeyF19					= 0x82,		///< F19 key
  KeyF20					= 0x83,		///< F20 key
  KeyF21					= 0x84,		///< F21 key
  KeyF22					= 0x85,		///< F22 key
  KeyF23					= 0x86,		///< F23 key
  KeyF24					= 0x87,		///< F24 key
  KeyLeftShift			= 0xA0,		///< Left shift key
  KeyRightShift			= 0xA1,		///< Right shift key
  KeyLeftControl			= 0xA2,		///< Left control key
  KeyRightControl			= 0xA3,		///< Right control key
  KeyVolumeMute			= 0xAD,		///< Volume mute key
  KeyVolumeDown			= 0xAE,		///< Volume down key
  KeyVolumeUp				= 0xAF,		///< Volume up key
  KeyMediaNextTrack		= 0xB0,		///< Next track key
  KeyMediaPreviousTrack	= 0xB1,		///< Previous track key
  KeyMediaStop			= 0xB2,		///< Stop media key
  KeyMediaPlayPause		= 0xB3,		///< Play/pause media key
  KeyAdd					= 0xBB,		///< For any country/region, the '+' key
  KeySeparator			= 0xBC,		///< For any country/region, the ',' key
  KeySubtract				= 0xBD,		///< For any country/region, the '-' key
  KeyDecimal				= 0xBE,		///< For any country/region, the '.' key
  KeyOEM1					= 0xBA,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
  KeyOEM2					= 0xBF,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
  KeyOEM3					= 0xC0,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
  KeyOEM4					= 0xDB,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
  KeyOEM5					= 0xDC,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
  KeyOEM6					= 0xDD,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
  KeyOEM7					= 0xDE,		///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
  KeyOEM8					= 0xDF,		///< Used for miscellaneous characters; it can vary by keyboard
  KeyOEM102				= 0xE2		///< Either the angle bracket key or the backslash key on the RT 102-key keyboard
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
