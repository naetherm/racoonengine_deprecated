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
#include "RECore/Reflect/Func/DynFunc.h"
#include "RECore/Reflect/Func/DynParams.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DynFunc::~DynFunc()
{
}


//[-------------------------------------------------------]
//[ Public virtual DynFunc functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get function descriptor
*/
const FuncDesc *DynFunc::getDesc() const
{
	// No descriptor by default
	return nullptr;
}

/**
*  @brief
*    Call function
*/
void DynFunc::call(DynParams &cParams)
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynFunc::call(const DynParams &cParams)
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynFunc::call(const String &sParams)
{
	// No function
}

/**
*  @brief
*    Call function with return as string
*/
String DynFunc::callWithReturn(const String &sParams)
{
	// No function
	return "";
}

/**
*  @brief
*    Clone function object
*/
DynFunc *DynFunc::clone() const
{
	// No function
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
DynFunc::DynFunc()
{
}

/**
*  @brief
*    Copy constructor
*/
DynFunc::DynFunc(const DynFunc &cDynFunc)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*
*/
DynFunc &DynFunc::operator =(const DynFunc &cDynFunc)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
