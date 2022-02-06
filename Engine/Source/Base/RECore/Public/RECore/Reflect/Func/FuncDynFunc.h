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
#include "RECore/Reflect/Func/Signature.h"
#include "RECore/Reflect/Func/Params.h"
#include "RECore/Reflect/Func/DynFunc.h"
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
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters and a return value
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncWrapper : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 16;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				case 15:	return Type<T15>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
																  Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 16;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				case 15:	return Type<T15>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 15;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
																  Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 15;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 14;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
																  Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 14;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 13;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
																  Type<T12>::convertStorageToReal(cP.Param12)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 13;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 12;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 12;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 11;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 11;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 10;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 10;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 9;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
																  Type<T8> ::convertStorageToReal(cP.Param8)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynFunc {
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
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 9;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6, T7> : public DynFunc {
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
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 8;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6, T7> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 8;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5, T6> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5, _T6> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 7;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5, T6> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5, _T6> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 7;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncWrapper<R, T0, T1, T2, T3, T4, T5> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4, _T5> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 6;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncWrapper<void, T0, T1, T2, T3, T4, T5> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef Func<void, _T0, _T1, _T2, _T3, _T4, _T5> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 6;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 5 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncWrapper<R, T0, T1, T2, T3, T4> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef Func<_R, _T0, _T1, _T2, _T3, _T4> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 5;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
																  Type<T4> ::convertStorageToReal(cP.Param4)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncWrapper<void, T0, T1, T2, T3, T4> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef Func<void, _T0, _T1, _T2, _T3, _T4> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 5;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class FuncWrapper<R, T0, T1, T2, T3> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef Func<_R, _T0, _T1, _T2, _T3> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2, T3>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 4;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3> &cP =
					static_cast<Params<R, T0, T1, T2, T3>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3> &cP =
					static_cast<const Params<R, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class FuncWrapper<void, T0, T1, T2, T3> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef Func<void, _T0, _T1, _T2, _T3> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2, T3>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 4;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3> &cP =
					static_cast<Params<void, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3> &cP =
					static_cast<const Params<void, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class FuncWrapper<R, T0, T1, T2> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef Func<_R, _T0, _T1, _T2> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1, T2>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 3;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1, T2> &cP =
					static_cast<Params<R, T0, T1, T2>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2> &cP =
					static_cast<const Params<R, T0, T1, T2>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class FuncWrapper<void, T0, T1, T2> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef Func<void, _T0, _T1, _T2> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1, T2>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 3;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1, T2> &cP =
					static_cast<Params<void, T0, T1, T2>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2> &cP =
					static_cast<const Params<void, T0, T1, T2>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class FuncWrapper<R, T0, T1> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef Func<_R, _T0, _T1> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0, T1>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 2;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0, T1> &cP = static_cast<Params<R, T0, T1>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0, T1> &cP = static_cast<const Params<R, T0, T1>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 2 parameters without a return value
*/
template <typename T0, typename T1>
class FuncWrapper<void, T0, T1> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef Func<void, _T0, _T1> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0, T1>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 2;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0, T1> &cP = static_cast<Params<void, T0, T1>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0, T1> &cP = static_cast<const Params<void, T0, T1>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 1 parameters and a return value
*/
template <typename R, typename T0>
class FuncWrapper<R, T0> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef Func<_R, _T0> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R, T0>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 1;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R, T0> &cP = static_cast<Params<R, T0>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0)));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<R, T0> &cP = static_cast<const Params<R, T0>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R, T0> cParams = Params<R, T0>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R, T0> cParams = Params<R, T0>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    DynFunc wrapper to a Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 1 parameters without a return value
*/
template <typename T0>
class FuncWrapper<void, T0> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef Func<void, _T0> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void, T0>::getSignatureID();
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 1;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<void, T0> &cP = static_cast<Params<void, T0>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0));
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const Params<void, T0> &cP = static_cast<const Params<void, T0>&>(cParams);

				// Call function
				(*m_pFunc)(Type<T0> ::convertStorageToReal(cP.Param0));
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void, T0> cParams = Params<void, T0>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void, T0> cParams = Params<void, T0>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}


	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class FuncWrapper<R> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef Func<_R> FuncType;
		
		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<R>::getSignatureID();
		}

		virtual int getReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 getNumOfParameters() const override
		{
			return 0;
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			// There are no candidates, so the choice is pretty simple
			return TypeInvalid;
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				Params<R> &cP = static_cast<Params<R>&>(cParams);

				// Call function
				cP.Return = Type<R>::convertRealToStorage((*m_pFunc)());
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Call function
				(*m_pFunc)();
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<R> cParams = Params<R>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<R> cParams = Params<R>::fromString(sParams);
			call(cParams);
			return Type<R>::convertToString(Type<R>::convertStorageToReal(cParams.Return));
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}

	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class FuncWrapper<void> : public DynFunc {
	public:
		typedef Func<void> FuncType;

		FuncWrapper() : m_pFunc(nullptr), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(Func<void>* pFunc)
			: m_pFunc(pFunc), m_bTakeOwnerShip(false)
		{
		}
		
		FuncWrapper(FuncType *pFunc, bool bTakeOwnerShip)
			: m_pFunc(pFunc), m_bTakeOwnerShip(bTakeOwnerShip)
		{
		}

		virtual ~FuncWrapper()
		{
			if(m_bTakeOwnerShip && m_pFunc)
				delete m_pFunc;
		}

		virtual String getSignature() const override
		{
			return Signature<void>::getSignatureID();
		}

		virtual int getParameterTypeID(uint32 nIndex) const override
		{
			// There are no candidates, so the choice is pretty simple
			return TypeInvalid;
		}

		virtual void call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Call function
				(*m_pFunc)();
			}
		}

		virtual void call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Call function
				(*m_pFunc)();
			}
		}

		virtual void call(const String &sParams) override
		{
			Params<void> cParams = Params<void>::fromString(sParams);
			call(cParams);
		}

		virtual String callWithReturn(const String &sParams) override
		{
			Params<void> cParams = Params<void>::fromString(sParams);
			call(cParams);
			return "";
		}

		virtual DynFunc *clone() const override
		{
			return new FuncWrapper(m_pFunc ? m_pFunc->clone() : nullptr, true);
		}

	protected:
		FuncType *m_pFunc;
	
	private:
		bool m_bTakeOwnerShip;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
