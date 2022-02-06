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
#include "REInput/Input/Devices/Mouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse implementation for Linux
*/
class LinuxMouseDevice : public UpdateDevice
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  inline LinuxMouseDevice():
    m_pDisplay(XOpenDisplay(nullptr)),
    m_nMouseX(0),
    m_nMouseY(0)
  {
    // Destroy device implementation automatically
    m_bDelete = true;
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~LinuxMouseDevice() override
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
      // Get mouse device
      Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

      // Get mouse state
      ::Window 	 nRootWindow, nChildWindow;
      int 		 nRootX, nRootY, nChildX, nChildY;
      unsigned int nMask;
      XLockDisplay(m_pDisplay);
      XQueryPointer(m_pDisplay, DefaultRootWindow(m_pDisplay), &nRootWindow, &nChildWindow, &nRootX, &nRootY, &nChildX, &nChildY, &nMask);
      XUnlockDisplay(m_pDisplay);

      // Update axes
      int nDeltaX = nRootX - m_nMouseX;
      int nDeltaY = nRootY - m_nMouseY;
      m_nMouseX = nRootX;
      m_nMouseY = nRootY;
      if (pMouse->X.getValue() != nDeltaX)
        pMouse->X.setValue(nDeltaX, true);
      if (pMouse->Y.getValue() != nDeltaY)
        pMouse->Y.setValue(nDeltaY, true);

      // Update buttons
      bool bPressed = ((nMask & Button1Mask) != 0);
      if (pMouse->Left.isPressed() != bPressed)
      {
        pMouse->Left.setPressed(bPressed);
      }
      bPressed = ((nMask & Button2Mask) != 0);
      if (pMouse->Right.isPressed() != bPressed)
      {
        pMouse->Right.setPressed(bPressed);
      }
      bPressed = ((nMask & Button3Mask) != 0);
      if (pMouse->Middle.isPressed() != bPressed)
      {
        pMouse->Middle.setPressed(bPressed);
      }
    }
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  ::Display *m_pDisplay;	///< X server display connection, can be a null pointer
  int		   m_nMouseX;	///< Current mouse X position
  int		   m_nMouseY;	///< Current mouse Y position


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
