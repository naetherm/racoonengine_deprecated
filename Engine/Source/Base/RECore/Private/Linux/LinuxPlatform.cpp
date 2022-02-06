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
#include "RECore/Linux/LinuxPlatform.h"
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <locale.h>
#include <sys/time.h>
#include <dlfcn.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
bool LinuxPlatform::getMemoryInformation(MemoryInformation &sMemoryInformation) const
{
  // Initialize memory information
  sMemoryInformation.nTotalPhysicalMemory	= 0;
  sMemoryInformation.nFreePhysicalMemory	= 0;
  sMemoryInformation.nTotalSwapMemory		= 0;
  sMemoryInformation.nFreeSwapMemory		= 0;

  // Error!
  return false;
}

LinuxPlatform::LinuxPlatform()
: m_bSysInfoInit(!uname(&m_sName)) {

}

LinuxPlatform::~LinuxPlatform() {

}

String LinuxPlatform::getPlatform() const {
  static String SPlatform = "Linux";
  return SPlatform;
}

String LinuxPlatform::getOS() const {
  // Get system info
  if (m_bSysInfoInit) {
    String sVersion = m_sName.sysname;
    sVersion += ' ';
    sVersion += m_sName.machine;
    sVersion += ' ';
    sVersion += m_sName.release;
    return sVersion;
  } else {
    return getPlatform() + " unknown";
  }
}

char LinuxPlatform::getSeparator() const {
  return '/';
}

String LinuxPlatform::getSharedLibraryPrefix() const {
  static String String = "lib";
  return String;
}

String LinuxPlatform::getSharedLibraryExtension() const {
  static String String = "so";
  return String;
}

String LinuxPlatform::getExecutableFilename() const {
  // Get PID of current process
  const pid_t nPID = getpid();
  char szLinkName[512];
  if (snprintf(szLinkName, 512, "/proc/%d/exe", nPID) >= 0) {
    // Read symbolic link
    char szProgram[512];
    const int nRet = readlink(szLinkName, szProgram, 512);
    if (nRet < 512) {
      szProgram[nRet] = 0;
      return String(szProgram);
    }
  }

  // Error: Could not determine path to application
  return "";
}

void LinuxPlatform::urgentMessage(const String &sMessage) const {
  // Do also feed the console, safe is safe
  fputs(sMessage, stdout);
  fputs("\n", stdout);
  fflush(stdout);

  // There's no such thing as "MessageBox()" from MS Windows and using a GUI system
  // like Qt would be a total overkill in here, so, go the easiest possible way...
  char szCommand[1024];
  sprintf(szCommand, "xmessage -center \"%s\"", sMessage.cstr());
  system(szCommand);
}

const Console &LinuxPlatform::getConsole() const {
  return this->mConsoleLinux;
}

void LinuxPlatform::sleep(uint64 nMilliseconds) const {
  // We have to split up the given number of milliseconds to sleep into seconds and milliseconds

  // Calculate the number of seconds to sleep
  const time_t nSeconds = static_cast<time_t>(nMilliseconds/1000);

  // Overwrite the given number of milliseconds with the remaining calculated number of milliseconds to sleep
  nMilliseconds = nMilliseconds - (nSeconds*1000);

  // Now sleep well my friend...
  struct timespec sTimespec;
  sTimespec.tv_sec  = nSeconds;
  sTimespec.tv_nsec = nMilliseconds*1000000L;
  nanosleep(&sTimespec, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
