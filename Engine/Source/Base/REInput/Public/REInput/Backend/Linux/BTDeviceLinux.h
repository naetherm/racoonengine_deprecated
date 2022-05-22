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
#include "REInput/Backend/Bluetooth/BTDevice.h"
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux implementation of 'BTDevice'
*/
class BTDeviceLinux : public BTDevice
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class BTLinux;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Default constructor
  */
  inline BTDeviceLinux() :
    m_nCtrlSocket(0),
    m_nIntSocket(0)
  {
    // Do not destroy device implementation automatically, because this is managed by BTLinux
    m_bDelete = false;
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~BTDeviceLinux() override
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Read handshake response
  *
  *  @return
  *    'true' if the handshake was successful, else 'false'
  */
  bool readHandshake()
  {
    unsigned char nHandshake;
    if (::read(m_nCtrlSocket, &nHandshake, 1) != 1) {
      // Error: Could not read handshake
      return false;
    } else if ((nHandshake & 0xF0) != TransHandshake) {
      // Error: Did not receive handshake
      return false;
    } else if ((nHandshake & 0x0F) != ResultSuccess) {
      // Error: Handshake non-successful
      switch (nHandshake & 0x0F) {
        case ResultNotReady:
        case ResultErrInvalidReportID:
        case ResultErrUnsupportedRequest:
        case ResultErrInvalidParameter:
        case ResultErrUnknown:
        case ResultErrFatal:
          break;
      }
      return false;
    }

    // Handshake successful
    return true;
  }


  //[-------------------------------------------------------]
  //[ Public virtual ConnectionDevice functions             ]
  //[-------------------------------------------------------]
public:
  virtual bool open(RECore::uint16 nOutputPort = 0, RECore::uint16 nInputPort = 0) override
  {
    // Create socket address for control socket
    struct sockaddr_l2 sCtrlAddr;
    memset(&sCtrlAddr, 0, sizeof(sCtrlAddr));
    sCtrlAddr.l2_family		 = AF_BLUETOOTH;
    sCtrlAddr.l2_bdaddr.b[0] = getAddress(0);
    sCtrlAddr.l2_bdaddr.b[1] = getAddress(1);
    sCtrlAddr.l2_bdaddr.b[2] = getAddress(2);
    sCtrlAddr.l2_bdaddr.b[3] = getAddress(3);
    sCtrlAddr.l2_bdaddr.b[4] = getAddress(4);
    sCtrlAddr.l2_bdaddr.b[5] = getAddress(5);
    sCtrlAddr.l2_psm		 = htobs(nOutputPort);

    // Create socket address for interrupt socket
    struct sockaddr_l2 sIntAddr;
    memset(&sIntAddr, 0, sizeof(sIntAddr));
    sIntAddr.l2_family		= AF_BLUETOOTH;
    sIntAddr.l2_bdaddr.b[0] = getAddress(0);
    sIntAddr.l2_bdaddr.b[1] = getAddress(1);
    sIntAddr.l2_bdaddr.b[2] = getAddress(2);
    sIntAddr.l2_bdaddr.b[3] = getAddress(3);
    sIntAddr.l2_bdaddr.b[4] = getAddress(4);
    sIntAddr.l2_bdaddr.b[5] = getAddress(5);
    sIntAddr.l2_psm			= htobs(nInputPort);

    // Create output socket
    m_nCtrlSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if (connect(m_nCtrlSocket, reinterpret_cast<struct sockaddr*>(&sCtrlAddr), sizeof(sCtrlAddr)) == 0) {
      // Create input socket
      m_nIntSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
      if (connect(m_nIntSocket, reinterpret_cast<struct sockaddr*>(&sIntAddr), sizeof(sIntAddr)) == 0) {
        // Connection successful
        int nFlags = fcntl(m_nIntSocket, F_GETFL, 0);
        fcntl(m_nIntSocket, F_SETFL, nFlags | O_NONBLOCK);

        // Start read-thread
        initThread();

        // Device connected
        OnConnectSignal();

        return true;
      }
    }

    // Error!
    return false;
  }

  virtual bool close() override
  {
    // Stop read-thread
    stopThread();

    // Device disconnected
    OnDisconnectSignal();

    // Close sockets
    ::close(m_nCtrlSocket);
    ::close(m_nIntSocket);
    return true;
  }

  inline virtual bool isOpen() const override
  {
    return (m_nCtrlSocket != 0 && m_nIntSocket != 0);
  }

  virtual bool read(RECore::uint8 *pBuffer, RECore::uint32 nSize) override
  {
    // Read data
    lockCriticalSection();
    RECore::uint8 nTemp[BufferSize];
    int nRes = ::read(m_nIntSocket, nTemp, nSize+1);
    if (nRes > 0) {
      if (nTemp[0] == (TransData | ParamInput)) {
        memcpy(pBuffer, &nTemp[1], nRes-1);
        unlockCriticalSection();
        OnReadSignal();
        return true;
      }
    }

    // Error!
    unlockCriticalSection();
    return false;
  }

  virtual bool write(const RECore::uint8 *pBuffer, RECore::uint32 nSize) override
  {
    // Write data
    lockCriticalSection();
    RECore::uint8 nTemp[BufferSize];
    nTemp[0] = TransSetReport | ParamOutput;
    memcpy(nTemp+1, pBuffer, nSize);
    int nRes = ::write(m_nCtrlSocket, nTemp, nSize+1);
    readHandshake();
    unlockCriticalSection();
    if (nRes > 0)
      return (static_cast<RECore::uint32>(nRes) - 1 == nSize);
    else
      return false;
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  int m_nCtrlSocket;	///< Control channel socket
  int m_nIntSocket;	///< Interrupt channel socket


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
