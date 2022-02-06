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
#include "REInput/Input/Devices/Keyboard.h"
#include "REInput/Input/Devices/Mouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class Control;
class Device;
class Provider;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input manager
*
*  @note
*    - The input manager stores all available devices that are present on the computer and controls the update of input messages
*/
class InputManager final
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class Provider;
  friend class Control;


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  typedef std::vector<Device*> Devices;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  InputManager();

  /**
  *  @brief
  *    Destructor
  */
  ~InputManager();

  /**
  *  @brief
  *    Update input manager once per frame
  *
  *  @remarks
  *    This function must be called once per frame to allow devices to update their status
  *    and to process input messages read from these devices. This is also done to make sure
  *    that input messages are processed synchronously in the main thread, rather than sending
  *    messages from other threads asynchronously.
  */
  REINPUT_API void update();

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
  *    input handlers will most certainly lose their connection to the device.
  */
  REINPUT_API void detectDevices(bool bReset = false);

  /**
  *  @brief
  *    Get list of input providers
  *
  *  @return
  *    Provider list, do not destroy the returned instances
  */
  [[nodiscard]] inline const std::vector<Provider*> &getProviders() const
  {
    return m_lstProviders;
  }

  /**
  *  @brief
  *    Get a specific input provider
  *
  *  @param[in] sProvider
  *    Name of provider
  *
  *  @return
  *    Provider, or a null pointer if it doesn't exist, do not destroy the returned instance!
  */
  [[nodiscard]] inline Provider *getProvider(const std::string &sProvider)
  {
    ProviderMap::const_iterator iterator = m_mapProviders.find(sProvider);
    return (m_mapProviders.cend() != iterator) ? iterator->second : nullptr;
  }

  /**
  *  @brief
  *    Get list of devices
  *
  *  @return
  *    Device list, do not destroy the returned instances!
  */
  [[nodiscard]] inline Devices &getDevices()
  {
    return m_lstDevices;
  }

  /**
  *  @brief
  *    Get a specific device
  *
  *  @param[in] sDevice
  *    Name of device
  *
  *  @return
  *    Device, or a null pointer if it doesn't exist, do not destroy the returned instance!
  */
  [[nodiscard]] inline Device *getDevice(const std::string &sDevice) const
  {
    DeviceMap::const_iterator iterator = m_mapDevices.find(sDevice);
    return (m_mapDevices.cend() != iterator) ? iterator->second : nullptr;
  }

  /**
  *  @brief
  *    Get default keyboard device
  *
  *  @return
  *    Default keyboard, can be a null pointer, do not destroy the returned instance!
  */
  [[nodiscard]] inline Keyboard *getKeyboard() const
  {
    return static_cast<Keyboard*>(getDevice("Keyboard"));
  }

  /**
  *  @brief
  *    Get default mouse device
  *
  *  @return
  *    Default mouse, can be a null pointer, do not destroy the returned instance!
  */
  [[nodiscard]] inline Mouse *getMouse() const
  {
    return static_cast<Mouse*>(getDevice("Mouse"));
  }


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Source to copy from
  */
  InputManager(const InputManager &cSource) = delete;

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] cSource
  *    Source to copy from
  *
  *  @return
  *    Reference to this input manager instance
  */
  InputManager& operator= (const InputManager &cSource) = delete;

  /**
  *  @brief
  *    Destroy all input providers and devices
  */
  void clear();

  /**
  *  @brief
  *    Detect devices from a specific provider
  *
  *  @param[in] sProvider
  *    Name of provider
  *  @param[in] bReset
  *    If 'true', delete all input devices and re-detect them all. Otherwise,
  *    only new and removed input devices will be detected.
  *
  *  @remarks
  *    If the provider is already present, it's Detect()-method will be called. Otherwise,
  *    a new instance of the provider will be created, then Detect() will be called as well.
  */
  void detectProvider(const std::string &sProvider, bool bReset);

  /**
  *  @brief
  *    Add a new input device
  *
  *  @param[in] pDevice
  *    Input device, shouldn't be a null pointer (but a null pointer is caught internally)
  *
  *  @return
  *    'true' if all went fine, else 'false'
  */
  bool addDevice(Device *pDevice);

  /**
  *  @brief
  *    Remove device
  *
  *  @param[in] pDevice
  *    Input device, shouldn't be a null pointer (but a null pointer is caught internally)
  *
  *  @return
  *    'true' if all went fine, else 'false'
  */
  bool removeDevice(Device *pDevice);

  /**
  *  @brief
  *    Remove control
  *
  *  @param[in] pControl
  *    Input control to remove, shouldn't be a null pointer (but a null pointer is caught internally)
  */
  void removeControl(Control *pControl);

  /**
  *  @brief
  *    Update control
  *
  *  @param[in] pControl
  *    Input control, shouldn't be a null pointer (but a null pointer is caught internally)
  *
  *  @remarks
  *    This marks the control as being updated recently, which will fire a message
  *    in the next update()-call.
  */
  void updateControl(Control *pControl);


  //[-------------------------------------------------------]
  //[ Private definitions                                   ]
  //[-------------------------------------------------------]
private:
  typedef std::vector<Control*> Controls;
  typedef std::unordered_map<std::string, Provider*> ProviderMap;
  typedef std::unordered_map<std::string, Device*> DeviceMap;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Providers and devices
  std::vector<Provider*>	 m_lstProviders;		///< List of providers
  ProviderMap				 m_mapProviders;		///< Hash map of providers
  Devices					 m_lstDevices;			///< List of devices
  DeviceMap				 m_mapDevices;			///< Hash map of devices
  std::mutex				*mMutex;				///< Mutex for reading/writing input messages, always valid
  Controls				 m_lstUpdatedControls;	///< List of controls that have been updated (message list)


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
