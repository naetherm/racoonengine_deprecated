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
#include "RECore/Reflect/Func/Func.h"
#include "RECore/Reflect/Func/Signature.h"
#include "RECore/Reflect/Func/Params.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncGenMemBasePtr : public Func<typename Type<R>::_Type,
		 typename Type<T0>::_Type, typename Type<T1>::_Type,
		 typename Type<T2>::_Type, typename Type<T3>::_Type,
		 typename Type<T4>::_Type, typename Type<T5>::_Type,
		 typename Type<T6>::_Type, typename Type<T7>::_Type,
		 typename Type<T8>::_Type, typename Type<T9>::_Type,
		 typename Type<T10>::_Type, typename Type<T11>::_Type,
		 typename Type<T12>::_Type, typename Type<T13>::_Type,
		 typename Type<T14>::_Type, typename Type<T15>::_Type>
{
	
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    This is a functoid that calls a member function of a object
*/
template <typename CLASS, typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncGenMemPtr : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams(t0, t1, t2, t3, t4, t5, t6, t7, t8);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams(t0, t1, t2, t3, t4, t5, t6, t7);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams(t0, t1, t2, t3, t4, t5, t6, t7);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6> cParams(t0, t1, t2, t3, t4, t5, t6);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6> cParams(t0, t1, t2, t3, t4, t5, t6);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5> cParams(t0, t1, t2, t3, t4, t5);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4, _T5> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5> cParams(t0, t1, t2, t3, t4, t5);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4, _T5> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4> : public FuncGenMemBasePtr<R, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4> cParams(t0, t1, t2, t3, t4);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2, _T3, _T4> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4> : public FuncGenMemBasePtr<void, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4> cParams(t0, t1, t2, t3, t4);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3, _T4> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3>
class FuncGenMemPtr<CLASS, R, T0, T1, T2, T3> : public FuncGenMemBasePtr<R, T0, T1, T2, T3> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2, T3> cParams(t0, t1, t2, t3);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<R, _T0, _T1, _T2, _T3> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
class FuncGenMemPtr<CLASS, void, T0, T1, T2, T3> : public FuncGenMemBasePtr<void, T0, T1, T2, T3> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2, T3> cParams(t0, t1, t2, t3);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2, _T3> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2>
class FuncGenMemPtr<CLASS, R, T0, T1, T2> : public FuncGenMemBasePtr<R, T0, T1, T2> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1, T2> cParams(t0, t1, t2);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1, _T2> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2>
class FuncGenMemPtr<CLASS, void, T0, T1, T2> : public FuncGenMemBasePtr<void, T0, T1, T2> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1, T2> cParams(t0, t1, t2);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1, _T2> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1>
class FuncGenMemPtr<CLASS, R, T0, T1> : public FuncGenMemBasePtr<R, T0, T1> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0, T1> cParams(t0, t1);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0, _T1> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1>
class FuncGenMemPtr<CLASS, void, T0, T1> : public FuncGenMemBasePtr<void, T0, T1> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0, T1> cParams(t0, t1);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0, _T1> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename CLASS, typename R, typename T0>
class FuncGenMemPtr<CLASS, R, T0> : public FuncGenMemBasePtr<R, T0> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()(_T0 t0) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R, T0> cParams(t0);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R, _T0> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename CLASS, typename T0>
class FuncGenMemPtr<CLASS, void, T0> : public FuncGenMemBasePtr<void, T0> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()(_T0 t0) override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void, T0> cParams(t0);

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void, _T0> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename CLASS, typename R>
class FuncGenMemPtr<CLASS, R> : public FuncGenMemBasePtr<R> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual _R operator ()() override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<R> cParams;

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);

				// Done, return the result
				return cParams.Return;
			} else {
				return DefaultValue<R>::defaultValue();
			}
		}

		virtual Func<_R> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};

/**
*  @brief
*    Member generic function pointer
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <typename CLASS>
class FuncGenMemPtr<CLASS, void> : public FuncGenMemBasePtr<void> {
	public:
		typedef typename MethodSignature<CLASS, void, DynParams&, void*>::MemFuncType MEMFUNC;

	public:
		FuncGenMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject, void *pUserData = nullptr) : m_pMemFunc(pMemFunc), m_pObject(pObject), m_pUserData(pUserData) {
		}

		virtual ~FuncGenMemPtr() {
		}

		virtual void operator ()() override {
			if (m_pMemFunc && m_pObject) {
				// Get typed params
				Params<void> cParams;

				// Make the generic method call
				((*m_pObject).*m_pMemFunc)(cParams, m_pUserData);
			}
		}

		virtual Func<void> *clone() const override {
			return new FuncGenMemPtr(m_pMemFunc, m_pObject, m_pUserData);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
		void	*m_pUserData;	/**< Pointer to user data, can be a null pointer */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
