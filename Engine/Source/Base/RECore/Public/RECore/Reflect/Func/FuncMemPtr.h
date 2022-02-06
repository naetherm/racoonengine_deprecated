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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Member function pointer
*
*  @remarks
*    This is a functoid that calls a member function of a object
*/
template <typename CLASS, typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncMemPtr : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6> : public Func<R, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6> : public Func<void, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5, t6);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5> : public Func<R, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5> : public Func<void, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4, t5);
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4> : public Func<R, T0, T1, T2, T3, T4> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3, T4> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4> : public Func<void, T0, T1, T2, T3, T4> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3, t4);
		}

		virtual Func<void, T0, T1, T2, T3, T4> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2, typename T3>
class FuncMemPtr<CLASS, R, T0, T1, T2, T3> : public Func<R, T0, T1, T2, T3> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2, T3>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2, T3> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
class FuncMemPtr<CLASS, void, T0, T1, T2, T3> : public Func<void, T0, T1, T2, T3> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2, T3>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2, t3);
		}

		virtual Func<void, T0, T1, T2, T3> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1, typename T2>
class FuncMemPtr<CLASS, R, T0, T1, T2> : public Func<R, T0, T1, T2> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1, T2>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1, t2) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1, T2> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1, typename T2>
class FuncMemPtr<CLASS, void, T0, T1, T2> : public Func<void, T0, T1, T2> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1, T2>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1, t2);
		}

		virtual Func<void, T0, T1, T2> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename CLASS, typename R, typename T0, typename T1>
class FuncMemPtr<CLASS, R, T0, T1> : public Func<R, T0, T1> {
	public:
		typedef typename MethodSignature<CLASS, R, T0, T1>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0, t1) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0, T1> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename CLASS, typename T0, typename T1>
class FuncMemPtr<CLASS, void, T0, T1> : public Func<void, T0, T1> {
	public:
		typedef typename MethodSignature<CLASS, void, T0, T1>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0, t1);
		}

		virtual Func<void, T0, T1> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename CLASS, typename R, typename T0>
class FuncMemPtr<CLASS, R, T0> : public Func<R, T0> {
	public:
		typedef typename MethodSignature<CLASS, R, T0>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()(T0 t0) override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)(t0) : DefaultValue<R>::defaultValue();
		}

		virtual Func<R, T0> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename CLASS, typename T0>
class FuncMemPtr<CLASS, void, T0> : public Func<void, T0> {
	public:
		typedef typename MethodSignature<CLASS, void, T0>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()(T0 t0) override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)(t0);
		}

		virtual Func<void, T0> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename CLASS, typename R>
class FuncMemPtr<CLASS, R> : public Func<R> {
	public:
		typedef typename MethodSignature<CLASS, R>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual R operator ()() override {
			return (m_pMemFunc && m_pObject) ? ((*m_pObject).*m_pMemFunc)() : DefaultValue<R>::defaultValue();
		}

		virtual Func<R> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};

/**
*  @brief
*    Member function pointer
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <typename CLASS>
class FuncMemPtr<CLASS, void> : public Func<void> {
	public:
		typedef typename MethodSignature<CLASS, void>::MemFuncType MEMFUNC;

	public:
		FuncMemPtr(const MEMFUNC &pMemFunc, CLASS *pObject) : m_pMemFunc(pMemFunc), m_pObject(pObject) {
		}

		virtual ~FuncMemPtr() {
		}

		virtual void operator ()() override {
			if (m_pMemFunc && m_pObject)
				((*m_pObject).*m_pMemFunc)();
		}

		virtual Func<void> *clone() const override {
			return new FuncMemPtr(m_pMemFunc, m_pObject);
		}

	private:
		MEMFUNC  m_pMemFunc;	/**< Member function of a class, can be a null pointer */
		CLASS	*m_pObject;		/**< Pointer to object, can be a null pointer */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
