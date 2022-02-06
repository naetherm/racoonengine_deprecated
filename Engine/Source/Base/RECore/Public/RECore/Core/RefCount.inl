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
RefCount<AType>::RefCount() :
	m_nRefCount(0)
{
}

/**
*  @brief
*    Destructor
*/
template <class AType>
RefCount<AType>::~RefCount()
{
}

/**
*  @brief
*    Get a pointer to the object
*/
template <class AType>
const AType *RefCount<AType>::GetPointer() const
{
	return reinterpret_cast<const AType*>(this);
}

template <class AType>
AType *RefCount<AType>::GetPointer()
{
	return reinterpret_cast<AType*>(this);
}

/**
*  @brief
*    Increases the reference count
*/
template <class AType>
uint32 RefCount<AType>::AddReference()
{
	// Increment reference count
	m_nRefCount++;

	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count
*/
template <class AType>
uint32 RefCount<AType>::Release()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		selfDestruct();

		// This object is no longer
		return 0;
	}
}

/**
*  @brief
*    Gets the current reference count
*/
template <class AType>
uint32 RefCount<AType>::GetRefCount() const
{
	// Return current reference count
	return m_nRefCount;
}

template <class AType>
void RefCount<AType>::selfDestruct()
{
  // Return current reference count
  delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
