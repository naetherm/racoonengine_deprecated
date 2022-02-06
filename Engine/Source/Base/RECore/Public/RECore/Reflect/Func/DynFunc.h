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
#include "RECore/Core/SmartPtr.h"
#include "RECore/Reflect/Func/DynSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FuncDesc;
class DynParams;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for function objects (functoids)
*
*  @remarks
*    This is the virtual base class to access functions and function objects dynamically.
*    It is a virtual interface that allows you to call a function or function like object
*    regardless of it's actual type (e.g. static function, method of an object or function object).
*/
class DynFunc : public DynSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~DynFunc();


	//[-------------------------------------------------------]
	//[ Public virtual DynFunc functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get function descriptor
		*
		*  @return
		*    Descriptor (can be a null pointer)
		*/
		RECORE_API virtual const FuncDesc *getDesc() const;

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] cParams
		*    Parameters
		*/
		RECORE_API virtual void call(DynParams &cParams);

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] cParams
		*    Parameters
		*/
		RECORE_API virtual void call(const DynParams &cParams);

		/**
		*  @brief
		*    Call function
		*
		*  @param[in] sParams
		*    Parameters as string
		*/
		RECORE_API virtual void call(const String &sParams);

		/**
		*  @brief
		*    Call function with return as string
		*
		*  @param[in] sParams
		*    Parameters as string
		*
		*  @return
		*    Return of the function as string, empty string if there's no return
		*/
		RECORE_API virtual String callWithReturn(const String &sParams);

		/**
		*  @brief
		*    Clone function object
		*
		*  @return
		*    Copy of this functoid (can be a null pointer!)
		*/
		RECORE_API virtual DynFunc *clone() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		RECORE_API DynFunc();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cDynFunc
		*    Source to copy from
		*/
		RECORE_API DynFunc(const DynFunc &cDynFunc);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cDynFunc
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		RECORE_API DynFunc &operator =(const DynFunc &cDynFunc);


};


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
typedef SmartPtr<DynFunc> DynFuncPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
