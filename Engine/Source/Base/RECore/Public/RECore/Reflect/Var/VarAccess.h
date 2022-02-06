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
#include "RECore/Reflect/Var/VarStorage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Object;


//[-------------------------------------------------------]
//[ Helper classes                                        ]
//[-------------------------------------------------------]
// Access types
class AccessReadWrite{};	// Allow read and write access to var
class AccessReadOnly {};	// Allow only read access to var


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Access mode for a variable
*
*  @remarks
*    This class template is used to control the access mode of a variable.
*    If AccessReadWrite is specified, a variable can be read and written to.
*    If AccessReadOnly is specified, a variable can only be read.
*/
template <typename T, typename ACCESS, typename STORAGE>
class VarAccess : public VarStorage<T, STORAGE, typename STORAGE::StorageType> {
};


/**
*  @brief
*    Access mode for a variable
*
*  @remarks
*    Implementation for read/write access
*/
template <typename T, typename STORAGE>
class VarAccess<T, AccessReadWrite, STORAGE> : public VarStorage<T, STORAGE, typename STORAGE::StorageType> {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage type
		typedef typename Type<T>::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value of var
		*/
		VarAccess(const _Type &DefaultValue) : VarStorage<T, STORAGE, typename STORAGE::StorageType>(DefaultValue)
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value of var
		*  @param[in] pObject
		*    Pointer to object holding the attribute
		*/
		VarAccess(const _Type &DefaultValue, Object *pObject) : VarStorage<T, STORAGE, typename STORAGE::StorageType>(DefaultValue, pObject)
		{
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		inline _Type get() const
		{
			return VarStorage<T, STORAGE, typename STORAGE::StorageType>::StorageGet();
		}

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] Value
		*    Value
		*/
		inline void set(const _Type &Value)
		{
			VarStorage<T, STORAGE, typename STORAGE::StorageType>::StorageSet(Value);
		}


};


/**
*  @brief
*    Access mode for a variable
*
*  @remarks
*    Implementation for read-only access
*/
template <typename T, typename STORAGE>
class VarAccess<T, AccessReadOnly, STORAGE> : public VarStorage<T, STORAGE, typename STORAGE::StorageType> {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage type
		typedef typename Type<T>::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value of var
		*/
		VarAccess(const _Type &DefaultValue) : VarStorage<T, STORAGE, typename STORAGE::StorageType>(DefaultValue)
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value of var
		*  @param[in] pObject
		*    Pointer to object holding the attribute
		*/
		VarAccess(const _Type &DefaultValue, Object *pObject) : VarStorage<T, STORAGE, typename STORAGE::StorageType>(DefaultValue, pObject)
		{
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		inline _Type get() const
		{
			return VarStorage<T, STORAGE, typename STORAGE::StorageType>::StorageGet();
		}

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] Value
		*    Value
		*/
		inline void set(const _Type &Value)
		{
		}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
