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
#include "REInput/Backend/Bluetooth/Bluetooth.h"
#include "REInput/Backend/Bluetooth/BTDevice.h"
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
*    Input provider for Bluetooth devices
*/
class BluetoothProvider final : public Provider
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  BluetoothProvider() = delete;

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  */
  inline explicit BluetoothProvider(InputManager& inputManager) :
    Provider(inputManager),
    mBluetooth(new Bluetooth())
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~BluetoothProvider() override
  {
    delete mBluetooth;
  }


  //[-------------------------------------------------------]
  //[ Private virtual Provider functions                    ]
  //[-------------------------------------------------------]
private:
  virtual void queryDevices() override
  {
    // Device counter
    int nWiiMote = 0;

    // Get list of Bluetooth-devices
    const std::vector<BTDevice*> &lstDevices = mBluetooth->getDevices();
    for (uint32_t i=0; i<lstDevices.size(); ++i) {
      // Get device
      BTDevice *pDevice = lstDevices[i];

      // Check device type
      if ( pDevice->getName() == "Nintendo RVL-CNT-01" ||
           (pDevice->getClass(0) == WiiMote::DeviceClass0 &&
            pDevice->getClass(1) == WiiMote::DeviceClass1 &&
            pDevice->getClass(2) == WiiMote::DeviceClass2) ) {
        // WiiMote
        std::string sName = std::string("WiiMote") + std::to_string(nWiiMote);
        ++nWiiMote;
        if (!checkDevice(sName))
          addDevice(sName, new WiiMote(mInputManager, sName, pDevice));
      }
    }
  }


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  BluetoothProvider(const BluetoothProvider &cSource) = delete;
  BluetoothProvider &operator =(const BluetoothProvider &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  Bluetooth* mBluetooth;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
