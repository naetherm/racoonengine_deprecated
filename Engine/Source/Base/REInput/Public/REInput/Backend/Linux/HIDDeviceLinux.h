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
#include "REInput/Backend/HID/HIDDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux implementation of 'HIDDevice'
*/
class HIDDeviceLinux : public HIDDevice
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  */
  inline HIDDeviceLinux()
  {
    // Do not destroy device implementation automatically, because this is managed by HIDLinux
    m_bDelete = false;
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~HIDDeviceLinux() override
  {
    // Close a possibly open connection
    close();
  }


  //[-------------------------------------------------------]
  //[ Public virtual ConnectionDevice functions             ]
  //[-------------------------------------------------------]
public:
  virtual bool open(RECore::uint16 nOutputPort = 0, RECore::uint16 nInputPort = 0) override
  {
    // Close first
    if (isOpen())
      close();

    // TODO(naetherm)
    /*
    // Connect

    // Start read thread
    initThread();

    // Call event
    OnConnectSignal.emit();

    // Done
    return true;
    */

    // Error
    return false;
  }

  virtual bool close() override
  {
    if (isOpen()) {
      // TODO(naetherm)
      /*
      // Stop read thread
      stopThread();

      // Call event
      OnDisconnectSignal.emit();

      // Disconnect

      // Done
      return true;
      */
    }

    // Error
    return false;
  }

  [[nodiscard]] virtual bool isOpen() const override
  {
    // TODO(naetherm)
    return false;
  }

  virtual bool read(RECore::uint8 *pBuffer, RECore::uint32 nSize) override
  {
    // TODO(naetherm)

    // Error!
    return false;
  }

  virtual bool write(const RECore::uint8 *pBuffer, RECore::uint32 nSize) override
  {
    // TODO(naetherm)

    // Error!
    return false;
  }


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
