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
#include "REInput/Backend/ConnectionDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bluetooth device
*/
class BTDevice : public ConnectionDevice
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class BTLinux;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  BTDevice()
  {
    // Set device type
    m_nDeviceType = ConnectionDevice::DeviceTypeBluetooth;

    // Initialize device
    memset(m_nAddress, 0, 6);
    memset(m_nClass,   0, 3);
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    BTDevice to copy from
  */
  explicit BTDevice(const BTDevice &cSource) :
    m_sName(cSource.m_sName)
  {
    // Set device type
    m_nDeviceType = ConnectionDevice::DeviceTypeBluetooth;

    // Copy device
    memcpy(m_nAddress, cSource.m_nAddress, 6);
    memcpy(m_nClass,   cSource.m_nClass,   3);
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~BTDevice() override
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Assignment operator
  *
  *  @param[in] cSource
  *    BTDevice to copy from
  *
  *  @return
  *    Reference to this BTDevice
  */
  BTDevice &operator =(const BTDevice &cSource)
  {
    m_sName = cSource.m_sName;
    memcpy(m_nAddress, cSource.m_nAddress, 6);
    memcpy(m_nClass,   cSource.m_nClass,   3);
    return *this;
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] cSource
  *    BTDevice to compare with
  *
  *  @return
  *    'true', if both are equal, else 'false'
  */
  [[nodiscard]] bool operator ==(const BTDevice &cSource) const
  {
    return (m_sName == cSource.m_sName &&
            memcmp(m_nAddress,	cSource.m_nAddress, 6) == 0 &&
            memcmp(m_nClass,	cSource.m_nClass,   3) == 0);
  }

  /**
  *  @brief
  *    Get device name
  *
  *  @return
  *    Device name
  */
  [[nodiscard]] inline const std::string& getName() const
  {
    return m_sName;
  }

  /**
  *  @brief
  *    Get device address
  *
  *  @param[in] nIndex
  *    Address component (0..7)
  *
  *  @return
  *    Address
  */
  [[nodiscard]] inline uint8_t getAddress(int8_t nIndex) const
  {
    return (nIndex >= 0 && nIndex < 6) ? m_nAddress[nIndex] : 0u;
  }

  /**
  *  @brief
  *    Get device class
  *
  *  @param[in] nIndex
  *    Class component (0..2)
  *
  *  @return
  *    Class
  */
  [[nodiscard]] inline uint8_t getClass(int8_t nIndex) const
  {
    return (nIndex >= 0 && nIndex < 3) ? m_nClass[nIndex] : 0u;
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Device data
  std::string	m_sName;		///< Device name
  uint8_t		m_nAddress[8];	///< Bluetooth address
  uint8_t		m_nClass[3];	///< Bluetooth device class


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
