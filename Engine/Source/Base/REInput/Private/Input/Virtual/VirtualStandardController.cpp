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
#include "REInput/Input/Virtual/VirtualStandardController.h"
#include "REInput/Input/Devices/Keyboard.h"
#include "REInput/Input/Devices/Mouse.h"
#include "REInput/Input/Devices/WiiMote.h"
#include "REInput/Input/Devices/SpaceMouse.h"
#include "REInput/Input/Devices/Joystick.h"
#include "REInput/Input/Devices/SplitTouchPad.h"
#include "REInput/Input/Devices/SensorManager.h"
#include "REInput/Input/InputManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
VirtualStandardController::VirtualStandardController(InputManager& inputManager) :
  VirtualController(inputManager, "VirtualStandardController", "Standard virtual input controller"),
  // Mouse
  MouseX						(*this, "MouseX",						"X axis (movement data, no absolute data)"),
  MouseY						(*this, "MouseY",						"Y axis (movement data, no absolute data)"),
  MouseWheel					(*this, "MouseWheel",					"Mouse wheel (movement data, no absolute data)"),
  MouseLeft					(*this, "MouseLeft",					"Left mouse button (mouse button #0)"),
  MouseRight					(*this, "MouseRight",					"Right mouse button (mouse button #1)"),
  MouseMiddle					(*this, "MouseMiddle",					"Middle mouse button (mouse button #2)"),
  MouseButton4				(*this, "MouseButton4",					"Mouse button #4"),
  MouseButton5				(*this, "MouseButton5",					"Mouse button #5"),
  MouseButton6				(*this, "MouseButton6",					"Mouse button #6"),
  MouseButton7				(*this, "MouseButton7",					"Mouse button #7"),
  MouseButton8				(*this, "MouseButton8",					"Mouse button #8"),
  MouseButton9				(*this, "MouseButton9",					"Mouse button #9"),
  MouseButton10				(*this, "MouseButton10",				"Mouse button #10"),
  MouseButton11				(*this, "MouseButton11",				"Mouse button #11"),
  MouseButton12				(*this, "MouseButton12",				"Mouse button #12"),
  // Keyboard
  KeyboardBackspace			(*this, "KeyboardBackspace",			"Backspace",																	0x08),
  KeyboardTab					(*this, "KeyboardTab",					"Tabulator",																	0x09),
  KeyboardClear				(*this, "KeyboardClear",				"Clear (not available everywhere)",												0x00),
  KeyboardReturn				(*this, "KeyboardReturn",				"Return (often the same as \"Enter\")",											0x0D),
  KeyboardShift				(*this, "KeyboardShift",				"Shift",																		0x00),
  KeyboardControl				(*this, "KeyboardControl",				"Control (\"Ctrl\")",															0x00),
  KeyboardAlt					(*this, "KeyboardAlt",					"Alt",																			0x00),
  KeyboardPause				(*this, "KeyboardPause",				"Pause",																		0x00),
  KeyboardCapsLock			(*this, "KeyboardCapsLock",				"Caps lock",																	0x00),
  KeyboardEscape				(*this, "KeyboardEscape",				"Escape",																		0x1B),
  KeyboardSpace				(*this, "KeyboardSpace",				"Space",																		0x20),
  KeyboardPageUp				(*this, "KeyboardPageUp",				"Page up",																		0x00),
  KeyboardPageDown			(*this, "KeyboardPageDown",				"Page down",																	0x00),
  KeyboardEnd					(*this, "KeyboardEnd",					"End",																			0x00),
  KeyboardHome				(*this, "KeyboardHome",					"Home",																			0x00),
  KeyboardLeft				(*this, "KeyboardLeft",					"Left arrow",																	0x00),
  KeyboardUp					(*this, "KeyboardUp",					"Up arrow",																		0x00),
  KeyboardRight				(*this, "KeyboardRight",				"Right arrow",																	0x00),
  KeyboardDown				(*this, "KeyboardDown",					"Down arrow",																	0x00),
  KeyboardSelect				(*this, "KeyboardSelect",				"Select (not available everywhere)",											0x00),
  KeyboardExecute				(*this, "KeyboardExecute",				"Execute (not available everywhere)",											0x00),
  KeyboardPrint				(*this, "KeyboardPrint",				"Print screen",																	0x00),
  KeyboardInsert				(*this, "KeyboardInsert",				"Insert",																		0x00),
  KeyboardDelete				(*this, "KeyboardDelete",				"Delete",																		0x7F),
  KeyboardHelp				(*this, "KeyboardHelp",					"Help (not available everywhere)",												0x00),
  Keyboard0					(*this, "Keyboard0",					"0",																			'0'),
  Keyboard1					(*this, "Keyboard1",					"1",																			'1'),
  Keyboard2					(*this, "Keyboard2",					"2",																			'2'),
  Keyboard3					(*this, "Keyboard3",					"3",																			'3'),
  Keyboard4					(*this, "Keyboard4",					"4",																			'4'),
  Keyboard5					(*this, "Keyboard5",					"5",																			'5'),
  Keyboard6					(*this, "Keyboard6",					"6",																			'6'),
  Keyboard7					(*this, "Keyboard7",					"7",																			'7'),
  Keyboard8					(*this, "Keyboard8",					"8",																			'8'),
  Keyboard9					(*this, "Keyboard9",					"9",																			'9'),
  KeyboardA					(*this, "KeyboardA",					"A",																			'a'),
  KeyboardB					(*this, "KeyboardB",					"B",																			'b'),
  KeyboardC					(*this, "KeyboardC",					"C",																			'c'),
  KeyboardD					(*this, "KeyboardD",					"D",																			'd'),
  KeyboardE					(*this, "KeyboardE",					"E",																			'e'),
  KeyboardF					(*this, "KeyboardF",					"F",																			'f'),
  KeyboardG					(*this, "KeyboardG",					"G",																			'g'),
  KeyboardH					(*this, "KeyboardH",					"H",																			'h'),
  KeyboardI					(*this, "KeyboardI",					"I",																			'i'),
  KeyboardJ					(*this, "KeyboardJ",					"J",																			'j'),
  KeyboardK					(*this, "KeyboardK",					"K",																			'k'),
  KeyboardL					(*this, "KeyboardL",					"L",																			'l'),
  KeyboardM					(*this, "KeyboardM",					"M",																			'm'),
  KeyboardN					(*this, "KeyboardN",					"N",																			'n'),
  KeyboardO					(*this, "KeyboardO",					"O",																			'o'),
  KeyboardP					(*this, "KeyboardP",					"P",																			'p'),
  KeyboardQ					(*this, "KeyboardQ",					"Q",																			'q'),
  KeyboardR					(*this, "KeyboardR",					"R",																			'r'),
  KeyboardS					(*this, "KeyboardS",					"S",																			's'),
  KeyboardT					(*this, "KeyboardT",					"T",																			't'),
  KeyboardU					(*this, "KeyboardU",					"U",																			'u'),
  KeyboardV					(*this, "KeyboardV",					"V",																			'v'),
  KeyboardW					(*this, "KeyboardW",					"W",																			'w'),
  KeyboardX					(*this, "KeyboardX",					"X",																			'x'),
  KeyboardY					(*this, "KeyboardY",					"Y",																			'y'),
  KeyboardZ					(*this, "KeyboardZ",					"Z",																			'z'),
  KeyboardNumpad0				(*this, "KeyboardNumpad0",				"Numpad 0",																		'0'),
  KeyboardNumpad1				(*this, "KeyboardNumpad1",				"Numpad 1",																		'1'),
  KeyboardNumpad2				(*this, "KeyboardNumpad2",				"Numpad 2",																		'2'),
  KeyboardNumpad3				(*this, "KeyboardNumpad3",				"Numpad 3",																		'3'),
  KeyboardNumpad4				(*this, "KeyboardNumpad4",				"Numpad 4",																		'4'),
  KeyboardNumpad5				(*this, "KeyboardNumpad5",				"Numpad 5",																		'5'),
  KeyboardNumpad6				(*this, "KeyboardNumpad6",				"Numpad 6",																		'6'),
  KeyboardNumpad7				(*this, "KeyboardNumpad7",				"Numpad 7",																		'7'),
  KeyboardNumpad8				(*this, "KeyboardNumpad8",				"Numpad 8",																		'8'),
  KeyboardNumpad9				(*this, "KeyboardNumpad9",				"Numpad 9",																		'9'),
  KeyboardNumpadMultiply		(*this, "KeyboardNumpadMultiply",		"Numpad multiply",																'*'),
  KeyboardNumpadAdd			(*this, "KeyboardNumpadAdd",			"Numpad add",																	'+'),
  KeyboardNumpadSeparator		(*this, "KeyboardNumpadSeparator",		"Numpad separator",																','),
  KeyboardNumpadSubtract		(*this, "KeyboardNumpadSubtract",		"Numpad subtract",																'-'),
  KeyboardNumpadDecimal		(*this, "KeyboardNumpadDecimal",		"Numpad decimal",																'.'),
  KeyboardNumpadDivide		(*this, "KeyboardNumpadDivide",			"Numpad divide",																'/'),
  KeyboardF1					(*this, "KeyboardF1",					"F1",																			0x00),
  KeyboardF2					(*this, "KeyboardF2",					"F2",																			0x00),
  KeyboardF3					(*this, "KeyboardF3",					"F3",																			0x00),
  KeyboardF4					(*this, "KeyboardF4",					"F4",																			0x00),
  KeyboardF5					(*this, "KeyboardF5",					"F5",																			0x00),
  KeyboardF6					(*this, "KeyboardF6",					"F6",																			0x00),
  KeyboardF7					(*this, "KeyboardF7",					"F7",																			0x00),
  KeyboardF8					(*this, "KeyboardF8",					"F8",																			0x00),
  KeyboardF9					(*this, "KeyboardF9",					"F9",																			0x00),
  KeyboardF10					(*this, "KeyboardF10",					"F10",																			0x00),
  KeyboardF11					(*this, "KeyboardF11",					"F11",																			0x00),
  KeyboardF12					(*this, "KeyboardF12",					"F12",																			0x00),
  KeyboardNumLock				(*this, "KeyboardNumLock",				"Num lock",																		0x00),
  KeyboardScrollLock			(*this, "KeyboardScrollLock",			"Scroll lock",																	0x00),
  KeyboardCircumflex			(*this, "KeyboardCircumflex",			"Circumflex (^)",																0x00),
  KeyboardLeftWindows			(*this, "KeyboardLeftWindows",			"Left Windows key",																0x00),
  KeyboardRightWindows		(*this, "KeyboardRightWindows",			"Right Windows key",															0x00),
  KeyboardApplications		(*this, "KeyboardApplications",			"Applications key (natural keyboard)",											0x00),
  KeyboardF13					(*this, "KeyboardF13",					"F13",																			0x00),
  KeyboardF14					(*this, "KeyboardF14",					"F14",																			0x00),
  KeyboardF15					(*this, "KeyboardF15",					"F15",																			0x00),
  KeyboardF16					(*this, "KeyboardF16",					"F16",																			0x00),
  KeyboardF17					(*this, "KeyboardF17",					"F17",																			0x00),
  KeyboardF18					(*this, "KeyboardF18",					"F18",																			0x00),
  KeyboardF19					(*this, "KeyboardF19",					"F19",																			0x00),
  KeyboardF20					(*this, "KeyboardF20",					"F20",																			0x00),
  KeyboardF21					(*this, "KeyboardF21",					"F21",																			0x00),
  KeyboardF22					(*this, "KeyboardF22",					"F22",																			0x00),
  KeyboardF23					(*this, "KeyboardF23",					"F23",																			0x00),
  KeyboardF24					(*this, "KeyboardF24",					"F24",																			0x00),
  KeyboardLeftShift			(*this, "KeyboardLeftShift",			"Left shift",																	0x00),
  KeyboardRightShift			(*this, "KeyboardRightShift",			"Right shift",																	0x00),
  KeyboardLeftControl			(*this, "KeyboardLeftControl",			"Left control",																	0x00),
  KeyboardRightControl		(*this, "KeyboardRightControl",			"Right control",																0x00),
  KeyboardVolumeMute			(*this, "KeyboardVolumeMute",			"Volume mute",																	0x00),
  KeyboardVolumeDown			(*this, "KeyboardVolumeDown",			"Volume down",																	0x00),
  KeyboardVolumeUp			(*this, "KeyboardVolumeUp",				"Volume up",																	0x00),
  KeyboardMediaNextTrack		(*this, "KeyboardMediaNextTrack",		"Media next track",																0x00),
  KeyboardMediaPreviousTrack	(*this, "KeyboardMediaPreviousTrack",	"Media previous track",															0x00),
  KeyboardMediaStop			(*this, "KeyboardMediaStop",			"Media stop",																	0x00),
  KeyboardMediaPlayPause		(*this, "KeyboardMediaPlayPause",		"Media play pause",																0x00),
  KeyboardAdd					(*this, "KeyboardAdd",					"For any country/region, the '+' key",											 '+'),
  KeyboardSeparator			(*this, "KeyboardSeparator",			"For any country/region, the ',' key",											 ','),
  KeyboardSubtract			(*this, "KeyboardSubtract",				"For any country/region, the '-' key",											 '-'),
  KeyboardDecimal				(*this, "KeyboardDecimal",				"For any country/region, the '.' key",											 '.'),
  KeyboardOEM1				(*this, "KeyboardOEM1",					"For the US standard keyboard, the ';:' key",									0x00),
  KeyboardOEM2				(*this, "KeyboardOEM2",					"For the US standard keyboard, the '/?' key",									0x00),
  KeyboardOEM3				(*this, "KeyboardOEM3",					"For the US standard keyboard, the '`~' key",									0x00),
  KeyboardOEM4				(*this, "KeyboardOEM4",					"For the US standard keyboard, the '[{' key",									0x00),
  KeyboardOEM5				(*this, "KeyboardOEM5",					"For the US standard keyboard, the 'backslash|' key",							0x00),
  KeyboardOEM6				(*this, "KeyboardOEM6",					"For the US standard keyboard, the ']}' key",									0x00),
  KeyboardOEM7				(*this, "KeyboardOEM7",					"For the US standard keyboard, the 'single-quote/double-quote' key",			0x00),
  KeyboardOEM8				(*this, "KeyboardOEM8",					"Used for miscellaneous characters; it can vary by keyboard",					0x00),
  KeyboardOEM102				(*this, "KeyboardOEM102",				"Either the angle bracket key or the backslash key on the RT 102-key keyboard",	0x00),
  // Main character controls
  TransX						(*this, "TransX",						"X translation axis: Strafe left/right (+/-)"),
  TransY						(*this, "TransY",						"Y translation axis: Move up/down (+/-)"),
  TransZ						(*this, "TransZ",						"Z translation axis: Move forwards/backwards (+/-)"),
  Pan							(*this, "Pan",							"Keep pressed to pan",															0x00),
  PanX						(*this, "PanX",							"X pan translation axis: Strafe left/right (+/-)"),
  PanY						(*this, "PanY",							"Y pan translation axis: Move up/down (+/-)"),
  PanZ						(*this, "PanZ",							"Z pan translation axis: Move forwards/backwards (+/-)"),
  RotX						(*this, "RotX",							"X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)"),
  RotY						(*this, "RotY",							"Y rotation axis: Yaw (also called 'heading') change is turning to the left or right"),
  RotZ						(*this, "RotZ",							"Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down"),
  Rotate						(*this, "Rotate",						"Keep pressed to rotate",														0x00),
  RotateSlow					(*this, "RotateSlow",					"Keep pressed to rotate slowly",												0x00),
  Forward						(*this, "Forward",						"Move forwards",																0x00),
  Backward					(*this, "Backward",						"Move backwards",																0x00),
  Left						(*this, "Left",							"Move (rotate) left",															0x00),
  Right						(*this, "Right",						"Move (rotate) right",															0x00),
  StrafeLeft					(*this, "StrafeLeft",					"Strafe left",																	0x00),
  StrafeRight					(*this, "StrafeRight",					"Strafe right",																	0x00),
  Up							(*this, "Up",							"Move up",																		0x00),
  Down						(*this, "Down",							"Move down",																	0x00),
  Run							(*this, "Run",							"Keep pressed to run",															0x00),
  Sneak						(*this, "Sneak",						"Keep pressed to sneak",														0x00),
  Crouch						(*this, "Crouch",						"Keep pressed to crouch",														0x00),
  Jump						(*this, "Jump",							"Jump",																			0x00),
  Zoom						(*this, "Zoom",							"Keep pressed to zoom",															0x00),
  ZoomAxis					(*this, "ZoomAxis",						"Zoom axis to zoom in or out (+/-)"),
  //Button1						(*this, "Button1",						"Button for action #1",															0x00),
  //Button2						(*this, "Button2",						"Button for action #2",															0x00),
  //Button3						(*this, "Button3",						"Button for action #3",															0x00),
  //Button4						(*this, "Button4",						"Button for action #4",															0x00),
  //Button5						(*this, "Button5",						"Button for action #5",															0x00),
  // Interaction
  Pickup						(*this, "Pickup",						"Keep pressed to pickup",														0x00),
  Throw						(*this, "Throw",						"Throw the picked object",														0x00),
  IncreaseForce				(*this, "IncreaseForce",				"Keep pressed to increase the force applied to the picked object",				0x00),
  DecreaseForce				(*this, "DecreaseForce",				"Keep pressed to decrease the force applied to the picked object",				0x00),
  PushPull					(*this, "PushPull",						"Used to push/pull the picked object")
{
  connectToDevices();
}


