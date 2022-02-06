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
#include "RECore/Tools/Version.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Version::Version() :
  m_nMajor(0),
  m_nMinor(0),
  m_nPatch(0),
  m_sRelease("") {
}

/**
*  @brief
*    Constructor
*/
Version::Version(const String &sName, const String &sCodename, uint16 nMajor, uint16 nMinor, uint16 nPatch,
                 const String &sRelease) :
  m_sName(sName),
  m_sCodename(sCodename),
  m_nMajor(nMajor),
  m_nMinor(nMinor),
  m_nPatch(nPatch),
  m_sRelease(sRelease) {
}

/**
*  @brief
*    Returns a string representation of the version
*/
String Version::toString() const {
  String sVersion;

  // "Name "
  if (m_sName.length())
    sVersion = m_sName + ' ';

  // "Major.Minor.Patch"
  sVersion += String() + static_cast<uint32>(m_nMajor) + '.' + static_cast<uint32>(m_nMinor) + '.' +
              static_cast<uint32>(m_nPatch);

  // "-release"
  if (m_sRelease.length())
    sVersion += '-' + m_sRelease;

  // " - Codename"
  if (m_sCodename.length())
    sVersion += " - " + m_sCodename;

  // Done
  return sVersion;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Version::operator==(const Version &cVersion) const {
  return (m_sName == cVersion.m_sName &&
          m_sCodename == cVersion.m_sName &&
          m_nMajor == cVersion.m_nMajor &&
          m_nMinor == cVersion.m_nMinor &&
          m_nPatch == cVersion.m_nPatch &&
          m_sRelease == cVersion.m_sRelease);
}

bool Version::operator!=(const Version &cVersion) const {
  return (m_sName != cVersion.m_sName ||
          m_sCodename != cVersion.m_sName ||
          m_nMajor != cVersion.m_nMajor ||
          m_nMinor != cVersion.m_nMinor ||
          m_nPatch != cVersion.m_nPatch ||
          m_sRelease == cVersion.m_sRelease);
}

bool Version::operator<(const Version &cVersion) const {
  // Major
  if (m_nMajor < cVersion.m_nMajor)
    return true;
  else if (m_nMajor == cVersion.m_nMajor) {
    // Minor
    if (m_nMinor < cVersion.m_nMinor)
      return true;
    else if (m_nMinor == cVersion.m_nMinor) {
      // Patch
      if (m_nPatch < cVersion.m_nPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator>(const Version &cVersion) const {
  // Major
  if (m_nMajor > cVersion.m_nMajor)
    return true;
  else if (m_nMajor == cVersion.m_nMajor) {
    // Minor
    if (m_nMinor > cVersion.m_nMinor)
      return true;
    else if (m_nMinor == cVersion.m_nMinor) {
      // Patch
      if (m_nPatch > cVersion.m_nPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator<=(const Version &cVersion) const {
  // Major
  if (m_nMajor < cVersion.m_nMajor)
    return true;
  else if (m_nMajor == cVersion.m_nMajor) {
    // Minor
    if (m_nMinor < cVersion.m_nMinor)
      return true;
    else if (m_nMinor == cVersion.m_nMinor) {
      // Patch
      if (m_nPatch < cVersion.m_nPatch)
        return true;
    }
  }
  return false;
}

bool Version::operator>=(const Version &cVersion) const {
  // Major
  if (m_nMajor > cVersion.m_nMajor)
    return true;
  else if (m_nMajor == cVersion.m_nMajor) {
    // Minor
    if (m_nMinor > cVersion.m_nMinor)
      return true;
    else if (m_nMinor == cVersion.m_nMinor) {
      // Patch
      if (m_nPatch > cVersion.m_nPatch)
        return true;
    }
  }
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
