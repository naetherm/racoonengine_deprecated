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
#include "RECore/Windows/WindowsPlatform.h"
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
bool WindowsPlatform::getMemoryInformation(MemoryInformation &sMemoryInformation) const
{
  // Initialize memory information
  sMemoryInformation.nTotalPhysicalMemory	= 0;
  sMemoryInformation.nFreePhysicalMemory	= 0;
  sMemoryInformation.nTotalSwapMemory		= 0;
  sMemoryInformation.nFreeSwapMemory		= 0;

  // Error!
  return false;
}

WindowsPlatform::WindowsPlatform() {

}

WindowsPlatform::~WindowsPlatform() {

}

String WindowsPlatform::getPlatform() const {
  static String SPlatform = "Windows";
  return SPlatform;
}

String WindowsPlatform::getOS() const {
  // Get system info
  return "";
}

char WindowsPlatform::getSeparator() const {
  return '/';
}

String WindowsPlatform::getSharedLibraryPrefix() const {
  static String String = "";
  return String;
}

String WindowsPlatform::getSharedLibraryExtension() const {
  static String String = "dll";
  return String;
}

void WindowsPlatform::urgentMessage(const String &sMessage) const {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
