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
#include "RECore/Reflect/Func/FuncGenFunPtr.h"
#include "RECore/Reflect/Event/DynEvent.h"
#include "RECore/Reflect/Event/EventHandlerDynEventHandler.h"
#include "RECore/Reflect/Event/Event.h"
#include "RECore/Reflect/Event/EventDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// [TODO] move this to DynEvent when conversion is done
class RECORE_API EventWrapperBase : public DynEvent {
	public:
		EventWrapperBase(const EventDesc *pDesc)
			: m_pDesc(pDesc)
		{
			
		}
		
		virtual ~EventWrapperBase()
		{
			
		}
		
		virtual const EventDesc *getDesc() const
		{
			return m_pDesc;
		}

	private:
		const EventDesc *m_pDesc;


};
/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class RECORE_API EventWrapper : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14), Type<T15>::convertStorageToReal(cP.Param15));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13), Type<T14>::convertStorageToReal(cP.Param14));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12), Type<T13>::convertStorageToReal(cP.Param13));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 >::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11),
						Type<T12>::convertStorageToReal(cP.Param12));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10), Type<T11>::convertStorageToReal(cP.Param11));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9),  Type<T10>::convertStorageToReal(cP.Param10));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8),  Type<T9> ::convertStorageToReal(cP.Param9));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7),
						Type<T8> ::convertStorageToReal(cP.Param8));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6),  Type<T7> ::convertStorageToReal(cP.Param7));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5, T6> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5),  Type<T6> ::convertStorageToReal(cP.Param6));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4, T5> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4),  Type<T5> ::convertStorageToReal(cP.Param5));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class RECORE_API EventWrapper<T0, T1, T2, T3, T4> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3),
						Type<T4> ::convertStorageToReal(cP.Param4));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class RECORE_API EventWrapper<T0, T1, T2, T3> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef EventHandler<_T0, _T1, _T2, _T3>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2, _T3> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2),  Type<T3> ::convertStorageToReal(cP.Param3));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class RECORE_API EventWrapper<T0, T1, T2> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef EventHandler<_T0, _T1, _T2>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1, _T2> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1),  Type<T2> ::convertStorageToReal(cP.Param2));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class RECORE_API EventWrapper<T0, T1> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef EventHandler<_T0, _T1>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1>		TypeParams; // Needed for DynEvent::emit

		EventWrapper(Event<_T0, _T1> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0),  Type<T1> ::convertStorageToReal(cP.Param1));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0, _T1> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class RECORE_API EventWrapper<T0> : public EventWrapperBase {
	public:
		typedef typename Type<T0>::_Type _T0;
	public:
		typedef EventHandler<_T0>	TypeHandler;
		typedef EventHandlerWrapper<T0>	TypeHandlerWrapper;
		typedef Signature<void, T0>	TypeSignature;
		typedef Params<void, T0>	TypeParams;

		EventWrapper(Event<_T0> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc), m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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

		virtual void connect(DynEventHandler &cHandler)
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const { return m_pEvent->getNumOfConnects(); }

		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0));
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// emit event
				(*m_pEvent)(Type<T0> ::convertStorageToReal(cP.Param0));
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void, T0> cParams = Params<void, T0>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<_T0> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class RECORE_API EventWrapper<> : public EventWrapperBase {
	public:
		typedef EventHandler<>	TypeHandler;
		typedef EventHandlerWrapper<>	TypeHandlerWrapper;
		typedef Signature<void>	TypeSignature;
		typedef Params<void>	TypeParams;

		EventWrapper(Event<> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc), m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
		}

		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
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
		
		virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void connect(DynEventHandler &cHandler) override
		{
			if (cHandler.getSignature() != getSignature())
				return;
			
			m_pEvent->connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void disconnect(DynEventHandler &cHandler) override
		{
			m_pEvent->disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 getNumOfConnects() const override { return m_pEvent->getNumOfConnects(); }


		virtual void emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// emit event
				(*m_pEvent)();
			}
		}

		virtual void emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.getSignature() == getSignature()) {
				// emit event
				(*m_pEvent)();
			}
		}

		virtual void emit(const String &sParams) const override
		{
			Params<void> cParams = Params<void>::fromString(sParams);
			emit(cParams);
		}

	private:
		Event<> *m_pEvent;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
