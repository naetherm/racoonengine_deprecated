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


class Device;
class InputManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input provider
*
*  @remarks
*    An input provider is responsible for detecting and managing a specific set of input devices,
*    e.g. the core provider will look after keyboard and mouse. Other types of input providers can
*    be written to support new input devices, typically implemented as plugins.
*/
class Provider
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~Provider()
  {
    // Clean up
    clear();
  }

  /**
  *  @brief
  *    Detect devices
  *
  *  @param[in] bReset
  *    If 'true', delete all input devices and re-detect them all. Otherwise,
  *    only new and removed input devices will be detected.
  *
  *  @remarks
  *    bReset = true should only be used if really necessary, because existing
  *    input handlers will most certainly loose their connection to the device.
  */
  void detectDevices(bool bReset = false);

  /**
  *  @brief
  *    Get list of devices
  *
  *  @return
  *    Device list, do not destroy the returned instances!
  */
  [[nodiscard]] inline const std::vector<Device*> &getDevices() const
  {
    return m_lstDevices;
  }


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  Provider() = delete;

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  */
  inline explicit Provider(InputManager& inputManager) :
    mInputManager(inputManager)
  {
    // Nothing here
  }

  Provider& operator= (const Provider& source) = delete;

  /**
  *  @brief
  *    Destroy all devices
  */
  void clear();

  /**
  *  @brief
  *    Check if a device is already known and update it
  *
  *  @param[in] sName
  *    Name of the input device (e.g. "Mouse0")
  *
  *  @return
  *    'true', if a device with this name is known, else 'false'
  *
  *  @remarks
  *    If the device is already present, it's "confirmed"-flag is set to 'true', so
  *    it will not get deleted by detectDevices(). Therefore, an input provider must
  *    call this function every time in it's queryDevices()-function.
  */
  [[nodiscard]] bool checkDevice(const std::string &sName);

  /**
  *  @brief
  *    Add a new input device
  *
  *  @param[in] sName
  *    Name of the input device (e.g. "Mouse0")
  *  @param[in] pDevice
  *    Pointer to the device (derived from Device), shouldn't be a null pointer (but a null pointer is caught internally)
  *
  *  @return
  *    'true' if all went fine, else 'false'
  *
  *  @remarks
  *    The function will fail, if the device is already present. Otherwise,
  *    the new device will be registered in the input system. On success, the input provider
  *    takes control over the device instance and will destroy it if it's no longer required.
  */
  bool addDevice(const std::string &sName, Device *pDevice);


  //[-------------------------------------------------------]
  //[ Protected virtual Provider functions                  ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Query devices
  *
  *  @remarks
  *    The provider is supposed to detect all of it's devices in this function and then call
  *    addDevice() with the specific name for each device. addDevice() will fail if the device
  *    is already there, which can be checked before by calling checkDevice(). The naming scheme
  *    is up to the provider itself, but something like "DeviceType0" .. "DeviceTypeN" is recommended.
  */
  virtual void queryDevices() = 0;


  //[-------------------------------------------------------]
  //[ Protected definitions                                 ]
  //[-------------------------------------------------------]
protected:
  typedef std::vector<Device*> Devices;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  InputManager& mInputManager;	///< Owner input manager
  Devices		  m_lstDevices;		///< List of devices


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
