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
#include "REInput/Backend/Provider.h"
#include "REInput/Input/Devices/Device.h"
#include "REInput/Input/InputManager.h"
#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
void Provider::detectDevices(bool bReset)
{
  // Delete all devices?
  if (bReset)
    clear();

  // Flag all current devices as 'not confirmed'
  for (RECore::uint32 i=0; i<m_lstDevices.size(); ++i)
    m_lstDevices[i]->m_bConfirmed = false;

  // Detect new devices (the ones that are already there will be ignored by addDevice)
  queryDevices();

  // Delete all devices that are no longer there (confirmed == false)
  for (RECore::uint32 i=0; i<m_lstDevices.size(); ++i) {
    Device *pDevice = m_lstDevices[i];
    if (!pDevice->m_bConfirmed) {
      // Remove device
      mInputManager.removeDevice(pDevice);
      m_lstDevices.erase(std::find(m_lstDevices.cbegin(), m_lstDevices.cend(), pDevice));
      delete pDevice;
      --i;
    }
  }
}

void Provider::clear()
{
  // Delete all input devices
  for (RECore::uint32 i=0; i<m_lstDevices.size(); ++i)
  {
    Device *pDevice = m_lstDevices[i];
    mInputManager.removeDevice(pDevice);
    delete pDevice;
  }
  m_lstDevices.clear();
}

bool Provider::checkDevice(const std::string &sName)
{
  // Check if the device is already present
  Device *pDevice = mInputManager.getDevice(sName);
  if (pDevice) {
    // Update device
    pDevice->m_bConfirmed = true;
    return true;
  }

  // Not found
  return false;
}

bool Provider::addDevice(const std::string &sName, Device *pDevice)
{
  // Check if the device is already present
  Device *pDeviceFound = mInputManager.getDevice(sName);
  if (!pDeviceFound) {
    // Add device to manager
    if (mInputManager.addDevice(pDevice)) {
      // Add device to own list - if we're in here, we now that the pDevice pointer is valid
      m_lstDevices.push_back(pDevice);
      pDevice->m_bConfirmed = true;
      return true;
    }
  }

  // Error!
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
