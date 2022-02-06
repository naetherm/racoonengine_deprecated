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
#include "RECore/Reflect/Func/FuncConstructor.h"
#include "RECore/Reflect/Func/ConstructorDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 16 parameters
*/
template <typename CLASS, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncConstructorWrapper : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 15 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncConstructorWrapper<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 14 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 13 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 12 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 11 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 10 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> _BaseWrapperType;
	public:
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
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 9 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 8 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6, T7> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 7 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5, T6> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5, _T6> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5, T6>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 6 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4, T5> : public FuncWrapper<Object*, T0, T1, T2, T3, T4, T5> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4, T5> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4, _T5> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4, T5>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 5 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3, T4> : public FuncWrapper<Object*, T0, T1, T2, T3, T4> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3, T4> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3, _T4> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3, T4>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 4 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
class FuncConstructorWrapper<CLASS*, T0, T1, T2, T3> : public FuncWrapper<Object*, T0, T1, T2, T3> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2, T3> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2, _T3> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2, T3>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2, T3>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 3 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2>
class FuncConstructorWrapper<CLASS*, T0, T1, T2> : public FuncWrapper<Object*, T0, T1, T2> {
	private:
		typedef FuncWrapper<Object*, T0, T1, T2> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1, _T2> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1, T2>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1, T2>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 2 parameters
*/
template <typename CLASS, typename T0, typename T1>
class FuncConstructorWrapper<CLASS*, T0, T1> : public FuncWrapper<Object*, T0, T1> {
	private:
		typedef FuncWrapper<Object*, T0, T1> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0, _T1> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0, T1>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0, T1>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 1 parameters
*/
template <typename CLASS, typename T0>
class FuncConstructorWrapper<CLASS*, T0> : public FuncWrapper<Object*, T0> {
	private:
		typedef FuncWrapper<Object*, T0> _BaseWrapperType;
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS, _T0> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*, T0>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*, T0>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};

/**
*  @brief
*    DynFunc wrapper for a Functoid that creates an object
*
*  @remarks
*    Implementation for up to 0 parameters
*/
template <typename CLASS>
class FuncConstructorWrapper<CLASS*> : public FuncWrapper<Object*> {
	private:
		typedef FuncWrapper<Object*> _BaseWrapperType;
	public:
		typedef CLASS _CLASS;
		typedef FuncConstructor<_CLASS> FuncConstructorType;
		
		FuncConstructorWrapper(FuncConstructorType *pFunc)
				:  FuncWrapper<Object*>(pFunc)
		{
		}
		
		FuncConstructorWrapper(FuncConstructorType *pFunc, bool bTakeOwnerShip)
				:  FuncWrapper<Object*>(pFunc, bTakeOwnerShip)
		{
		}

		virtual ~FuncConstructorWrapper() {
		}

		virtual DynFunc *clone() const override {
			if (_BaseWrapperType::m_pFunc)
				return new FuncConstructorWrapper(static_cast<FuncConstructorType*>(_BaseWrapperType::m_pFunc->clone()), true);
			else
				return new FuncConstructorWrapper(nullptr);
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