//[-------------------------------------------------------]
//[ Public virtual VirtualStandardController functions    ]
//[-------------------------------------------------------]
void VirtualStandardController::connectToDevices()
{
  // Connect mouse
  connectAll(mInputManager.getMouse(), "Mouse", "");

  // Connect keyboard
  connectAll(mInputManager.getKeyboard(), "Keyboard", "");

  // Connect character controls
  std::vector<Device*> &lstDevices = mInputManager.getDevices();
  for (RECore::uint32 i=0; i<lstDevices.size(); i++) {
    // Get device
    Device *pDevice = lstDevices[i];

    // Mouse
    if (pDevice->getName() == "Mouse") {
      // Get mouse
      Mouse *pMouse = static_cast<Mouse*>(pDevice);

      // Movement
      // Keep pressed to pan
      connect("Pan",			pMouse->Middle);
      // RotX: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
      connect("RotX",			pMouse->X);
      // RotY: Yaw (also called 'heading') change is turning to the left or right
      connect("RotY",			pMouse->Y);
      // Keep pressed to rotate
      connect("Rotate",		pMouse->Right);
      // Pan x
      connect("PanX",			pMouse->X, -0.05f);
      // Pan y
      connect("PanY",			pMouse->Y, -0.05f);

      // Zoom
      connect("Zoom",			pMouse->Middle);
      connect("ZoomAxis",		pMouse->Wheel, 0.01f);
      connect("ZoomAxis",		pMouse->Y, -0.1f);

      // Buttons
      connect("MouseWheel",	pMouse->Wheel);
      connect("Button1",		pMouse->Left);
      connect("Button2",		pMouse->Right);
      connect("Button3",		pMouse->Middle);
      //connect("Button4",		pMouse->Button4);
      //connect("Button5",		pMouse->Button5);

      // Interaction
      connect("Pickup",		pMouse->Middle);
      connect("PushPull",		pMouse->Wheel, 0.001f);
    }

      // Keyboard
    else if (pDevice->getName() == "Keyboard") {
      // Get keyboard
      Keyboard *pKeyboard = static_cast<Keyboard*>(pDevice);

      // Keep pressed to rotate slowly
      connect("RotateSlow",		pKeyboard->Q);

      // WASD
      connect("Forward",			pKeyboard->W);
      connect("Backward",			pKeyboard->S);
      connect("StrafeLeft",		pKeyboard->A);
      connect("StrafeRight",		pKeyboard->D);
      connect("Left",				pKeyboard->Q);
      connect("Right",			pKeyboard->E);

      // Cursor keys
      connect("Forward",			pKeyboard->Up);
      connect("Backward",			pKeyboard->Down);
      connect("StrafeLeft",		pKeyboard->Left);
      connect("StrafeRight",		pKeyboard->Right);
      connect("Left",				pKeyboard->Left);
      connect("Right",			pKeyboard->Right);

      // Look up/down
      connect("Up",				pKeyboard->PageUp);
      connect("Down",				pKeyboard->PageDown);

      // Run/sneak/crouch/jump
      connect("Run",				pKeyboard->Shift);
      connect("Sneak",			pKeyboard->Control);
      connect("Crouch",			pKeyboard->C);
      connect("Jump",				pKeyboard->Space);

      // Buttons
      connect("Button1",			pKeyboard->Space);
      connect("Button2",			pKeyboard->Return);
      connect("Button3",			pKeyboard->Backspace);
      connect("Button4",			pKeyboard->Alt);
      connect("Button5",			pKeyboard->Circumflex);

      // Interaction
      connect("Throw",			pKeyboard->T);
      connect("IncreaseForce",	pKeyboard->Shift);
      connect("DecreaseForce",	pKeyboard->Control);
    }

      // Joystick
    else if (pDevice->getName().find("Joystick") != std::string::npos) {
      // Get joystick
      Joystick *pJoystick = static_cast<Joystick*>(pDevice);
      static constexpr float ROTATION_SCALE = 500.0f;

      // TODO(naetherm) Movement

      // Rotation
      // RotX: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
      connect("RotX",		pJoystick->X, ROTATION_SCALE);
      // RotY: Yaw (also called 'heading') change is turning to the left or right
      connect("RotY",		pJoystick->Y, ROTATION_SCALE);

      // Buttons
      //connect("Button1",	pJoystick->Button0);
      //connect("Button2",	pJoystick->Button1);
      //connect("Button3",	pJoystick->Button2);
      //connect("Button4",	pJoystick->Button3);
      //connect("Button5",	pJoystick->Button4);
    }

      // SpaceMouse
    else if (pDevice->getName().find("SpaceMouse") != std::string::npos) {
      // Get space mouse
      SpaceMouse *pSpaceMouse = static_cast<SpaceMouse*>(pDevice);
      static constexpr float ROTATION_SCALE = 1.5f;
      static constexpr float TRANSFORM_SCALE = -0.004f;

      // Movement
      // RotX: Yaw (also called 'heading', change is turning to the left or right)
      connect("RotX",		pSpaceMouse->RotZ,    ROTATION_SCALE);
      // RotY: Pitch (also called 'bank', change is moving the nose down and the tail up or vice-versa)
      connect("RotY",		pSpaceMouse->RotX,   -ROTATION_SCALE);
      // RotZ: Roll (also called 'attitude') change is moving one wingtip up and the other down
      connect("RotZ",		pSpaceMouse->RotY,   -ROTATION_SCALE);
      // X translation axis: Strafe left/right (+/-)
      connect("TransX",	pSpaceMouse->TransX, TRANSFORM_SCALE);
      // Y translation axis: Move up/down (+/-)
      connect("TransY",	pSpaceMouse->TransZ, TRANSFORM_SCALE);
      // Z translation axis: Move forwards/backwards (+/-)
      connect("TransZ",	pSpaceMouse->TransY, TRANSFORM_SCALE);
      // X pan translation axis: Strafe left/right (+/-)
      connect("PanX",		pSpaceMouse->TransX, TRANSFORM_SCALE);
      // Y pan translation axis: Move up/down (+/-)
      connect("PanY",		pSpaceMouse->TransZ, TRANSFORM_SCALE);
      // Z pan translation axis: Move forwards/backwards (+/-)
      connect("PanZ",		pSpaceMouse->TransY, TRANSFORM_SCALE);

      // Zoom
      connect("ZoomAxis",	pSpaceMouse->TransY, TRANSFORM_SCALE);

      // Buttons
      //connect("Button1",	pSpaceMouse->Button0);
      //connect("Button2",	pSpaceMouse->Button1);
      //connect("Button3",	pSpaceMouse->Button2);
      //connect("Button4",	pSpaceMouse->Button3);
      //connect("Button5",	pSpaceMouse->Button4);
    }

      // Splitscreen touchpad device
    else if (pDevice->getName().find("SplitTouchPad") != std::string::npos) {
      // Get splitscreen touchpad device
      SplitTouchPad *pSplitTouchPad = static_cast<SplitTouchPad*>(pDevice);

      // Movement
      // RotX: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
      connect("RotY",		pSplitTouchPad->RightX, -0.5f);
      // RotY: Yaw (also called 'heading') change is turning to the left or right
      connect("RotX",		pSplitTouchPad->RightY, 0.5f);
      // X translation axis: Strafe left/right (+/-)
      connect("TransX",	pSplitTouchPad->LeftX,  -0.01f);
      // Y translation axis: Move up/down (+/-)
      connect("TransZ",	pSplitTouchPad->LeftY,  -0.01f);
    }

      // WiiMote
    else if (pDevice->getName().find("WiiMote") != std::string::npos) {
      // Get WiiMote
      WiiMote *pWiiMote = static_cast<WiiMote*>(pDevice);

      // Movement
      // TODO(naetherm) We need some more logic here to calculate movement from WiiMote data

      // Cursor keys
      connect("Forward",	pWiiMote->ButtonUp);
      connect("Backward",	pWiiMote->ButtonDown);
      connect("Left",		pWiiMote->ButtonLeft);
      connect("Right",	pWiiMote->ButtonRight);

      // Buttons
      connect("Button1",	pWiiMote->ButtonA);
      connect("Button2",	pWiiMote->ButtonB);
      //connect("Button3",	pWiiMote->Button1);
      //connect("Button4",	pWiiMote->Button2);
      connect("Button5",	pWiiMote->ButtonHome);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
