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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class AType> class RefCount;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Smart pointer template
*/
template <class AType>
class SmartPtr {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		SmartPtr();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pPtr
		*    Direct pointer to initialize with, can be a null pointer
		*/
		SmartPtr(AType *pPtr);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pPtr
		*    Smart pointer to initialize with
		*/
		SmartPtr(const SmartPtr<AType> &pPtr);

		/**
		*  @brief
		*    Destructor
		*/
		~SmartPtr();

		/**
		*  @brief
		*    Assign a pointer
		*
		*  @param[in] pPtr
		*    Direct pointer to assign, can be a null pointer
		*
		*  @return
		*    Reference to the smart pointer
		*/
		SmartPtr<AType> &operator =(AType *pPtr);

		/**
		*  @brief
		*    Assign a smart pointer
		*
		*  @param[in] cPtr
		*    Smart pointer to assign
		*
		*  @return
		*    Reference to the smart pointer
		*/
		SmartPtr<AType> &operator =(const SmartPtr<AType> &cPtr);

		/**
		*  @brief
		*    Get a direct pointer to the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		AType *GetPointer() const;

		/**
		*  @brief
		*    Get a pointer to access the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		AType *operator ->() const;

		/**
		*  @brief
		*    Cast to a pointer to the object
		*
		*  @return
		*    Pointer to the object, can be a null pointer
		*/
		operator AType*() const;

		/**
		*  @brief
		*    Check if the pointer is not a null pointer
		*
		*  @return
		*    'true' if the pointer is not a null pointer
		*/
		bool operator !() const;

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] pPtr
		*    Direct pointer to compare with, can be a null pointer
		*
		*  @return
		*    'true' if the two pointers are equal
		*/
		bool operator ==(AType *pPtr) const;

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] cPtr
		*    Smart pointer to compare with
		*
		*  @return
		*    'true' if the two pointers are equal
		*/
		bool operator ==(const SmartPtr<AType> &cPtr) const;

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] pPtr
		*    Direct pointer to compare with, can be a null pointer
		*
		*  @return
		*    'true' if the two pointers are not equal
		*/
		bool operator !=(AType *pPtr) const;

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] cPtr
		*    Smart pointer to compare with
		*
		*  @return
		*    'true' if the two pointers are not equal
		*/
		bool operator !=(const SmartPtr<AType> &cPtr) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Assign a pointer to an object that does not implement RefCount
		*
		*  @param[in] pPtr
		*    Pointer to assign, can be a null pointer
		*/
		void SetPtr(void *pPtr);

		/**
		*  @brief
		*    Assign a pointer to an object that implements RefCount
		*
		*  @param[in] pPtr
		*    Pointer to assign, can be a null pointer
		*/
		void SetPtr(RefCount<AType> *pPtr);

		/**
		*  @brief
		*    Get pointer to the reference counted object
		*
		*  @return
		*    Pointer to the RefCount object, can be a null pointer
		*/
		RefCount<AType> *GetPtr() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RefCount<AType> *m_pPtr; /**< Pointer to reference counter, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Core/SmartPtr.inl"
