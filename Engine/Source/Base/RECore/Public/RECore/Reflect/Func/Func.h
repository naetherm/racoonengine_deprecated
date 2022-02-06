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
#include "RECore/Reflect/Type/DefaultValue.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters and a return value
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class Func {
	public:
		
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> _MethodType;
		
		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15)
		{
			return DefaultValue<R>::defaultValue();
		}
		
		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};


/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6, T7> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6, T7> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Func<R, T0, T1, T2, T3, T4, T5, T6>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5, T6> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Func<void, T0, T1, T2, T3, T4, T5, T6>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5, T6> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Func<R, T0, T1, T2, T3, T4, T5>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4, T5> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Func<void, T0, T1, T2, T3, T4, T5>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4, T5> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class Func<R, T0, T1, T2, T3, T4>  {
	public:
		typedef Func<R, T0, T1, T2, T3, T4> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class Func<void, T0, T1, T2, T3, T4>  {
	public:
		typedef Func<void, T0, T1, T2, T3, T4> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class Func<R, T0, T1, T2, T3>  {
	public:
		typedef Func<R, T0, T1, T2, T3> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class Func<void, T0, T1, T2, T3>  {
	public:
		typedef Func<void, T0, T1, T2, T3> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class Func<R, T0, T1, T2>  {
	public:
		typedef Func<R, T0, T1, T2> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class Func<void, T0, T1, T2>  {
	public:
		typedef Func<void, T0, T1, T2> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class Func<R, T0, T1>  {
	public:
		typedef Func<R, T0, T1> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0, T1 t1)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class Func<void, T0, T1>  {
	public:
		typedef Func<void, T0, T1> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0, T1 t1)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class Func<R, T0>  {
	public:
		typedef Func<R, T0> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()(T0 t0)
		{
			return DefaultValue<R>::defaultValue();
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class Func<void, T0>  {
	public:
		typedef Func<void, T0> _MethodType;

	public:
		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(T0 t0)
		{
		}

		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class Func<R>  {
	public:
		typedef Func<R> _MethodType;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual R operator ()()
		{
			return DefaultValue<R>::defaultValue();
		}
		
		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class Func<void>  {
	public:
		
		typedef Func<void> _MethodType;
		
		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()()
		{
			
		}
		
		virtual _MethodType *clone() const
		{
			// No function
			return nullptr;
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
