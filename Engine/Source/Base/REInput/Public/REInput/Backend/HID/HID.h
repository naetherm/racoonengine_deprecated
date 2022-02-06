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
#include "REInput/Backend/HID/HIDImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class HIDDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for accessing HID (Human Interface Device) devices
*/
class HID final
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class HIDProvider;


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Detect available HID devices
  */
  void detectDevices()
  {
    // Clear list
    clear();

    // Check HID implementation
    if (m_pHIDImpl) {
      // Enumerate devices
      m_pHIDImpl->enumerateDevices(m_lstDevices);
    }
  }

  /**
  *  @brief
  *    Get list of available HID devices
  *
  *  @return
  *    Device list
  */
  [[nodiscard]] inline const std::vector<HIDDevice*> &getDevices() const
  {
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
  HID();

  explicit HID(const HID& source) = delete;

  /**
  *  @brief
  *    Destructor
  */
  inline ~HID()
  {
    // Clear devices
    clear();

    // Delete HID implementation
    if (m_pHIDImpl)
      delete m_pHIDImpl;
  }

  HID& operator= (const HID& source) = delete;

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
  HIDImpl					*m_pHIDImpl;	///< Platform specific HID implementation
  std::vector<HIDDevice*>  m_lstDevices;	///< List of devices


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
