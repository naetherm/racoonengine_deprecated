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
#include "RECore/Windows/WindowsDynLib.h"
#if !defined(ANDROID) && !defined(APPLE)
#include <link.h>	// For "dlinfo", not available on Android and Mac OS X
#endif
#include <dlfcn.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
WindowsDynLib::WindowsDynLib()
: m_hModule(nullptr){

}

WindowsDynLib::~WindowsDynLib() {
  this->unload();
}

bool WindowsDynLib::isLoaded() const {
  return (m_hModule != nullptr);
}

bool WindowsDynLib::load(const String &cUrl) {
  if (m_hModule) {
    return false;
  }

  m_sPath = cUrl;
  // Load library
  m_hModule = ::LoadLibraryExA(cUrl.cstr(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

  return (m_hModule != nullptr);
}

String WindowsDynLib::getAbsPath() const {
  // TODO(naethern): Not correct, use something like GetModuleFileNameW
  return m_sPath;
}

bool WindowsDynLib::unload() {
  // Error, could not unload the library
  if (m_hModule) {
    if (::FreeLibrary(m_hModule)) {
      m_hModule = nullptr;

      return true;
    }
  }

  // Error, either not loaded at all or error during unloading
  return false;
}

void *WindowsDynLib::getSymbol(const String &sSymbol) const {
  return m_hModule ? ::GetProcAddress(m_hModule, sSymbol.cstr()) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
