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
#include "REInput/Backend/Linux/HIDDeviceLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux implementation of 'HIDImpl'
*
*  @remarks
*    This implementation uses the event subsystem (input.h) to access generic
*    HID devices. Therefore, the HID interface is not fully supported, so no
*    HID-interna like UsePage-IDs or similar will be present when using this backend.
*/
class HIDLinux : public HIDImpl
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class HID;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Default constructor
  */
  inline HIDLinux()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  virtual ~HIDLinux() override
  {
    // Clear devices
    for (RECore::uint32 i=0; i<m_lstDevices.size(); i++)
      delete m_lstDevices[i];
    m_lstDevices.clear();
  }


  //[-------------------------------------------------------]
  //[ Private virtual HIDImpl functions                     ]
  //[-------------------------------------------------------]
private:
  virtual void enumerateDevices(std::vector<HIDDevice*> &lstDevices) override
  {
    // Clear devices
    for (RECore::uint32 i=0; i<m_lstDevices.size(); i++)
      delete m_lstDevices[i];
    m_lstDevices.clear();
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  std::vector<HIDDeviceLinux*> m_lstDevices;	///< List of devices


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
