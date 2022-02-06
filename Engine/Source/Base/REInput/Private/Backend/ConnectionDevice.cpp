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
#include "REInput/Backend/ConnectionDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ DeviceInput::ConnectionDevice                         ]
//[-------------------------------------------------------]
void ConnectionDevice::initThread()
{
  // Create input buffer
  if (!m_pInputBuffer && m_nInputReportSize > 0)
    m_pInputBuffer  = new uint8_t[m_nInputReportSize];

  // Create output buffer
  if (!m_pOutputBuffer && m_nOutputReportSize > 0)
    m_pOutputBuffer = new uint8_t[m_nOutputReportSize];

  // Create critical section
  if (nullptr == mMutex)
  {
    mMutex = new std::mutex();
  }

  // Start thread
  if (nullptr == m_pThread)
  {
    m_pThread = new std::thread(ConnectionDevice::readThread, this);
  }
}

void ConnectionDevice::stopThread()
{
  // Stop thread
  if (m_pThread) {
    // Exit thread function and wait until it has ended
    m_bThreadExit = true;
    m_pThread->join();

    // Delete thread
    delete m_pThread;
    m_pThread = nullptr;
  }

  // Delete critical section
  if (nullptr != mMutex) {
    delete mMutex;
    mMutex = nullptr;
  }

  // Destroy input buffer
  if (m_pInputBuffer) {
    delete [] m_pInputBuffer;
    m_pInputBuffer = nullptr;
  }

  // Destroy output buffer
  if (m_pOutputBuffer) {
    delete [] m_pOutputBuffer;
    m_pOutputBuffer = nullptr;
  }
}

void ConnectionDevice::lockCriticalSection()
{
  // Check critical section
  if (nullptr != mMutex) {
    // Lock critical section (no read/write allowed)
    mMutex->lock();
  }
}

void ConnectionDevice::unlockCriticalSection()
{
  // Check critical section
  if (nullptr != mMutex) {
    // Unlock critical section
    mMutex->unlock();
  }
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
int ConnectionDevice::readThread(void *pData)
{
#ifdef _WIN32
  // Set thread name to improve debugging
			__try
			{
				const DWORD MS_VC_EXCEPTION = 0x406D1388;
				#pragma pack(push, 8)
					typedef struct tagTHREADNAME_INFO
					{
						DWORD  dwType;		///< Must be 0x1000
						LPCSTR szName;		///< Pointer to name (in user address space)
						DWORD  dwThreadID;	///< Thread ID (-1 = caller thread)
						DWORD  dwFlags;		///< Reserved for future use, must be zero
					} THREADNAME_INFO;
				#pragma pack(pop)
				const THREADNAME_INFO info = { 0x1000, "Input connection device", ::GetCurrentThreadId(), 0 };
				::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<const ULONG_PTR*>(&info));
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				// Nothing here
			}
#endif

  // Get handler
  ConnectionDevice *pDevice = static_cast<ConnectionDevice*>(pData);
  while (!pDevice->m_bThreadExit) {
    // Read data from device
    pDevice->read(pDevice->m_pInputBuffer, pDevice->m_nInputReportSize);
  }

  // Done
  return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
