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
#include "RECore/Reflect/Event/EventHandler.h"
#include "RECore/Reflect/Event/DynEventHandler.h"
#include "RECore/Reflect/Event/EventHandlerDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class EventWrapper;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// [TODO] move this to DynEvent when conversion is done
class EventHandlerWrapperBase : public DynEventHandler {
	public:
		EventHandlerWrapperBase(const EventHandlerDesc *pDesc)
			: m_pDesc(pDesc)
		{
			
		}
		
		virtual ~EventHandlerWrapperBase()
		{
			
		}
		
		virtual const EventHandlerDesc *getDesc() const
		{
			return m_pDesc;
		}

	private:
		const EventHandlerDesc *m_pDesc;


};
/**
*  @brief
*    DynEventHandler wrapper of a Generic event handler class
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class EventHandlerWrapper : public EventHandlerWrapperBase {


	friend class EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>;
	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15>		EventHandlerType; // Needed as Member
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeSignature; // Needed for DynEventHandler::GetSignature (from DynSignature)


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		EventHandlerWrapper(EventHandlerType *pHandler, const EventHandlerDesc *pDesc) :
			EventHandlerWrapperBase(pDesc), m_bTakeOwnership(false), m_pHandler(pHandler)
		{
		}
		
		EventHandlerWrapper(EventHandlerType *pHandler, const EventHandlerDesc *pDesc, bool takeOwnership) :
			EventHandlerWrapperBase(pDesc), m_bTakeOwnership(takeOwnership), m_pHandler(pHandler)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		~EventHandlerWrapper()
		{
			if (m_bTakeOwnership)
				delete m_pHandler;
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynEventHandler functions              ]
	//[-------------------------------------------------------]
	public:
		virtual String getSignature() const override
		{
			return TypeSignature::getSignatureID();
		}


	//[-------------------------------------------------------]
	//[ private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				 m_bTakeOwnership;	/**< Indicates if this wrapper takes ownershipt of the eventhandler (deletes handler on destruction of this wrapper) */
		EventHandlerType 	*m_pHandler;		/**< Functor containing the event handler */

};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
