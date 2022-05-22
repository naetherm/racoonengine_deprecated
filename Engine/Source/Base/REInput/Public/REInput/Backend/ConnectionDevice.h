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
#include "REInput/Backend/DeviceImpl.h"
#include <RECore/Reflect/Event/Event.h>
#include <thread>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Device connection class
*
*  @remarks
*    A connection device is a device backend that uses e.g. a HID or Bluetooth connection to communicate
*    directly with the input device (no use of HID features, only read/write commands) and expose an
*    interface that can be used by the device class to use that connection.
*/
class ConnectionDevice : public DeviceImpl
{


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Device type
  */
  enum EDeviceType {
    DeviceTypeUnknown,	///< Unknown device type
    DeviceTypeHID,		///< HID device
    DeviceTypeBluetooth	///< Bluetooth device
  };


  //[-------------------------------------------------------]
  //[ Public signals                                        ]
  //[-------------------------------------------------------]
public:
  RECore::Event<> OnConnectSignal;		///< Emitted when device has been connected
  RECore::Event<> OnDisconnectSignal;	///< Emitted when device has been disconnected
  RECore::Event<> OnReadSignal;			///< Emitted when data has been read


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  inline ConnectionDevice() :
    m_nDeviceType(DeviceTypeUnknown),
    m_pInputBuffer(nullptr),
    m_pOutputBuffer(nullptr),
    m_nInputReportSize(0),
    m_nOutputReportSize(0),
    m_pThread(nullptr),
    mMutex(nullptr),
    m_bThreadExit(false)
  {
    // Set device backend type
    mDeviceBackendType = DeviceBackendType::CONNECTION_DEVICE;
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~ConnectionDevice() override
  {
    // Stop thread
    stopThread();
  }

  /**
  *  @brief
  *    Get device type
  *
  *  @return
  *    Device type
  */
  [[nodiscard]] inline EDeviceType getDeviceType() const
  {
    return m_nDeviceType;
  }

  /**
  *  @brief
  *    Get input report size
  *
  *  @return
  *    Size of an input report in bytes (unique to each HID device)
  */
  [[nodiscard]] inline RECore::uint32 getInputReportSize() const
  {
    return m_nInputReportSize;
  }

  /**
  *  @brief
  *    Set input report size
  *
  *  @param[in] nSize
  *    Size of an input report in bytes (unique to each HID device)
  */
  inline void setInputReportSize(RECore::uint32 nSize)
  {
    m_nInputReportSize = static_cast<RECore::uint16>(nSize);
  }

  /**
  *  @brief
  *    Get output report size
  *
  *  @return
  *    Size of an output report in bytes
  */
  [[nodiscard]] inline RECore::uint32 getOutputReportSize() const
  {
    return m_nOutputReportSize;
  }

  /**
  *  @brief
  *    Set output report size
  *
  *  @param[in] nSize
  *    Size of an output report in bytes (unique to each HID device)
  */
  inline void setOutputReportSize(RECore::uint32 nSize)
  {
    m_nOutputReportSize = static_cast<RECore::uint16>(nSize);
  }

  /**
  *  @brief
  *    Get input buffer
  *
  *  @return
  *    Input buffer (can be a null pointer if the device is not open), do not destroy the returned buffer!
  */
  [[nodiscard]] inline RECore::uint8 *getInputBuffer() const
  {
    return m_pInputBuffer;
  }

  /**
  *  @brief
  *    Get output buffer
  *
  *  @return
  *    Output buffer (can be a null pointer if the device is not open), do not destroy the returned buffer!
  */
  [[nodiscard]] inline RECore::uint8 *getOutputBuffer() const
  {
    return m_pOutputBuffer;
  }


  //[-------------------------------------------------------]
  //[ Public virtual ConnectionDevice functions             ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Open device connection
  *
  *  @param[in] nOutputPort
  *    Bluetooth port for output channel
  *  @param[in] nInputPort
  *    Bluetooth port for input channel
  *
  *  @return
  *    'true' if all went fine, else 'false'
  *
  *  @note
  *    - The default implementation is empty
  *    - If you are using a "HIDDevice", the output and input ports will be ignored
  */
  inline virtual bool open([[maybe_unused]] RECore::uint16 nOutputPort = 0, [[maybe_unused]] RECore::uint16 nInputPort = 0)
  {
    // To be implemented in derived classes
    // Please use initThread() to start the read thread after successful connection
    // And don't forget to emit "OnConnectSignal" please

    // Not implemented here...
    return false;
  }

  /**
  *  @brief
  *    Close device connection
  *
  *  @return
  *    'true' if all went fine, else 'false'
  *
  *  @note
  *    - The default implementation is empty
  */
  inline virtual bool close()
  {
    // To be implemented in derived classes
    // Please use stopThread() to stop the read thread after closing the connection
    // And don't forget to emit "OnDisconnectSignal" please

    // Not implemented here...
    return false;
  }

  /**
  *  @brief
  *    Check if the device is open
  *
  *  @return
  *    'true' if device is open, else 'false'
  *
  *  @note
  *    - The default implementation is empty
  */
  [[nodiscard]] inline virtual bool isOpen() const
  {
    // To be implemented in derived classes

    // Not implemented here...
    return false;
  }

  /**
  *  @brief
  *    Read from device
  *
  *  @param[out] pBuffer
  *    Buffer that will receive the data, must be valid and at least "nSize"-bytes long!
  *  @param[in]  nSize
  *    Buffer size in bytes
  *
  *  @return
  *    'true' if all went fine, else 'false'
  *
  *  @note
  *    - The default implementation is empty
  */
  inline virtual bool read(RECore::uint8*, RECore::uint32)
  {
    // To be implemented in derived classes
    // Please call lockCriticalSection() before and unlockCriticalSection() after the read operation
    // And don't forget to emit "OnReadSignal" please

    // Not implemented here...
    return false;
  }

  /**
  *  @brief
  *    Write to device
  *
  *  @param[in] pBuffer
  *    Buffer containing the data, must be valid and at least "nSize"-bytes long!
  *  @param[in] nSize
  *    Buffer size in bytes
  *
  *  @return
  *    'true' if all went fine, else 'false'
  *
  *  @note
  *    - The default implementation is empty
  */
  inline virtual bool write(const RECore::uint8*, RECore::uint32)
  {
    // To be implemented in derived classes
    // Please call lockCriticalSection() before and unlockCriticalSection() after the write operation

    // Not implemented here...
    return false;
  }


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Initialize and start thread for read/write operations
  *
  *  @remarks
  *    Creates and starts the read thread and initializes "mMutex", which should be
  *    used by derived classes in their read() and write() functions.
  */
  void initThread();

  /**
  *  @brief
  *    Stop thread for read/write operations
  */
  void stopThread();

  /**
  *  @brief
  *    Lock read/write critical section
  *
  *  @remarks
  *    This should be used by derived classes inside their read() and write() functions!
  */
  void lockCriticalSection();

  /**
  *  @brief
  *    Unlock read/write critical section
  *
  *  @remarks
  *    This should be used by derived classes inside their read() and write() functions!
  */
  void unlockCriticalSection();


  //[-------------------------------------------------------]
  //[ Private static functions                              ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Device update thread function
  *
  *  @param[in] pData
  *    Pointer to this ConnectionDevice instance, always valid!
  */
  [[nodiscard]] static int readThread(void *pData);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  // Device type
  EDeviceType m_nDeviceType;	///< Device type

  // Input and output buffers
  RECore::uint8	 *m_pInputBuffer;		///< Input report buffer, can be a null pointer
  RECore::uint8	 *m_pOutputBuffer;		///< Output report buffer, can be a null pointer
  RECore::uint16  m_nInputReportSize;	///< Size of input report in bytes
  RECore::uint16  m_nOutputReportSize;	///< Size of output report in bytes

  // Read thread
  std::thread	*m_pThread;		///< Update thread, can be a null pointer
  std::mutex	*mMutex;		///< Update mutex, can be a null pointer
  bool		 m_bThreadExit;	///< Flag to exit the thread


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
