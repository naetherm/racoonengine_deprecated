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
#include "REInput/Input/Control/Axis.h"
#include "REInput/Input/Control/Button.h"
#include <linux/input.h>
#include <unistd.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux device implementation using the event subsystem of input.h
*/
class LinuxEventDevice : public UpdateDevice
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] nFile
  *    File handle
  */
  explicit LinuxEventDevice(int nFile):
    m_nFile(nFile),
    m_nBusType(0),
    m_nVendor(0),
    m_nProduct(0),
    m_nVersion(0)
  {
    // Do not destroy device implementation automatically, because this is managed by HIDLinux
    m_bDelete = false;

    // Get device info
    input_id sDeviceInfo;
    if (!ioctl(m_nFile, EVIOCGID, &sDeviceInfo)) {
      // Save device info
      m_nBusType = sDeviceInfo.bustype;
      m_nVendor  = sDeviceInfo.vendor;
      m_nProduct = sDeviceInfo.product;
      m_nVersion = sDeviceInfo.version;
    }
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~LinuxEventDevice() override
  {
    // Close file
    close(m_nFile);
  }

  /**
  *  @brief
  *    Get bus type
  *
  *  @return
  *    Bus type
  */
  [[nodiscard]] inline RECore::uint16 getBusType() const
  {
    return m_nBusType;
  }

  /**
  *  @brief
  *    Get vendor ID
  *
  *  @return
  *    Vendor ID
  */
  [[nodiscard]] inline RECore::uint16 getVendorID() const
  {
    return m_nVendor;
  }

  /**
  *  @brief
  *    Get product ID
  *
  *  @return
  *    Product ID
  */
  [[nodiscard]] inline RECore::uint16 getProductID() const
  {
    return m_nProduct;
  }

  /**
  *  @brief
  *    Get version
  *
  *  @return
  *    Version
  */
  [[nodiscard]] inline RECore::uint16 getVersion() const
  {
    return m_nVersion;
  }


  //[-------------------------------------------------------]
  //[ Public virtual UpdateDevice functions                 ]
  //[-------------------------------------------------------]
public:
  virtual void update() override
  {
    // Read input events
    struct input_event pEvents[64];
    ssize_t nSize = read(m_nFile, pEvents, sizeof(struct input_event)*64);
    int nEvents = (nSize > 0 ? static_cast<int>(nSize / sizeof(struct input_event)) : 0);
    for (int i=0; i<nEvents; i++) {
      // Get corresponding control
      Control *pControl = nullptr;
      if (pEvents[i].code == ABS_X)
        pControl = m_pDevice->getControl("X");
      else if (pEvents[i].code == ABS_Y)
        pControl = m_pDevice->getControl("Y");
      else if (pEvents[i].code == ABS_Z)
        pControl = m_pDevice->getControl("Z");
      else if (pEvents[i].code == ABS_RX)
        pControl = m_pDevice->getControl("RX");
      else if (pEvents[i].code == ABS_RY)
        pControl = m_pDevice->getControl("RY");
      else if (pEvents[i].code == ABS_RZ)
        pControl = m_pDevice->getControl("RZ");
      else if (pEvents[i].code == ABS_HAT0X)
        pControl = m_pDevice->getControl("Hat");
      else if (pEvents[i].code >= BTN_JOYSTICK && pEvents[i].code < BTN_GAMEPAD)
        pControl = m_pDevice->getControl(std::string("Button") + std::to_string(pEvents[i].code - BTN_JOYSTICK));
      else if (pEvents[i].code >= BTN_GAMEPAD && pEvents[i].code < BTN_DIGI)
        pControl = m_pDevice->getControl(std::string("Button") + std::to_string(pEvents[i].code - BTN_GAMEPAD));

      // Get control type
      Axis   *pAxis   = (pControl != nullptr && pControl->getControlType() == ControlType::AXIS)   ? static_cast<Axis*>  (pControl) : nullptr;
      Button *pButton = (pControl != nullptr && pControl->getControlType() == ControlType::BUTTON) ? static_cast<Button*>(pControl) : nullptr;

      // Set control value
      if (pEvents[i].type == EV_KEY && pButton)
      {
        // Button
        pButton->setPressed(pEvents[i].value != 0);
      }
      else if ((pEvents[i].type == EV_ABS || pEvents[i].type == EV_REL) && pAxis)
      {
        // Absolute or relative axis
        // Get minimum and maximum values
        input_absinfo sAbsInfo;
        if (!ioctl(m_nFile, EVIOCGABS(0), &sAbsInfo))
        {
          // Get value in a range of -1.0 - +1.0
          float fValue = (static_cast<float>(pEvents[i].value - sAbsInfo.minimum) / static_cast<float>(sAbsInfo.maximum - sAbsInfo.minimum)) * 2.0f - 1.0f;
          if (fValue >  1.0f)
            fValue =  1.0f;
          if (fValue < -1.0f)
            fValue = -1.0f;
          pAxis->setValue(fValue, false);
        }
      }
    }
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  int		 m_nFile;		///< File handle
  RECore::uint16 m_nBusType;	///< Bus type (e.g. USB)
  RECore::uint16 m_nVendor;		///< Vendor ID
  RECore::uint16 m_nProduct;	///< Product ID
  RECore::uint16 m_nVersion;	///< Version


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
