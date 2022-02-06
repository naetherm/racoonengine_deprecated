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
#include "REInput/Input/InputManager.h"
#include <algorithm>
#include "REInput/Backend/HID/HIDProvider.h"
#include "REInput/Backend/Bluetooth/BluetoothProvider.h"
#if defined(LINUX)
#include "REInput/Backend/Linux/LinuxProvider.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
InputManager::InputManager() :
  mMutex(new std::mutex())
{
  detectDevices();
}

InputManager::~InputManager()
{
  // Shut down
  clear();

  // Destroy mutex
  delete mMutex;
}

void InputManager::update()
{
  Controls lstUpdatedControls;
  {
    std::lock_guard<std::mutex> mutexLock(*mMutex);

    // Copy list of controls that have changed
    lstUpdatedControls = m_lstUpdatedControls;

    // Clear list
    m_lstUpdatedControls.clear();
  }

  // Inform controllers about changed controls
  for (size_t i=0; i<lstUpdatedControls.size(); i++) {
    // Inform controller
    Control* pControl = lstUpdatedControls[i];
    pControl->getController().informControl(pControl);
  }

  // Update devices
  for (uint32_t i=0; i<m_lstDevices.size(); i++)
    m_lstDevices[i]->update();
}

void InputManager::detectDevices(bool bReset)
{
  std::lock_guard<std::mutex> mutexLock(*mMutex);

  // Delete all existing providers and devices?
  if (bReset)
    clear();

  // Query available input providers
  // TODO(naetherm) Add provider factory
  detectProvider("DeviceInput::HIDProvider", bReset);
  detectProvider("DeviceInput::BluetoothProvider", bReset);
#if defined(_WIN32)
  // "DeviceInput::LegacyJoystickProvider" must be detected after InputHID, otherwise everything will be detected just as joysticks
			detectProvider("DeviceInput::RawInputProvider", bReset);
			detectProvider("DeviceInput::LegacyJoystickProvider", bReset);
#elif defined(LINUX)
  detectProvider("DeviceInput::LinuxProvider", bReset);
#elif defined(APPLE)
  detectProvider("DeviceInput::MacOSXProvider", bReset);
		#elif defined(__ANDROID__)
			detectProvider("DeviceInput::AndroidProvider", bReset);
		#else
			#error "Unsupported platform"
#endif
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
void InputManager::clear()
{
  // Destroy all input providers
  for (uint32_t i=0; i<m_lstProviders.size(); i++)
    delete m_lstProviders[i];
  m_lstProviders.clear();
  m_mapProviders.clear();

  // Destroy all left-over input devices (usually, all devices should have been destroyed by their providers)
  for (uint32_t i=0; i<m_lstDevices.size(); i++)
    delete m_lstDevices[i];
  m_lstDevices.clear();
  m_mapDevices.clear();
}

bool InputManager::addDevice(Device *pDevice)
{
  // Check if the device can be added
  if (pDevice && m_mapDevices.find(pDevice->getName()) == m_mapDevices.cend()) {
    m_lstDevices.push_back(pDevice);
    m_mapDevices.emplace(pDevice->getName(), pDevice);
    return true;
  }

  // Error!
  return false;
}

bool InputManager::removeDevice(Device *pDevice)
{
  // Check device
  if (pDevice)
  {
    Devices::const_iterator iterator = std::find(m_lstDevices.cbegin(), m_lstDevices.cend(), pDevice);
    if (m_lstDevices.cend() != iterator)
    {
      m_lstDevices.erase(iterator);
      m_mapDevices.erase(pDevice->getName());
      return true;
    }
  }

  // Error!
  return false;
}

void InputManager::removeControl(Control *pControl)
{
  // Valid pointer?
  if (pControl) {
    // Remove control from list (if it's within the list at all)
    std::lock_guard<std::mutex> mutexLock(*mMutex);
    Controls::const_iterator iterator = std::find(m_lstUpdatedControls.cbegin(), m_lstUpdatedControls.cend(), pControl);
    if (m_lstUpdatedControls.cend() != iterator)
    {
      m_lstUpdatedControls.erase(iterator);
    }
  }
}

void InputManager::updateControl(Control *pControl)
{
  // Valid pointer?
  if (pControl) {
    // Add control to list, but only if it's not already within the list!
    std::lock_guard<std::mutex> mutexLock(*mMutex);
    if (std::find(m_lstUpdatedControls.cbegin(), m_lstUpdatedControls.cend(), pControl) == m_lstUpdatedControls.cend())
    {
      m_lstUpdatedControls.push_back(pControl);
    }
  }
}




//[-------------------------------------------------------]
//[ DeviceInput::InputManager methods                     ]
//[-------------------------------------------------------]
void InputManager::detectProvider(const std::string &sProvider, bool bReset)
{
  // Check if the provider is already present
  Provider *pProvider = getProvider(sProvider);
  if (!pProvider) {
    // TODO(naetherm) Add provider factory
    if ("DeviceInput::HIDProvider" == sProvider)
    {
      pProvider = new HIDProvider(*this);
    }
    else if ("DeviceInput::BluetoothProvider" == sProvider)
    {
      pProvider = new BluetoothProvider(*this);
    }
#if defined(_WIN32)
      else if ("DeviceInput::RawInputProvider" == sProvider)
				{
					pProvider = new RawInputProvider(*this);
				}
				else if ("DeviceInput::LegacyJoystickProvider" == sProvider)
				{
					pProvider = new LegacyJoystickProvider(*this);
				}
#elif defined(__ANDROID__)
#warning "TODO(naetherm) Implement me"
#elif defined(LINUX)
    else if ("DeviceInput::LinuxProvider" == sProvider)
    {
      pProvider = new LinuxProvider(*this);
    }
#elif defined(APPLE)
    #warning "TODO(naetherm) Implement me"
			#elif
				#error "Unsupported platform"
#endif

    // Add provider
    if (nullptr != pProvider)
    {
      m_lstProviders.push_back(pProvider);
      m_mapProviders.emplace(sProvider, pProvider);
    }
  }

  // Detect devices
  if (pProvider)
    pProvider->detectDevices(bReset);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
