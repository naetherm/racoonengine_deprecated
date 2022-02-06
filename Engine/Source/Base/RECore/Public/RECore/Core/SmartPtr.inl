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
#include "RECore/RECore.h"
#include "RECore/Core/RefCountPtr.h"


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
template <class AType>
SmartPtr<AType>::SmartPtr() :
	m_pPtr(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
template <class AType>
SmartPtr<AType>::SmartPtr(AType *pPtr) :
	m_pPtr(nullptr)
{
	SetPtr(pPtr);
}

/**
*  @brief
*    Constructor
*/
template <class AType>
SmartPtr<AType>::SmartPtr(const SmartPtr<AType> &pPtr) :
	m_pPtr(nullptr)
{
	SetPtr(pPtr.GetPtr());
}

/**
*  @brief
*    Destructor
*/
template <class AType>
SmartPtr<AType>::~SmartPtr()
{
	SetPtr(static_cast<RefCount<AType>*>(nullptr));
}

/**
*  @brief
*    Assign a pointer
*/
template <class AType>
SmartPtr<AType> &SmartPtr<AType>::operator =(AType *pPtr)
{
	if (GetPointer() != pPtr)
		SetPtr(pPtr);
	return *this;
}

/**
*  @brief
*    Assign a smart pointer
*/
template <class AType>
SmartPtr<AType> &SmartPtr<AType>::operator =(const SmartPtr<AType> &cPtr)
{
	if (GetPointer() != cPtr.GetPointer())
		SetPtr(cPtr.GetPtr());
	return *this;
}

/**
*  @brief
*    Get a direct pointer to the object
*/
template <class AType>
AType *SmartPtr<AType>::GetPointer() const
{
	return m_pPtr ? m_pPtr->GetPointer() : nullptr;
}

/**
*  @brief
*    Get a pointer to access the object
*/
template <class AType>
AType *SmartPtr<AType>::operator ->() const
{
	return GetPointer();
}

/**
*  @brief
*    Cast to a pointer to the object
*/
template <class AType>
SmartPtr<AType>::operator AType*() const
{
	return GetPointer();
}

/**
*  @brief
*    Check if the pointer is not a null pointer
*/
template <class AType>
bool SmartPtr<AType>::operator !() const
{
	return (GetPointer() == nullptr);
}

/**
*  @brief
*    Check for equality
*/
template <class AType>
bool SmartPtr<AType>::operator ==(AType *pPtr) const
{
	return (GetPointer() == pPtr);
}

/**
*  @brief
*    Check for equality
*/
template <class AType>
bool SmartPtr<AType>::operator ==(const SmartPtr<AType> &cPtr) const
{
	return (GetPointer() == cPtr.GetPointer());
}

/**
*  @brief
*    Check for equality
*/
template <class AType>
bool SmartPtr<AType>::operator !=(AType *pPtr) const
{
	return (GetPointer() != pPtr);
}

/**
*  @brief
*    Check for equality
*/
template <class AType>
bool SmartPtr<AType>::operator !=(const SmartPtr<AType> &cPtr) const
{
	return (GetPointer() != cPtr.GetPointer());
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Assign a pointer to an object that does not implement RefCount
*/
template <class AType>
void SmartPtr<AType>::SetPtr(void *pPtr)
{
	// Create a reference counter for the object, then assign it
	SetPtr(pPtr ? new RefCountPtr<AType>(static_cast<AType*>(pPtr)) : static_cast<RefCount<AType>*>(nullptr));
}

/**
*  @brief
*    Assign a pointer to an object that implements RefCount
*/
template <class AType>
void SmartPtr<AType>::SetPtr(RefCount<AType> *pPtr)
{
	// Release old pointer
	if (m_pPtr)
		m_pPtr->Release();

	// Assign new pointer
	if (pPtr)
		pPtr->AddReference();
	m_pPtr = pPtr;
}

/**
*  @brief
*    Get pointer to the reference counted object
*/
template <class AType>
RefCount<AType> *SmartPtr<AType>::GetPtr() const
{
	// Return pointer
	return m_pPtr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
