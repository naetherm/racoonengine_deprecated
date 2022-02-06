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
#include <cstring>	// For "memcpy()", "memmove()", "memset()" and "memcmp()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copies memory
*/
inline void Memory::Copy(void *pDestination, const void *pSource, uint32 nNumOfBytes)
{
  memcpy(pDestination, pSource, nNumOfBytes);
}

/**
*  @brief
*    Moves memory
*/
inline void Memory::Move(void *pDestination, const void *pSource, uint32 nNumOfBytes)
{
  memmove(pDestination, pSource, nNumOfBytes);
}

/**
*  @brief
*    Sets memory to a specified character
*/
inline void Memory::Set(void *pDestination, int nCharacter, uint32 nNumOfBytes)
{
  memset(pDestination, nCharacter, nNumOfBytes);
}

/**
*  @brief
*    Compares memory
*/
inline int Memory::compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes)
{
  return memcmp(pFirstBuffer, pSecondBuffer, nNumOfBytes);
}

inline void* Memory::reallocate(void* oldPointer, size_t oldNumberOfBytes, size_t newNumberOfBytes, size_t alignment) {
  return SAllocator.reallocate(oldPointer, oldNumberOfBytes, newNumberOfBytes, alignment);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
