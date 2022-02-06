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
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for signatures
*/
class DynSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API DynSignature();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~DynSignature();


	//[-------------------------------------------------------]
	//[ Public virtual DynSignature functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get signature as string
		*
		*  @return
		*    Signature (e.g. "void(int,float)")
		*/
		RECORE_API virtual String getSignature() const;

		/**
		*  @brief
		*    Get the return type ID
		*
		*  @return
		*    Return type ID (e.g. "TypeNull" for "void()" or "TypeInt" for "int()"), "TypeInvalid" if there's no return type
		*/
		RECORE_API virtual int getReturnTypeID() const;

		/**
		*  @brief
		*    Return the number of parameters
		*
		*  @return
		*    Number of parameters
		*/
		RECORE_API virtual uint32 getNumOfParameters() const;

		/**
		*  @brief
		*    Get a parameter type ID
		*
		*  @param[in] nIndex
		*    Index of the parameter to return the type ID from
		*
		*  @return
		*    Parameter type ID (e.g. "TypeInt" for "void(int)"), "TypeInvalid" on error
		*/
		RECORE_API virtual int getParameterTypeID(uint32 nIndex) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
