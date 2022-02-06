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
#include "RECore/RECore.h"
#include "RECore/Reflect/Func/Func.h"
#include "RECore/Reflect/Func/FuncDynFunc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Script;


// [TODO] create convenience Wrapper class template?
//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Script function pointer
*
*  @remarks
*    This is a functoid that calls a function of a script
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncScriptPtr : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				m_pScript->pushArgument(t14);
				m_pScript->pushArgument(t15);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				m_pScript->pushArgument(t14);
				m_pScript->pushArgument(t15);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				m_pScript->pushArgument(t14);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				m_pScript->pushArgument(t14);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->pushArgument(t13);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->pushArgument(t12);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->pushArgument(t11);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->pushArgument(t10);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->pushArgument(t9);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->pushArgument(t8);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6, T7> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->pushArgument(t7);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6, T7> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6> : public Func<R, T0, T1, T2, T3, T4, T5, T6> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5, T6>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5, T6> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6> : public Func<void, T0, T1, T2, T3, T4, T5, T6> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5, T6>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->pushArgument(t6);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5, T6> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5> : public Func<R, T0, T1, T2, T3, T4, T5> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4, T5>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4, T5> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5> : public Func<void, T0, T1, T2, T3, T4, T5> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4, T5>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->pushArgument(t5);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4, T5> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template < typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncScriptPtr<R, T0, T1, T2, T3, T4> : public Func<R, T0, T1, T2, T3, T4> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3, T4>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3, T4> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncScriptPtr<void, T0, T1, T2, T3, T4> : public Func<void, T0, T1, T2, T3, T4> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3, T4>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->pushArgument(t4);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3, T4> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class FuncScriptPtr<R, T0, T1, T2, T3> : public Func<R, T0, T1, T2, T3> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2, T3 t3) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2, T3>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2, T3> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class FuncScriptPtr<void, T0, T1, T2, T3> : public Func<void, T0, T1, T2, T3> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2, T3>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->pushArgument(t3);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2, T3> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class FuncScriptPtr<R, T0, T1, T2> : public Func<R, T0, T1, T2> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1, T2 t2) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1, T2>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1, T2> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class FuncScriptPtr<void, T0, T1, T2> : public Func<void, T0, T1, T2> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1, T2>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->pushArgument(t2);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1, T2> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class FuncScriptPtr<R, T0, T1> : public Func<R, T0, T1> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0, T1 t1) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0, T1>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0, T1> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class FuncScriptPtr<void, T0, T1> : public Func<void, T0, T1> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0, T1 t1) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0, T1>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->pushArgument(t1);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0, T1> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class FuncScriptPtr<R, T0> : public Func<R, T0> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()(T0 t0) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R, T0>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R, T0> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class FuncScriptPtr<void, T0> : public Func<void, T0> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(T0 t0) override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void, T0>::GetSignatureID(), m_sNamespace)) {
				m_pScript->pushArgument(t0);
				m_pScript->endCall();
			}
		}

		virtual Func<void, T0> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class FuncScriptPtr<R> : public Func<R> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual R operator ()() override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<R>::GetSignatureID(), m_sNamespace)) {
				if (m_pScript->endCall()) {
					R r = DefaultValue<R>::Default();
					return (R)m_pScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual Func<R> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class FuncScriptPtr<void> : public Func<void> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()() override {
			if (m_pScript && m_pScript->beginCall(m_sFunction, Signature<void>::GetSignatureID(), m_sNamespace))
				m_pScript->endCall();
		}

		virtual Func<void> *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
