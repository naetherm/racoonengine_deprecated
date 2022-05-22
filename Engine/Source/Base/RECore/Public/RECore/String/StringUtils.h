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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/TypeTraits.h"
#include <cctype>
#include <cstring>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

inline char CharToLower(char c) {
  return (char) tolower(c);
}

inline char16_t CharToLower(char16_t c) {
  if ((unsigned) c <= 0xff)
    return (char16_t) tolower((RECore::uint8) c);
  return c;
}

inline char32_t CharToLower(char32_t c) {
  if ((unsigned) c <= 0xff)
    return (char32_t) tolower((RECore::uint8) c);
  return c;
}

inline wchar_t CharToLower(wchar_t c) {
  if ((unsigned) c <= 0xff)
    return (wchar_t) tolower((RECore::uint8) c);
  return c;
}


inline char CharToUpper(char c) {
  return (char) toupper(c);
}

inline char16_t CharToUpper(char16_t c) {
  if ((unsigned) c <= 0xff)
    return (char16_t) toupper((RECore::uint8) c);
  return c;
}

inline char32_t CharToUpper(char32_t c) {
  if ((unsigned) c <= 0xff)
    return (char32_t) toupper((RECore::uint8) c);
  return c;
}

inline wchar_t CharToUpper(wchar_t c) {
  if ((unsigned) c <= 0xff)
    return (wchar_t) toupper((RECore::uint8) c);
  return c;
}


//
// CharStrlen
//
inline sizeT CharStrlen(const char *p) {
#if defined(_MSC_VER) || !defined(__GNUC__)
  return strlen(p);
#else
  // Default implementation of all other platforms
  const char* pCurrent = p;
  while(*pCurrent != '\0') {
    ++pCurrent;
  }
  return (sizeT)(pCurrent - p);
#endif
}

inline sizeT CharStrlen(const char16_t *p) {
  // Default implementation of all other platforms
  const char16_t *pCurrent = p;
  while (*pCurrent) {
    ++pCurrent;
  }
  return (sizeT) (pCurrent - p);
}

inline sizeT CharStrlen(const char32_t *p) {
  // Default implementation of all other platforms
  const char32_t *pCurrent = p;
  while (*pCurrent) {
    ++pCurrent;
  }
  return (sizeT) (pCurrent - p);
}

inline sizeT CharStrlen(const wchar_t *p) {
  // Default implementation of all other platforms
  const wchar_t *pCurrent = p;
  while (*pCurrent) {
    ++pCurrent;
  }
  return (sizeT) (pCurrent - p);
}


//
// Find
//
inline const char *Find(const char *p, char c, sizeT n) {
  return (const char *) memchr(p, c, n);
}

inline const char16_t *Find(const char16_t *p, char16_t c, sizeT n) {
  for (; n > 0; --n, ++p) {
    if (*p == c) {
      return p;
    }
  }

  return nullptr;
}

inline const char32_t *Find(const char32_t *p, char32_t c, sizeT n) {
  for (; n > 0; --n, ++p) {
    if (*p == c) {
      return p;
    }
  }

  return nullptr;
}

inline const wchar_t *Find(const wchar_t *p, wchar_t c, sizeT n) {
  for (; n > 0; --n, ++p) {
    if (*p == c) {
      return p;
    }
  }

  return nullptr;
}


//
//
//
template<typename T>
inline T *CharStringUninitializedCopy(const T *pSource, const T *pEnd, T *pDst) {
  memcpy(pDst, pSource, (sizeT) (pEnd - pSource) * sizeof(T));

  return pDst + (pEnd - pSource);
}


//
// CharTypeStringfindFirstOf
//
template<typename T>
inline const T *CharTypeStringfindFirstOf(const T *p1Begin, const T *p1End, const T *p2Begin, const T *p2End) {
  for (; p1Begin != p1End; ++p1Begin) {
    for (const T *pTemp = p2Begin; pTemp != p2End; ++pTemp) {
      if (*p1Begin == *pTemp) {
        return p1Begin;
      }
    }
  }
  return p1End;
}

//
// CharTypeStringrfindFirstNotOf
//
template<typename T>
inline const T *CharTypeStringrfindFirstNotOf(const T *p1RBegin, const T *p1REnd, const T *p2Begin, const T *p2End) {
  for (; p1RBegin != p1REnd; --p1RBegin) {
    const T *pTemp;
    for (pTemp = p2Begin; pTemp != p2End; ++pTemp) {
      if (*(p1RBegin - 1) == *pTemp) {
        break;
      }
    }
    if (pTemp == p2End) {
      return p1RBegin;
    }
  }
  return p1REnd;
}

//
// CharTypeStringfindFirstNotOf
//
template<typename T>
inline const T *CharTypeStringfindFirstNotOf(const T *p1Begin, const T *p1End, const T *p2Begin, const T *p2End) {
  for (; p1Begin != p1End; ++p1Begin) {
    const T *pTemp;
    for (pTemp = p2Begin; pTemp != p2End; ++pTemp) {
      if (*p1Begin == *pTemp) {
        break;
      }
    }
    if (pTemp == p2End) {
      return p1Begin;
    }
  }
  return p1End;
}

template<typename T>
const T *CharTypeStringFindEnd(const T *pBegin, const T *pEnd, T c) {
  const T *pTemp = pEnd;
  while (--pTemp >= pBegin) {
    if (*pTemp == c)
      return pTemp;
  }

  return pEnd;
}

