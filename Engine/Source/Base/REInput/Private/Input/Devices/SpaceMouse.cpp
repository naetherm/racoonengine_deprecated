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
#include "REInput/Input/Devices/SpaceMouse.h"
#include "REInput/Backend/HID/HIDDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
SpaceMouse::SpaceMouse(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl) :
  Device(inputManager, sName, "SpaceMouse type controller", pImpl),
  TransX	(*this,	"TransX",	"X translation axis"),
  TransY	(*this,	"TransY",	"Y translation axis"),
  TransZ	(*this,	"TransZ",	"Z translation axis"),
  RotX	(*this,	"RotX",		"X rotation axis"),
  RotY	(*this,	"RotY",		"Y rotation axis"),
  RotZ	(*this,	"RotZ",		"Z rotation axis"),
  //Button0	(*this,	"Button0",	"Button #0"),
  //Button1	(*this,	"Button1",	"Button #1"),
  //Button2	(*this,	"Button2",	"Button #2"),
  //Button3	(*this,	"Button3",	"Button #3"),
  //Button4	(*this,	"Button4",	"Button #4"),
  //Button5	(*this,	"Button5",	"Button #5"),
  Button6	(*this,	"Button6",	"Button #6"),
  Button7	(*this,	"Button7",	"Button #7"),
  EventHandlerOnDeviceRead(&SpaceMouse::onDeviceRead, this),
  m_pHIDDevice(nullptr)
{
  // Check if we have a HID backend
  if (pImpl && pImpl->getDeviceBackendType() == DeviceBackendType::HID)
  {
    // Save extra pointer to HIDDevice
    m_pHIDDevice = static_cast<HIDDevice*>(pImpl);

    // Connect to HIDDevice events
    m_pHIDDevice->OnReadSignal.connect(EventHandlerOnDeviceRead);

    // Connect to HID device
    m_pHIDDevice->open();
  }
}

SpaceMouse::~SpaceMouse()
{
  // Check if we have a HID backend
  if (m_pHIDDevice) {
    // We use m_pImpl here to check, because if the device backend has been deleted before, m_pImpl has
    // been reset to a null pointer, but not m_pHIDDevice as this is unknown in the base class
    if (m_pImpl) {
      // Disconnect
      m_pHIDDevice->close();
    }
  }
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
void SpaceMouse::onDeviceRead()
{
  // Get input buffer
  uint8_t *pInputBuffer = m_pHIDDevice->getInputBuffer();
  if (pInputBuffer) {
    // Read data
    switch (pInputBuffer[0]) {
      // Translation
      case 0x01:
      {
        float fTransX = static_cast<float>(static_cast<int16_t>((pInputBuffer[1] & 0x000000ff) | (static_cast<int>(pInputBuffer[2])<<8 & 0xffffff00)));
        float fTransY = static_cast<float>(static_cast<int16_t>((pInputBuffer[3] & 0x000000ff) | (static_cast<int>(pInputBuffer[4])<<8 & 0xffffff00)));
        float fTransZ = static_cast<float>(static_cast<int16_t>((pInputBuffer[5] & 0x000000ff) | (static_cast<int>(pInputBuffer[6])<<8 & 0xffffff00)));
        if (TransX.getValue() != fTransX)
          TransX.setValue(fTransX, false);
        if (TransY.getValue() != fTransY)
          TransY.setValue(fTransY, false);
        if (TransZ.getValue() != fTransZ)
          TransZ.setValue(fTransZ, false);
        break;
      }

        // Rotation
      case 0x02:
      {
        float fRotX = static_cast<float>(static_cast<int16_t>((pInputBuffer[1] & 0x000000ff) | (static_cast<int>(pInputBuffer[2])<<8 & 0xffffff00)));
        float fRotY = static_cast<float>(static_cast<int16_t>((pInputBuffer[3] & 0x000000ff) | (static_cast<int>(pInputBuffer[4])<<8 & 0xffffff00)));
        float fRotZ = static_cast<float>(static_cast<int16_t>((pInputBuffer[5] & 0x000000ff) | (static_cast<int>(pInputBuffer[6])<<8 & 0xffffff00)));
        if (RotX.getValue() != fRotX)
          RotX.setValue(fRotX, false);
        if (RotY.getValue() != fRotY)
          RotY.setValue(fRotY, false);
        if (RotZ.getValue() != fRotZ)
          RotZ.setValue(fRotZ, false);
        break;
      }

        // Buttons
      case 0x03:
      {
        bool bPressed = ((pInputBuffer[1] & 0x0001) != 0);
        //if (Button0.isPressed() != bPressed)
        //{
        //	Button0.setPressed(bPressed);
        //}
        bPressed = ((pInputBuffer[1] & 0x0002) != 0);
        /*
        if (Button1.isPressed() != bPressed)
        {
          Button1.setPressed(bPressed);
        }
        bPressed = ((pInputBuffer[1] & 0x0004) != 0);
        if (Button2.isPressed() != bPressed)
        {
          Button2.setPressed(bPressed);
        }
        bPressed = ((pInputBuffer[1] & 0x0008) != 0);
        if (Button3.isPressed() != bPressed)
        {
          Button3.setPressed(bPressed);
        }
        bPressed = ((pInputBuffer[1] & 0x0010) != 0);
        if (Button4.isPressed() != bPressed)
        {
          Button4.setPressed(bPressed);
        }
        bPressed = ((pInputBuffer[1] & 0x0020) != 0);
        if (Button5.isPressed() != bPressed)
        {
          Button5.setPressed(bPressed);
        }
        bPressed = ((pInputBuffer[1] & 0x0040) != 0);
        if (Button6.isPressed() != bPressed)
        {
          Button6.setPressed(bPressed);
        }
         */
        bPressed = ((pInputBuffer[1] & 0x0080) != 0);
        if (Button7.isPressed() != bPressed)
        {
          Button7.setPressed(bPressed);
        }
        break;
      }
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
