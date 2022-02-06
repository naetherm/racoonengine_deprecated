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
#include "REInput/Backend/Bluetooth/BTImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class BTDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for accessing Bluetooth devices
*/
class Bluetooth final
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class BluetoothProvider;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Detect available Bluetooth devices
  */
  void detectDevices()
  {
    // Clear list
    clear();

    // Check Bluetooth implementation
    if (m_pBTImpl) {
      // Enumerate devices
      m_pBTImpl->enumerateDevices(m_lstDevices);
    }
  }

  /**
  *  @brief
  *    Get list of available Bluetooth devices
  *
  *  @return
  *    Device list
  */
  [[nodiscard]] inline const std::vector<BTDevice*> &getDevices() const
  {
    // Return list
    return m_lstDevices;
  }


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Default constructor
  */
  Bluetooth();

  explicit Bluetooth(const Bluetooth& source) = delete;

  /**
  *  @brief
  *    Destructor
  */
  ~Bluetooth()
  {
    // Clear devices
    clear();

    // Delete Bluetooth implementation
    if (m_pBTImpl)
      delete m_pBTImpl;
  }

  Bluetooth& operator= (const Bluetooth& source) = delete;

  /**
  *  @brief
  *    Remove all devices
  */
  inline void clear()
  {
    // Destroy devices
    m_lstDevices.clear();
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  BTImpl					*m_pBTImpl;		///< Platform specific Bluetooth implementation
  std::vector<BTDevice*>	 m_lstDevices;	///< List of devices


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
