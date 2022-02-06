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
#include "REInput/Backend/Provider.h"
#include "REInput/Backend/HID/HID.h"
#include "REInput/Backend/HID/HIDTypes.h"
#include "REInput/Backend/HID/HIDDevice.h"
#include "REInput/Input/Devices/Joystick.h"
#include "REInput/Input/Devices/SpaceMouse.h"
#include "REInput/Input/Devices/WiiMote.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input provider for HID devices
*/
class HIDProvider : public Provider
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  HIDProvider() = delete;

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  */
  inline explicit HIDProvider(InputManager& inputManager) :
    Provider(inputManager),
    mHid(new HID())
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~HIDProvider() override
  {
    delete mHid;
  }


  //[-------------------------------------------------------]
  //[ Private virtual Provider functions                    ]
  //[-------------------------------------------------------]
private:
  virtual void queryDevices() override
  {
    // Device counter
    int nSpaceMouse = 0;
    int nWiiMote	= 0;
    // TODO(naetherm) HIDJoystick not finished yet
    int nJoystick	= 0;

    // Get list of HID-devices
    const std::vector<HIDDevice*> &lstDevices = mHid->getDevices();
    for (uint32_t i=0; i<lstDevices.size(); i++) {
      // Get device
      HIDDevice *pDevice = lstDevices[i];

      // Get vendor and product ID
      const uint32_t nVendor	  = pDevice->getVendor();
      const uint32_t nProduct	  = pDevice->getProduct();
      const uint32_t nUsage	  = pDevice->getUsage();
      const uint32_t nUsagePage = pDevice->getUsagePage();

      // Check device type

      // SpaceMouse
      if (nVendor == SpaceMouse::VendorID && nUsagePage == UsagePageGeneric && nUsage == UsageMultiAxisController) {
        std::string sName = std::string("SpaceMouse") + std::to_string(nSpaceMouse);
        nSpaceMouse++;
        if (!checkDevice(sName))
          addDevice(sName, new SpaceMouse(mInputManager, sName, pDevice));

        // WiiMote
      } else if (nVendor == WiiMote::VendorID && nProduct == WiiMote::ProductID) {
        std::string sName = std::string("WiiMote") + std::to_string(nWiiMote);
        nWiiMote++;
        if (!checkDevice(sName))
          addDevice(sName, new WiiMote(mInputManager, sName, pDevice));

        // Joystick or Joypad
      } else if (nUsagePage == UsagePageGeneric && (nUsage == UsageJoystick || nUsage == UsageGamepad)) {
        std::string sName = std::string("Joystick") + std::to_string(nJoystick);
        nJoystick++;
        if (!checkDevice(sName))
          addDevice(sName, new Joystick(mInputManager, sName, pDevice));
      }
    }
  }


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  HIDProvider(const HIDProvider &cSource) = delete;
  HIDProvider &operator =(const HIDProvider &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  HID* mHid;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
