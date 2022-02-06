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
#include "RECore/Platform/IPlatform.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

String Platform::getPlatform() const {
  return this->mpImpl->getPlatform();
}

String Platform::getOS() const {
  return this->mpImpl->getOS();
}

char Platform::getSeparator() const {
  return this->mpImpl->getSeparator();
}

String Platform::getSharedLibraryPrefix() const {
  return this->mpImpl->getSharedLibraryPrefix();
}

String Platform::getSharedLibraryExtension() const {
  return this->mpImpl->getSharedLibraryExtension();
}

String Platform::getExecutableFilename() const {
  return this->mpImpl->getExecutableFilename();
}

void Platform::urgentMessage(const String &sMessage) const {
  this->mpImpl->urgentMessage(sMessage);
}

const Console& Platform::getConsole() const {
  return this->mpImpl->getConsole();
}

void Platform::sleep(uint64 nMilliseconds) const {
  this->mpImpl->sleep(nMilliseconds);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
