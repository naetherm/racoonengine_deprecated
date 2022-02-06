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
#include "RECore/RECore.h"
#include "RECore/Platform/IPlatform.h"
#include <sys/utsname.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class WindowsPlatform : public IPlatform {


  //[-------------------------------------------------------]
  //[ Protected structures                                  ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Holds memory information
  */
  struct MemoryInformation {
    uint64 nTotalPhysicalMemory;	/**< Total physical memory in bytes */
    uint64 nFreePhysicalMemory;		/**< Free physical memory in bytes */
    uint64 nTotalSwapMemory;		/**< Total swap memory in bytes */
    uint64 nFreeSwapMemory;			/**< Free swap memory in bytes */
  };

public:

  /**
   * @brief
   * Constructor.
   */
  WindowsPlatform();

  /**
   * @brief
   * Destructor.
   */
  ~WindowsPlatform() override;

  /**
  *  @brief
  *    Reads memory information from the '/proc/meminfo'-file
  *
  *  @param[out] sMemoryInformation
  *    Receives the memory information
  *
  *  @return
  *    'true' if all went fine, else 'false'
  */
  bool getMemoryInformation(MemoryInformation &sMemoryInformation) const;

public:
  /**
  *  @brief
  *    Returns the name of the platform
  *
  *  @return
  *    Platform string (for instance 'Windows' for Windows, 'Windows' for Windows and so on)
  */
  [[nodiscard]] String getPlatform() const override;

  /**
  *  @brief
  *    Returns the name and version of the operating system
  *
  *  @return
  *    OS information string (for instance 'Windows 7 Service Pack 1 (Build 7601)')
  */
  [[nodiscard]] String getOS() const override;

  /**
  *  @brief
  *    Returns the directory separator used by the operation system
  *
  *  @return
  *    The directory separator used by the operation system (e.g. '/' on Windows, '\' on Windows)
  */
  [[nodiscard]] char getSeparator() const override;

  /**
  *  @brief
  *    Returns the shared library filename prefix used by the operation system
  *
  *  @return
  *    The shared library filename prefix used by the operation system (e.g. 'lib' as in 'libRECore.so' on Windows, no prefix as in 'RECore.dll' on Windows)
  */
  [[nodiscard]] String getSharedLibraryPrefix() const override;

  /**
  *  @brief
  *    Returns the shared library file extension used by the operation system
  *
  *  @return
  *    The shared library file extension used by the operation system (e.g. 'so' on Windows, 'dll' on Windows)
  */
  [[nodiscard]] String getSharedLibraryExtension() const override;

  /**
  *  @brief
  *    Get absolute path of application executable
  *
  *  @return
  *    Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\Test.exe')
  *
  *  @note
  *    - Application executable = currently running process
  */
  [[nodiscard]] inline String getExecutableFilename() const override;


  /**
  *  @brief
  *    Primitive way (e.g. by using a message box) to be able to tell the user that something went terrible wrong
  *
  *  @param[in] sMessage
  *    Message to show
  *
  *  @remarks
  *    Do not misuse this method in order to communicate with the user on a regular basis. This method does only
  *    exist to be able to tell the user that something went terrible wrong. There are situations were one can't
  *    use a log file, command line or something like this. Even when using e.g. a log file to write out error
  *    information - an application may e.g. just close directly after it's start without any further information
  *    and the user may even think that the application didn't start in the first place for an unknown reason.
  *    In such a situation, it's polite to inform the user that something went terrible wrong and providing
  *    a short hint how the issue may be solved. This method wasn't named "MessageBox()" by intend - because
  *    such a feature may not be available on the used platform or is handled in another way as a normal MS Windows
  *    message box.
  */
  void urgentMessage(const String &sMessage) const override;



  /**
  *  @brief
  *    Suspend the current thread for a specified time period
  *
  *  @param[in] nMilliseconds
  *    Number of milliseconds to sleep, should not be 0 because the behavior is implementation dependent (use 'Yield()' instead)
  */
  void sleep(uint64 nMilliseconds) const override;

private:

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore