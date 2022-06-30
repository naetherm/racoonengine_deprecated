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
#include "RECore/Linux/LinuxDynLib.h"
#include "RECore/Log/Log.h"
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
LinuxDynLib::LinuxDynLib()
: m_pLib(nullptr) {

}

LinuxDynLib::~LinuxDynLib() {
  this->unload();
}

bool LinuxDynLib::isLoaded() const {
  return (this->m_pLib != nullptr);
}

bool LinuxDynLib::load(const String &cUrl) {
  if (this->m_pLib) {
    //return false;
    this->unload();
  }

  this->m_pLib = dlopen(cUrl.cstr(), RTLD_NOW | RTLD_GLOBAL);

  if (m_pLib) {
    // Done
    return true;
  } else {
    // Error: Failed to load dll!
    RE_LOG(Error, String("Unable to load library due to ") + dlerror())

    return false;
  }
}

String LinuxDynLib::getAbsPath() const {
#if !defined(ANDROID) && !defined(APPLE)
  // "dlinfo" is not available on Android and Mac OS X
  link_map *pLinkMap = nullptr;
  if (m_pLib && !dlinfo(m_pLib, RTLD_DI_LINKMAP, &pLinkMap)) {
    // Success
    return String(pLinkMap->l_name);
  }
#endif

  // Error!
  return "";
}

bool LinuxDynLib::unload() {
  if (m_pLib) {
    // Unload library
    if (!dlclose(m_pLib)) {
      m_pLib = nullptr;

      // Done
      return true;
    }
  }

  // Error, could not unload the library
  return false;
}

void *LinuxDynLib::getSymbol(const String &sSymbol) const {
  return m_pLib ? dlsym(m_pLib, sSymbol.cstr()) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
