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
#include "RECore/Reflect/Tools/TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
// Enum type
template <typename T>
class EnumType;

// Flag type
template <typename T>
class FlagType;

template <typename T>
struct CheckType;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename T>
class DefaultValue : public DefaultValue< typename CheckType<T>::Type > {
};

template <>
class DefaultValue<bool> {
	public:
		static bool defaultValue() {
			return false;
		}
};

/*
template <>
class DefaultValue<int> {
	public:
		static int Default() {
			return 0;
		}
};
*/

template <>
class DefaultValue<int8> {
	public:
		static int8 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<int16> {
	public:
		static int16 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<int32> {
	public:
		static int32 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<int64> {
	public:
		static int64 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<uint8> {
	public:
		static uint8 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<uint16> {
	public:
		static uint16 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<uint32> {
	public:
		static uint32 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<uint64> {
	public:
		static uint64 defaultValue() {
			return 0;
		}
};

template <>
class DefaultValue<float> {
	public:
		static float defaultValue() {
			return 0.0f;
		}
};

template <>
class DefaultValue<double> {
	public:
		static double defaultValue() {
			return 0.0;
		}
};

template <>
class DefaultValue<String> {
	public:
		static String defaultValue() {
			return "";
		}
};

template <typename T>
class DefaultValue<T*> {
	public:
		static T* defaultValue() {
			return nullptr;
		}
};

template <typename T>
class DefaultValue<T&> {
	public:
		static T& defaultValue() {
			return *static_cast<T*>(nullptr);
		}
};

template <typename ENUM>
class DefaultValue< EnumTypePlain<ENUM> > {
	public:
		static ENUM defaultValue() {
			return static_cast<ENUM>(0);
		}
};

template <typename ENUM>
class DefaultValue< EnumType<ENUM> > {
	public:
		// Type
		typedef typename ENUM::_Type _Type;

		static _Type defaultValue() {
			return DefaultValue<_Type>::defaultValue();
		}
};

template <typename ENUM>
class DefaultValue< FlagType<ENUM> > {
	public:
		// Type
		typedef typename ENUM::_Type _Type;

		static _Type defaultValue() {
			return DefaultValue<_Type>::defaultValue();
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
