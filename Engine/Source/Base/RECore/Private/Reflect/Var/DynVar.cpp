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
#include "RECore/Reflect/ReflectTypes.h"
#include "RECore/Reflect/Type/Type.h"
#include "RECore/Reflect/Var/DynVar.h"


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
DynVar::DynVar()
{
}

/**
*  @brief
*    Destructor
*/
DynVar::~DynVar()
{
}


//[-------------------------------------------------------]
//[ Public virtual DynVar functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get variable descriptor
*/
const VarDesc *DynVar::getDesc() const
{
	// No descriptor by default
	return nullptr;
}

/**
*  @brief
*    Get type
*/
DynTypeInfo &DynVar::getType() const
{
	// Return unknown type
	return TypeInfo<NullType>::Instance;
}

/**
*  @brief
*    Get type ID
*/
int DynVar::getTypeID() const
{
	// Return unknown type
	return TypeNull;
}

/**
*  @brief
*    Get type name
*/
String DynVar::getTypeName() const
{
	// Return unknown type
	return "";
}

/**
*  @brief
*    Check if variable is set to default value
*/
bool DynVar::isDefault() const
{
	// No data
	return false;
}

/**
*  @brief
*    Set variable to default value
*/
void DynVar::setDefault()
{
	// No data
}

/**
*  @brief
*    Get default value as string
*/
String DynVar::getDefault() const
{
	// No data
	return "";
}

/**
*  @brief
*    Set value
*/
void DynVar::setVar(const DynVar &cValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
bool DynVar::getBool() const
{
	// No data available
	return false;
}

/**
*  @brief
*    Set value
*/
void DynVar::setBool(bool bValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
int DynVar::getInt() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setInt(int nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
int8 DynVar::getInt8() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setInt8(int8 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
int16 DynVar::getInt16() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setInt16(int16 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
int32 DynVar::getInt32() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setInt32(int32 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
int64 DynVar::getInt64() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setInt64(int64 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
uint8 DynVar::getUInt8() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setUInt8(uint8 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
uint16 DynVar::getUInt16() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setUInt16(uint16 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
uint32 DynVar::getUInt32() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setUInt32(uint32 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
uint64 DynVar::getUInt64() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setUInt64(uint64 nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
uint_ptr DynVar::getUIntPtr() const
{
	// No data available
	return 0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setUIntPtr(uint_ptr nValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
float DynVar::getFloat() const
{
	// No data available
	return 0.0f;
}

/**
*  @brief
*    Set value
*/
void DynVar::setFloat(float fValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
double DynVar::getDouble() const
{
	// No data available
	return 0.0;
}

/**
*  @brief
*    Set value
*/
void DynVar::setDouble(double dValue)
{
	// No data
}

/**
*  @brief
*    Get value
*/
String DynVar::getString() const
{
	// No data available
	return "";
}

/**
*  @brief
*    Set value
*/
void DynVar::setString(const String &sValue)
{
	// No data
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