template<typename T>
const T *CharTypeStringRSearch(const T *p1Begin, const T *p1End,
                               const T *p2Begin, const T *p2End) {
  // Test for zero length strings, in which case we have a match or a failure,
  // but the return value is the same either way.
  if ((p1Begin == p1End) || (p2Begin == p2End))
    return p1Begin;

  // Test for a pattern of length 1.
  if ((p2Begin + 1) == p2End)
    return CharTypeStringFindEnd(p1Begin, p1End, *p2Begin);

  // Test for search string length being longer than string length.
  if ((p2End - p2Begin) > (p1End - p1Begin))
    return p1End;

  // General case.
  const T *pSearchEnd = (p1End - (p2End - p2Begin) + 1);
  const T *pCurrent1;
  const T *pCurrent2;

  while (pSearchEnd != p1Begin) {
    // Search for the last occurrence of *p2Begin.
    pCurrent1 = CharTypeStringFindEnd(p1Begin, pSearchEnd, *p2Begin);
    if (pCurrent1 == pSearchEnd) // If the first char of p2 wasn't found,
      return p1End;           // then we immediately have failure.

    // In this case, *pTemp == *p2Begin. So compare the rest.
    pCurrent2 = p2Begin;
    while (*pCurrent1++ == *pCurrent2++) {
      if (pCurrent2 == p2End)
        return (pCurrent1 - (p2End - p2Begin));
    }

    // A smarter algorithm might know to subtract more than just one,
    // but in most cases it won't make much difference anyway.
    --pSearchEnd;
  }

  return p1End;
}

template<typename T>
inline const T *CharTypeStringrfindFirstOf(const T *p1RBegin, const T *p1REnd, const T *p2Begin, const T *p2End) {
  for (; p1RBegin != p1REnd; --p1RBegin) {
    for (const T *pTemp = p2Begin; pTemp != p2End; ++pTemp) {
      if (*(p1RBegin - 1) == *pTemp) {
        return p1RBegin;
      }
    }
  }
  return p1REnd;
}

template<typename T>
inline const T *CharTypeStringrFind(const T *pRBegin, const T *pREnd, const T c) {
  while (pRBegin > pREnd) {
    if (*(pRBegin - 1) == c) {
      return pRBegin;
    }
    --pRBegin;
  }
  return pREnd;
}


inline char *CharStringUninitializedFillN(char *pDestination, sizeT n, const char c) {
  if (n) {
    memset(pDestination, (RECore::uint8) c, (sizeT) n);
  }
  return pDestination + n;
}

inline char16_t *CharStringUninitializedFillN(char16_t *pDestination, sizeT n, const char16_t c) {
  char16_t *pDest16 = pDestination;
  const char16_t *const pEnd = pDestination + n;
  while (pDest16 < pEnd) {
    *pDest16++ = c;
  }
  return pDestination + n;
}

inline char32_t *CharStringUninitializedFillN(char32_t *pDestination, sizeT n, const char32_t c) {
  char32_t *pDest32 = pDestination;
  const char32_t *const pEnd = pDestination + n;
  while (pDest32 < pEnd) {
    *pDest32++ = c;
  }
  return pDestination + n;
}

inline wchar_t *CharStringUninitializedFillN(wchar_t *pDestination, sizeT n, const wchar_t c) {
  wchar_t *pDest32 = pDestination;
  const wchar_t *const pEnd = pDestination + n;
  while (pDest32 < pEnd) {
    *pDest32++ = c;
  }
  return pDestination + n;
}


inline char *CharTypeAssignN(char *pDestination, sizeT n, char c) {
  if (n) {
    return (char *) memset(pDestination, c, (sizeT) n);
  }
  return pDestination;
}

inline char16_t *CharTypeAssignN(char16_t *pDestination, sizeT n, char16_t c) {
  char16_t *pDest16 = pDestination;
  const char16_t *const pEnd = pDestination + n;
  while (pDest16 < pEnd) {
    *pDest16++ = c;
  }
  return pDestination;
}

inline char32_t *CharTypeAssignN(char32_t *pDestination, sizeT n, char32_t c) {
  char32_t *pDest32 = pDestination;
  const char32_t *const pEnd = pDestination + n;
  while (pDest32 < pEnd) {
    *pDest32++ = c;
  }
  return pDestination;
}

inline wchar_t *CharTypeAssignN(wchar_t *pDestination, sizeT n, wchar_t c) {
  wchar_t *pDest32 = pDestination;
  const wchar_t *const pEnd = pDestination + n;
  while (pDest32 < pEnd) {
    *pDest32++ = c;
  }
  return pDestination;
}


//
// CharStringCompare
//
template<typename T>
int CharStringcompare(const T *p1, const T *p2, sizeT n) {
  for (; n > 0; ++p1, ++p2, --n) {
    if (*p1 != *p2) {
      return (static_cast<typename RECore::MakeUnsigned<T>::Type>(*p1) < static_cast<typename RECore::MakeUnsigned<T>::Type>(*p2)) ? -1 : 1;
    }
  }
  return false;
}

//
// CharStringCompareI
//
template<typename T>
int CharStringCompareI(const T *p1, const T *p2, sizeT n) {
  for (; n > 0; ++p1, ++p2, --n) {
    const T c1 = CharToLower(*p1);
    const T c2 = CharToLower(*p2);
    if (c1 != c2) {
      return (static_cast<typename RECore::MakeUnsigned<T>::Type>(c1) < static_cast<typename RECore::MakeUnsigned<T>::Type>(c2)) ? -1 : 1;
    }
  }
  return false;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore