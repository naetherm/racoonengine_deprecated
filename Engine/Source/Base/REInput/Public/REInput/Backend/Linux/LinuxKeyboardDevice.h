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
#include "REInput/Backend/Linux/Linux.h"
#include "REInput/Backend/UpdateDevice.h"
#include "REInput/Input/Devices/Keyboard.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Keyboard implementation for Linux
*/
class LinuxKeyboardDevice : public UpdateDevice
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  LinuxKeyboardDevice() :
    m_pDisplay(XOpenDisplay(nullptr))
  {
    // Destroy device implementation automatically
    m_bDelete = true;

    // Init keys as 'not pressed'
    for (int i=0; i<32; i++)
      m_nKeys[i] = 0;
  }

  /**
  *  @brief
  *    Destructor
  */
  virtual ~LinuxKeyboardDevice() override
  {
    // Close display
    if (m_pDisplay)
      XCloseDisplay(m_pDisplay);
  }


  //[-------------------------------------------------------]
  //[ Public virtual UpdateDevice functions                 ]
  //[-------------------------------------------------------]
public:
  virtual void update() override
  {
    // Check if display and input device is valid
    if (m_pDisplay && m_pDevice) {
      // Get keyboard device
      Keyboard *pKeyboard = static_cast<Keyboard*>(m_pDevice);

      // Get keyboard state
      XLockDisplay(m_pDisplay);
      XQueryKeymap(m_pDisplay, m_nKeys);
      XUnlockDisplay(m_pDisplay);

      // Find changed keys
      for (int i=0; i<32*8; i++) {
        // Get state
        const int nState = (m_nKeys[i/8] >> (i%8)) & 1;

        // Get virtual key code without any modifier set (otherwise we would get different virtual key codes for one and the same physical key)
        //   Example: The user is pressing the "a"-key and the method returns "XK_a" as virtual key code.
        //            In case the user additionally is pressing the "shift"-key, the method returns "XK_A" (= a different virtual key code!).
        //            When the user releases the "a"-key on the keyboard, but is still holding the "shift"-key pressed down, then the input
        //            system wouldn't see that the "a"-key was released. The reason for this is, that the method returns "XK_A" and not "XK_a"
        //            which was the virtual key code returned when the user pressed the "a"-key in the first place.
        const KeySym nKeySym = XKeycodeToKeysym(m_pDisplay, i, 0);

        // Get button
        Button *pButton = getKeyboardKey(pKeyboard, nKeySym);
        if (pButton) {
          // Get button state
          const bool bPressed = (nState != 0);

          // Propagate changes
          if (pButton->isPressed() != bPressed)
          {
            pButton->setPressed(bPressed);
          }
        }
      }
    }
  }


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Get key for virtual key code
  *
  *  @param[in] pKeyboard
  *    Pointer to keyboard device, must be valid
  *  @param[in] nKeySym
  *    Virtual key code
  *
  *  @return
  *    Corresponding key, a null pointer if key code is invalid
  */
  [[nodiscard]] Button *getKeyboardKey(Keyboard *pKeyboard, KeySym nKeySym)
  {
    // Return key that corresponds to the given key code
    switch (nKeySym) {
      case XK_BackSpace:		return &pKeyboard->Backspace;
      case XK_Tab:			return &pKeyboard->Tab;
      case XK_Clear:			return &pKeyboard->Clear;
      case XK_Shift_L:		return &pKeyboard->Shift;
      case XK_Control_L:		return &pKeyboard->Control;
      case XK_Menu:			return &pKeyboard->Alt;
      case XK_Pause:			return &pKeyboard->Pause;
      case XK_Caps_Lock:		return &pKeyboard->CapsLock;
      case XK_Escape:			return &pKeyboard->Escape;
      case XK_space:			return &pKeyboard->Space;
      case XK_Prior:			return &pKeyboard->PageUp;
      case XK_Next:			return &pKeyboard->PageDown;
      case XK_End:			return &pKeyboard->End;
      case XK_Home:			return &pKeyboard->Home;
      case XK_Left:			return &pKeyboard->Left;
      case XK_Up:				return &pKeyboard->Up;
      case XK_Right:			return &pKeyboard->Right;
      case XK_Down:			return &pKeyboard->Down;
      case XK_Select:			return &pKeyboard->Select;
      case XK_Execute:		return &pKeyboard->Execute;
      case XK_Print:			return &pKeyboard->Print;
      case XK_Insert:			return &pKeyboard->Insert;
      case XK_Delete:			return &pKeyboard->Delete;
      case XK_Help:			return &pKeyboard->Help;
      case XK_0:				return &pKeyboard->Key0;
      case XK_1:				return &pKeyboard->Key1;
      case XK_2:				return &pKeyboard->Key2;
      case XK_3:				return &pKeyboard->Key3;
      case XK_4:				return &pKeyboard->Key4;
      case XK_5:				return &pKeyboard->Key5;
      case XK_6:				return &pKeyboard->Key6;
      case XK_7:				return &pKeyboard->Key7;
      case XK_8:				return &pKeyboard->Key8;
      case XK_9:				return &pKeyboard->Key9;
      case XK_a:				return &pKeyboard->A;
      case XK_b:				return &pKeyboard->B;
      case XK_c:				return &pKeyboard->C;
      case XK_d:				return &pKeyboard->D;
      case XK_e:				return &pKeyboard->E;
      case XK_f:				return &pKeyboard->F;
      case XK_g:				return &pKeyboard->G;
      case XK_h:				return &pKeyboard->H;
      case XK_i:				return &pKeyboard->I;
      case XK_j:				return &pKeyboard->J;
      case XK_k:				return &pKeyboard->K;
      case XK_l:				return &pKeyboard->L;
      case XK_m:				return &pKeyboard->M;
      case XK_n:				return &pKeyboard->N;
      case XK_o:				return &pKeyboard->O;
      case XK_p:				return &pKeyboard->P;
      case XK_q:				return &pKeyboard->Q;
      case XK_r:				return &pKeyboard->R;
      case XK_s:				return &pKeyboard->S;
      case XK_t:				return &pKeyboard->T;
      case XK_u:				return &pKeyboard->U;
      case XK_v:				return &pKeyboard->V;
      case XK_w:				return &pKeyboard->W;
      case XK_x:				return &pKeyboard->X;
      case XK_y:				return &pKeyboard->Y;
      case XK_z:				return &pKeyboard->Z;
      case XK_KP_0:			return &pKeyboard->Numpad0;
      case XK_KP_1:			return &pKeyboard->Numpad1;
      case XK_KP_2:			return &pKeyboard->Numpad2;
      case XK_KP_3:			return &pKeyboard->Numpad3;
      case XK_KP_4:			return &pKeyboard->Numpad4;
      case XK_KP_5:			return &pKeyboard->Numpad5;
      case XK_KP_6:			return &pKeyboard->Numpad6;
      case XK_KP_7:			return &pKeyboard->Numpad7;
      case XK_KP_8:			return &pKeyboard->Numpad8;
      case XK_KP_9:			return &pKeyboard->Numpad9;
      case XK_KP_Multiply:	return &pKeyboard->NumpadMultiply;
      case XK_KP_Add:			return &pKeyboard->NumpadAdd;
      case XK_KP_Separator:	return &pKeyboard->NumpadSeparator;
      case XK_KP_Subtract:	return &pKeyboard->NumpadSubtract;
      case XK_KP_Decimal:		return &pKeyboard->NumpadDecimal;
      case XK_KP_Divide:		return &pKeyboard->NumpadDivide;
      case XK_F1:				return &pKeyboard->F1;
      case XK_F2:				return &pKeyboard->F2;
      case XK_F3:				return &pKeyboard->F3;
      case XK_F4:				return &pKeyboard->F4;
      case XK_F5:				return &pKeyboard->F5;
      case XK_F6:				return &pKeyboard->F6;
      case XK_F7:				return &pKeyboard->F7;
      case XK_F8:				return &pKeyboard->F8;
      case XK_F9:				return &pKeyboard->F9;
      case XK_F10:			return &pKeyboard->F10;
      case XK_F11:			return &pKeyboard->F11;
      case XK_F12:			return &pKeyboard->F12;
      case XK_Num_Lock:		return &pKeyboard->NumLock;
      case XK_Scroll_Lock:	return &pKeyboard->ScrollLock;
      case XK_asciicircum:	return &pKeyboard->Circumflex;
      default:				return  nullptr;
    }
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  ::Display	*m_pDisplay;	///< X server display connection, can be a null pointer
  char		 m_nKeys[32];	///< State of all keys


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
