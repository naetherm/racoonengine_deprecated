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
#include "REInput/Input/Devices/Keyboard.h"
#include "REInput/Backend/UpdateDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
Keyboard::Keyboard(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl) :
  Device(inputManager, sName, "Keyboard input controller", pImpl),
  Backspace			(*this, "Backspace",			"Backspace",																	0x08),
  Tab					(*this, "Tab",					"Tabulator",																	0x09),
  Clear				(*this, "Clear",				"Clear (not available everywhere)",												0x00),
  Return				(*this, "Return",				"Return (often the same as \"Enter\")",											0x0D),
  Shift				(*this, "Shift",				"Shift",																		0x00),
  Control				(*this, "Control",				"Control (\"Ctrl\")",															0x00),
  Alt					(*this, "Alt",					"Alt",																			0x00),
  Pause				(*this, "Pause",				"Pause",																		0x00),
  CapsLock			(*this, "CapsLock",				"Caps lock",																	0x00),
  Escape				(*this, "Escape",				"Escape",																		0x1B),
  Space				(*this, "Space",				"Space",																		0x20),
  PageUp				(*this, "PageUp",				"Page up",																		0x00),
  PageDown			(*this, "PageDown",				"Page down",																	0x00),
  End					(*this, "End",					"End",																			0x00),
  Home				(*this, "Home",					"Home",																			0x00),
  Left				(*this, "Left",					"Left arrow",																	0x00),
  Up					(*this, "Up",					"Up arrow",																		0x00),
  Right				(*this, "Right",				"Right arrow",																	0x00),
  Down				(*this, "Down",					"Down arrow",																	0x00),
  Select				(*this, "Select",				"Select (not available everywhere)",											0x00),
  Execute				(*this, "Execute",				"Execute (not available everywhere)",											0x00),
  Print				(*this, "Print",				"Print screen",																	0x00),
  Insert				(*this, "Insert",				"Insert",																		0x00),
  Delete				(*this, "Delete",				"Delete",																		0x7F),
  Help				(*this, "Help",					"Help (not available everywhere)",												0x00),
  Key0				(*this, "0",					"0",																			 '0'),
  Key1				(*this, "1",					"1",																			 '1'),
  Key2				(*this, "2",					"2",																			 '2'),
  Key3				(*this, "3",					"3",																			 '3'),
  Key4				(*this, "4",					"4",																			 '4'),
  Key5				(*this, "5",					"5",																			 '5'),
  Key6				(*this, "6",					"6",																			 '6'),
  Key7				(*this, "7",					"7",																			 '7'),
  Key8				(*this, "8",					"8",																			 '8'),
  Key9				(*this, "9",					"9",																			 '9'),
  A					(*this, "A",					"A",																			 'a'),
  B					(*this, "B",					"B",																			 'b'),
  C					(*this, "C",					"C",																			 'c'),
  D					(*this, "D",					"D",																			 'd'),
  E					(*this, "E",					"E",																			 'e'),
  F					(*this, "F",					"F",																			 'f'),
  G					(*this, "G",					"G",																			 'g'),
  H					(*this, "H",					"H",																			 'h'),
  I					(*this, "I",					"I",																			 'i'),
  J					(*this, "J",					"J",																			 'j'),
  K					(*this, "K",					"K",																			 'k'),
  L					(*this, "L",					"L",																			 'l'),
  M					(*this, "M",					"M",																			 'm'),
  N					(*this, "N",					"N",																			 'n'),
  O					(*this, "O",					"O",																			 'o'),
  P					(*this, "P",					"P",																			 'p'),
  Q					(*this, "Q",					"Q",																			 'q'),
  R					(*this, "R",					"R",																			 'r'),
  S					(*this, "S",					"S",																			 's'),
  T					(*this, "T",					"T",																			 't'),
  U					(*this, "U",					"U",																			 'u'),
  V					(*this, "V",					"V",																			 'v'),
  W					(*this, "W",					"W",																			 'w'),
  X					(*this, "X",					"X",																			 'x'),
  Y					(*this, "Y",					"Y",																			 'y'),
  Z					(*this, "Z",					"Z",																			 'z'),
  Numpad0				(*this, "Numpad0",				"Numpad 0",																		 '0'),
  Numpad1				(*this, "Numpad1",				"Numpad 1",																		 '1'),
  Numpad2				(*this, "Numpad2",				"Numpad 2",																		 '2'),
  Numpad3				(*this, "Numpad3",				"Numpad 3",																		 '3'),
  Numpad4				(*this, "Numpad4",				"Numpad 4",																		 '4'),
  Numpad5				(*this, "Numpad5",				"Numpad 5",																		 '5'),
  Numpad6				(*this, "Numpad6",				"Numpad 6",																		 '6'),
  Numpad7				(*this, "Numpad7",				"Numpad 7",																		 '7'),
  Numpad8				(*this, "Numpad8",				"Numpad 8",																		 '8'),
  Numpad9				(*this, "Numpad9",				"Numpad 9",																		 '9'),
  NumpadMultiply		(*this, "NumpadMultiply",		"Numpad multiply",																 '*'),
  NumpadAdd			(*this, "NumpadAdd",			"Numpad add",																	 '+'),
  NumpadSeparator		(*this, "NumpadSeparator",		"Numpad separator",																 ','),
  NumpadSubtract		(*this, "NumpadSubtract",		"Numpad subtract",																 '-'),
  NumpadDecimal		(*this, "NumpadDecimal",		"Numpad decimal",																 '.'),
  NumpadDivide		(*this, "NumpadDivide",			"Numpad divide",																 '/'),
  F1					(*this, "F1",					"F1",																			0x00),
  F2					(*this, "F2",					"F2",																			0x00),
  F3					(*this, "F3",					"F3",																			0x00),
  F4					(*this, "F4",					"F4",																			0x00),
  F5					(*this, "F5",					"F5",																			0x00),
  F6					(*this, "F6",					"F6",																			0x00),
  F7					(*this, "F7",					"F7",																			0x00),
  F8					(*this, "F8",					"F8",																			0x00),
  F9					(*this, "F9",					"F9",																			0x00),
  F10					(*this, "F10",					"F10",																			0x00),
  F11					(*this, "F11",					"F11",																			0x00),
  F12					(*this, "F12",					"F12",																			0x00),
  NumLock				(*this, "NumLock",				"Num lock",																		0x00),
  ScrollLock			(*this, "ScrollLock",			"Scroll lock",																	0x00),
  Circumflex			(*this, "Circumflex",			"Circumflex (^)",																0x00),
  LeftWindows			(*this, "LeftWindows",			"Left Windows key (natural keyboard)",											0x00),
  RightWindows		(*this, "RightWindows",			"Right Windows key (natural keyboard)",											0x00),
  Applications		(*this, "Applications",			"Applications key (natural keyboard)",											0x00),
  F13					(*this, "F13",					"F13",																			0x00),
  F14					(*this, "F14",					"F14",																			0x00),
  F15					(*this, "F15",					"F15",																			0x00),
  F16					(*this, "F16",					"F16",																			0x00),
  F17					(*this, "F17",					"F17",																			0x00),
  F18					(*this, "F18",					"F18",																			0x00),
  F19					(*this, "F19",					"F19",																			0x00),
  F20					(*this, "F20",					"F20",																			0x00),
  F21					(*this, "F21",					"F21",																			0x00),
  F22					(*this, "F22",					"F22",																			0x00),
  F23					(*this, "F23",					"F23",																			0x00),
  F24					(*this, "F24",					"F24",																			0x00),
  LeftShift			(*this, "LeftShift",			"Left shift",																	0x00),
  RightShift			(*this, "RightShift",			"Right shift",																	0x00),
  LeftControl			(*this, "LeftControl",			"Left control",																	0x00),
  RightControl		(*this, "RightControl",			"Right control",																0x00),
  VolumeMute			(*this, "VolumeMute",			"Volume mute",																	0x00),
  VolumeDown			(*this, "VolumeDown",			"Volume down",																	0x00),
  VolumeUp			(*this, "VolumeUp",				"Volume up",																	0x00),
  MediaNextTrack		(*this, "MediaNextTrack",		"Media next track",																0x00),
  MediaPreviousTrack	(*this, "MediaPreviousTrack",	"Media previous track",															0x00),
  MediaStop			(*this, "MediaStop",			"Media stop",																	0x00),
  MediaPlayPause		(*this, "MediaPlayPause",		"Media play pause",																0x00),
  Add					(*this, "Add",					"For any country/region, the '+' key",											 '+'),
  Separator			(*this, "Separator",			"For any country/region, the ',' key",											 ','),
  Subtract			(*this, "Subtract",				"For any country/region, the '-' key",											 '-'),
  Decimal				(*this, "Decimal",				"For any country/region, the '.' key",											 '.'),
  OEM1				(*this, "OEM1",					"For the US standard keyboard, the ';:' key",									0x00),
  OEM2				(*this, "OEM2",					"For the US standard keyboard, the '/?' key",									0x00),
  OEM3				(*this, "OEM3",					"For the US standard keyboard, the '`~' key",									0x00),
  OEM4				(*this, "OEM4",					"For the US standard keyboard, the '[{' key",									0x00),
  OEM5				(*this, "OEM5",					"For the US standard keyboard, the 'backslash|' key",							0x00),
  OEM6				(*this, "OEM6",					"For the US standard keyboard, the ']}' key",									0x00),
  OEM7				(*this, "OEM7",					"For the US standard keyboard, the 'single-quote/double-quote' key",			0x00),
  OEM8				(*this, "OEM8",					"Used for miscellaneous characters; it can vary by keyboard",					0x00),
  OEM102				(*this, "OEM102",				"Either the angle bracket key or the backslash key on the RT 102-key keyboard",	0x00)
{
  // Nothing here
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void Keyboard::update()
{
  // Update device backend
  if (m_pImpl && m_pImpl->getDeviceBackendType() == DeviceBackendType::UPDATE_DEVICE)
  {
    static_cast<UpdateDevice*>(m_pImpl)->update();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
