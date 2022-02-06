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
#include "RECore/Memory/MimallocAllocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


class Memory {
public:
  /**
  *  @brief
  *    Copies memory
  *
  *  @param[in] pDestination
  *    Destination buffer (MUST be valid!)
  *  @param[in] pSource
  *    Source buffer (MUST be valid!)
  *  @param[in] nNumOfBytes
  *    Number of bytes to copy
  *
  *  @note
  *    - 'memcpy'-wrapper
  *    -  The memory areas should not overlap
  */
  static inline void Copy(void *pDestination, const void *pSource, uint32 nNumOfBytes);

  /**
  *  @brief
  *    Moves memory
  *
  *  @param[in] pDestination
  *    Destination buffer (MUST be valid!)
  *  @param[in] pSource
  *    Source buffer (MUST be valid!)
  *  @param[in] nNumOfBytes
  *    Number of bytes to copy
  *
  *  @note
  *    - 'memmove'-wrapper
  *    -  The memory areas may overlap
  */
  static inline void Move(void *pDestination, const void *pSource, uint32 nNumOfBytes);

  /**
  *  @brief
  *    Sets memory to a specified character
  *
  *  @param[in] pDestination
  *    Destination buffer (MUST be valid!)
  *  @param[in] nCharacter
  *    Character to set
  *  @param[in] nNumOfBytes
  *    Number of bytes to set
  *
  *  @note
  *    - 'memset'-wrapper
  */
  static inline void Set(void *pDestination, int nCharacter, uint32 nNumOfBytes);

  /**
  *  @brief
  *    Compares memory
  *
  *  @param[in] pFirstBuffer
  *    First buffer (MUST be valid!)
  *  @param[in] pSecondBuffer
  *    Second buffer (MUST be valid!)
  *  @param[in] nNumOfBytes
  *    Number of bytes to compare
  *
  *  @return
  *    < 0 = First buffer is less than second buffer
  *      0 = First buffer identical to second buffer
  *    > 0 = First buffer is greater than second buffer
  *
  *  @note
  *    - 'memcmp'-wrapper
  */
  static inline int compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes);

  static inline void* reallocate(void* oldPointer, size_t oldNumberOfBytes, size_t newNumberOfBytes, size_t alignment);

private:

  static MimallocAllocator SAllocator;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Memory/Memory.inl"


// Malloc and free
#define RHI_MALLOC(context, newNumberOfBytes) (context).getAllocator().reallocate(nullptr, 0, newNumberOfBytes, 1)
#define RHI_MALLOC_TYPED(context, type, newNumberOfElements) reinterpret_cast<type*>(RECore::Memory::reallocate(nullptr, 0, sizeof(type) * (newNumberOfElements), 1))
#define RHI_FREE(context, oldPointer) RECore::Memory::reallocate(oldPointer, 0, 0, 1)

// New and delete
// - Using placement new and explicit destructor call
// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
#define RHI_NEW(context, type) new (RECore::Memory::reallocate(nullptr, 0, sizeof(type), 1)) type
#define RHI_DELETE(context, type, oldPointer) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				typedef type destructor; \
				static_cast<type*>(oldPointer)->~destructor(); \
				RECore::Memory::reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)
