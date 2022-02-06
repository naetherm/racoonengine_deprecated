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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract memory allocator interface
*
*  @note
*    - The implementation must be multithreading safe since the RHI is allowed to internally use multiple threads
*    - The interface design is basing on "Nicholas Frechette's Blog Raw bits" - "A memory allocator interface" - http://nfrechette.github.io/2015/05/11/memory_allocator_interface/
*/
class IAllocator
{

  // Public static methods
public:
  template<typename Type>
  [[nodiscard]] static inline Type* constructN(Type* basePointer, size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      new ((void*)(basePointer + i)) Type();
    }
    return basePointer;
  }

  // Public methods
public:
  /**
  *  @brief
  *    Reallocate
  *
  *  @param[in] oldPointer
  *    Old pointer, can be a null pointer
  *  @param[in] oldNumberOfBytes
  *    Old number of bytes, must be zero of the old pointer is a null pointer, else can be zero if this information isn't available
  *  @param[in] newNumberOfBytes
  *    New number of bytes
  *  @param[in] alignment
  *    Alignment
  *
  *  @return
  *    New pointer, can be a null pointer
  */
  inline void* reallocate(void* oldPointer, size_t oldNumberOfBytes, size_t newNumberOfBytes, size_t alignment)
  {
    ASSERT(nullptr != mReallocateFuntion, "Invalid reallocate function")
    ASSERT(nullptr != oldPointer || 0 == oldNumberOfBytes, "Invalid old pointer")
    return (*mReallocateFuntion)(*this, oldPointer, oldNumberOfBytes, newNumberOfBytes, alignment);
  }

  // Protected definitions
protected:
  typedef void* (*ReallocateFuntion)(IAllocator&, void*, size_t, size_t, size_t);

  // Protected methods
protected:
  inline explicit IAllocator(ReallocateFuntion reallocateFuntion) :
    mReallocateFuntion(reallocateFuntion)
  {
    ASSERT(nullptr != mReallocateFuntion, "Invalid reallocate function")
  }

  inline virtual ~IAllocator()
  {}

  explicit IAllocator(const IAllocator&) = delete;
  IAllocator& operator=(const IAllocator&) = delete;

  // Private methods
private:
  ReallocateFuntion mReallocateFuntion;

};

// Macros & definitions

/*
// Malloc and free
#define RHI_MALLOC(context, newNumberOfBytes) (context).getAllocator().reallocate(nullptr, 0, newNumberOfBytes, 1)
#define RHI_MALLOC_TYPED(context, type, newNumberOfElements) reinterpret_cast<type*>((context).getAllocator().reallocate(nullptr, 0, sizeof(type) * (newNumberOfElements), 1))
#define RHI_FREE(context, oldPointer) (context).getAllocator().reallocate(oldPointer, 0, 0, 1)

// New and delete
// - Using placement new and explicit destructor call
// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
#define RHI_NEW(context, type) new ((context).getAllocator().reallocate(nullptr, 0, sizeof(type), 1)) type
#define RHI_DELETE(context, type, oldPointer) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				typedef type destructor; \
				static_cast<type*>(oldPointer)->~destructor(); \
				(context).getAllocator().reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)
*/
// New and delete of arrays
// - Using placement new and explicit destructor call, not using the array version since it's using an undocumented additional amount of memory
// - See http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ - "2.  Wrap your macros in do { � } while(0)." for background information about the do-while wrap
#define RHI_NEW_ARRAY(context, type, count) RECore::IAllocator::constructN(static_cast<type*>(((context).getAllocator().reallocate(nullptr, 0, sizeof(type) * (count), 1))), count)
#define RHI_DELETE_ARRAY(context, type, oldPointer, count) \
		do \
		{ \
			if (nullptr != oldPointer) \
			{ \
				for (size_t allocatorArrayIndex = 0; allocatorArrayIndex < count; ++allocatorArrayIndex) \
				{ \
					typedef type destructor; \
					(static_cast<type*>(oldPointer))[allocatorArrayIndex].~destructor(); \
				} \
				(context).getAllocator().reallocate(oldPointer, 0, 0, 1); \
			} \
		} while (0)



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore