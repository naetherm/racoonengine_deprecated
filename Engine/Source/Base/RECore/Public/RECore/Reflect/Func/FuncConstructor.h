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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for up to 16 parameters
*/
template <typename CLASS, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncConstructor : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 15 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 14 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 13 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 12 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 11 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 10 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 9 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 8 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 7 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) override {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5, T6> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 6 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5> : public Func<Object*, T0, T1, T2, T3, T4, T5> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) override {
			return new CLASS(t0, t1, t2, t3, t4, t5);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4, T5> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 5 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4> : public Func<Object*, T0, T1, T2, T3, T4> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) override {
			return new CLASS(t0, t1, t2, t3, t4);
		}

		virtual Func<Object*, T0, T1, T2, T3, T4> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 4 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
class FuncConstructor<CLASS, T0, T1, T2, T3> : public Func<Object*, T0, T1, T2, T3> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2, T3 t3) override {
			return new CLASS(t0, t1, t2, t3);
		}

		virtual Func<Object*, T0, T1, T2, T3> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 3 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2>
class FuncConstructor<CLASS, T0, T1, T2> : public Func<Object*, T0, T1, T2> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1, T2 t2) override {
			return new CLASS(t0, t1, t2);
		}

		virtual Func<Object*, T0, T1, T2> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 2 parameters
*/
template <typename CLASS, typename T0, typename T1>
class FuncConstructor<CLASS, T0, T1> : public Func<Object*, T0, T1> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0, T1 t1) override {
			return new CLASS(t0, t1);
		}

		virtual Func<Object*, T0, T1> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 1 parameters
*/
template <typename CLASS, typename T0>
class FuncConstructor<CLASS, T0> : public Func<Object*, T0> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()(T0 t0) override {
			return new CLASS(t0);
		}

		virtual Func<Object*, T0> *clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 0 parameters
*/
template <typename CLASS>
class FuncConstructor<CLASS> : public Func<Object*> {
	public:
		FuncConstructor() {
		}

		~FuncConstructor() override {
		}

		Object *operator ()() override {
			return new CLASS();
		}

		virtual Func<Object*> *clone() const override {
			return new FuncConstructor();
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
