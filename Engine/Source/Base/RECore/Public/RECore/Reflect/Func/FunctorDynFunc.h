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
#include "RECore/Reflect/Func/FuncDynFunc.h"
#include "RECore/Reflect/Func/Functor.h"
#include "RECore/Reflect/Func/FuncFunPtr.h"
#include "RECore/Reflect/Func/FuncMemPtr.h"
#include "RECore/Reflect/Func/FuncGenFunPtr.h"
#include "RECore/Reflect/Func/FuncGenMemPtr.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 16 parameters with a return value
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FunctorWrapper : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T1> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T15>::_Type _T15;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T15>::_Type _T15;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 15 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 14 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 13 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 12 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 11 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 10 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 9 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 8 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 7 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5, T6> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5, T6> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 6 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FunctorWrapper<R, T0, T1, T2, T3, T4, T5> : public FuncWrapper<R, T0, T1, T2, T3, T4, T5> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4, T5> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4, _T5> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4, T5>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FunctorWrapper<void, T0, T1, T2, T3, T4, T5> : public FuncWrapper<void, T0, T1, T2, T3, T4, T5> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4, T5> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4, _T5> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4, T5>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 5 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FunctorWrapper<R, T0, T1, T2, T3, T4> : public FuncWrapper<R, T0, T1, T2, T3, T4> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3, T4> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef Functor<_R, _T0, _T1, _T2, _T3, _T4> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3, T4>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3, T4>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class FunctorWrapper<void, T0, T1, T2, T3, T4> : public FuncWrapper<void, T0, T1, T2, T3, T4> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3, T4> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef Functor<void, _T0, _T1, _T2, _T3, _T4> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3, T4>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3, T4>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 4 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class FunctorWrapper<R, T0, T1, T2, T3> : public FuncWrapper<R, T0, T1, T2, T3> {
	private:
		typedef FuncWrapper<R, T0, T1, T2, T3> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef Functor<_R, _T0, _T1, _T2, _T3> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2, T3>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2, T3>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class FunctorWrapper<void, T0, T1, T2, T3> : public FuncWrapper<void, T0, T1, T2, T3> {
	private:
		typedef FuncWrapper<void, T0, T1, T2, T3> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef Functor<void, _T0, _T1, _T2, _T3> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2, T3>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2, T3>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 3 parameters with a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class FunctorWrapper<R, T0, T1, T2> : public FuncWrapper<R, T0, T1, T2> {
	private:
		typedef FuncWrapper<R, T0, T1, T2> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef Functor<_R, _T0, _T1, _T2> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1, T2>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1, T2>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class FunctorWrapper<void, T0, T1, T2> : public FuncWrapper<void, T0, T1, T2> {
	private:
		typedef FuncWrapper<void, T0, T1, T2> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef Functor<void, _T0, _T1, _T2> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1, T2>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1, T2>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 2 parameters with a return value
*/
template <typename R, typename T0, typename T1>
class FunctorWrapper<R, T0, T1> : public FuncWrapper<R, T0, T1> {
	private:
		typedef FuncWrapper<R, T0, T1> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef Functor<_R, _T0, _T1> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0, T1>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0, T1>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class FunctorWrapper<void, T0, T1> : public FuncWrapper<void, T0, T1> {
	private:
		typedef FuncWrapper<void, T0, T1> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef Functor<void, _T0, _T1> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0, T1>(pFunc)
		{
		}

		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0, T1>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 1 parameters with a return value
*/
template <typename R, typename T0>
class FunctorWrapper<R, T0> : public FuncWrapper<R, T0> {
	private:
		typedef FuncWrapper<R, T0> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef Functor<_R, _T0> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R, T0>(pFunc)
		{
		}
		
		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R, T0>(pFunc, bTakeOwnerShip)
		{
		}
		
		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class FunctorWrapper<void, T0> : public FuncWrapper<void, T0> {
	private:
		typedef FuncWrapper<void, T0> _BaseWrapperType;

	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<T0> ::_Type _T0;
		typedef Functor<void, _T0> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void, T0>(pFunc)
		{
		}
		
		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void, T0>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 0 parameters with a return value
*/
template <typename R>
class FunctorWrapper<R> : public FuncWrapper<R> {
	private:
		typedef FuncWrapper<R> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef Functor<_R> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<R>(pFunc)
		{
		}
		
		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<R>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class FunctorWrapper<void> : public FuncWrapper<void> {
	private:
		typedef FuncWrapper<void> _BaseWrapperType;


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef Functor<void> FunctorType;
		
		FunctorWrapper(FunctorType *pFunc)
				:  FuncWrapper<void>(pFunc)
		{
		}
		
		FunctorWrapper(FunctorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<void>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FunctorWrapper()
		{
		}

		virtual DynFunc *clone() const override
		{
			if (_BaseWrapperType::m_pFunc)
				return new FunctorWrapper(static_cast<FunctorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FunctorWrapper(nullptr);
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
