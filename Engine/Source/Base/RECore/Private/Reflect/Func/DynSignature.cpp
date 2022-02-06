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
#include "RECore/Reflect/Type/Type.h"
#include "RECore/Reflect/Func/DynSignature.h"


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
DynSignature::DynSignature()
{
}

/**
*  @brief
*    Destructor
*/
DynSignature::~DynSignature()
{
}


//[-------------------------------------------------------]
//[ Public virtual DynSignature functions                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get signature as string
*/
String DynSignature::getSignature() const
{
	// No parameters
	return "";
}

/**
*  @brief
*    Get the return type ID
*/
int DynSignature::getReturnTypeID() const
{
	// No function
	return TypeInvalid;
}

/**
*  @brief
*    Return the number of parameters
*/
uint32 DynSignature::getNumOfParameters() const
{
	// No function
	return 0;
}

/**
*  @brief
*    Get a parameter type ID
*/
int DynSignature::getParameterTypeID(uint32 nIndex) const
{
	// No function
	return TypeInvalid;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
