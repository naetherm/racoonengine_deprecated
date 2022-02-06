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
#include "RECore/Core/SmartPtr.h"
#include "RECore/Reflect/Func/Signature.h"
#include "RECore/Reflect/Func/DynSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class EventDesc;
class DynParams;
class DynEventHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for events
*
*  @remarks
*    This is the virtual base class to access events dynamically.
*
*  @note
*    - Implementation of the observer design pattern (this class is the subject/observable, the source)
*/
class DynEvent : public DynSignature {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef Signature<void, DynParams&, void*>::FuncType FUNC;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API DynEvent();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~DynEvent();

		/**
		*  @brief
		*    Connect event handler to the event
		*
		*  @param[in] cHandler
		*    Event handler
		*/
		RECORE_API virtual void connect(DynEventHandler &cHandler);

		/**
		*  @brief
		*    Disconnect event handler from the event
		*
		*  @param[in] cHandler
		*    Event handler
		*/
		RECORE_API virtual void disconnect(DynEventHandler &cHandler);

		/**
		*  @brief
		*    Return the number of connections
		*
		*  @return
		*    The number of connections
		*
		*  @note
		*    - Don't use this method within performance critical situations because due internal
		*      implementation, this request has a linear, not constant runtime!
		*/
		RECORE_API virtual uint32 getNumOfConnects() const { return 0; }


	//[-------------------------------------------------------]
	//[ Public virtual DynEvent functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get event descriptor
		*
		*  @return
		*    Descriptor (can be a null pointer)
		*/
		RECORE_API virtual const EventDesc *getDesc() const;

		/**
		*  @brief
		*    Create a generic event handler which is compatible with this dynamic event
		*
		*  @param[in] pFunc
		*    Function to be called by the dynamic event handler
		*  @param[in] pUserData
		*    Optional pointer to user data to pass on to the given generic function when the event was emitted, can be a null pointer
		*
		*  @return
		*    The generic event handler which is compatible with this dynamic event, null pointer on error (delete the created instance if you no longer need it)
		*
		*  @note
		*    - This method just creates an instance of the generic event handler and does not automatically connect it with this dynamic event
		*    - Use typed instead of generic event handlers whenever possible, this method only exists for situations were no type information
		*      is available at compile time
		*/
		RECORE_API virtual DynEventHandler *createGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const;

		/**
		*  @brief
		*    emit event
		*
		*  @param[in] cParams
		*    Parameters
		*/
		RECORE_API virtual void emit(DynParams &cParams) const;

		/**
		*  @brief
		*    emit event
		*
		*  @param[in] cParams
		*    Parameters
		*/
		RECORE_API virtual void emit(const DynParams &cParams) const;

		/**
		*  @brief
		*    emit event
		*
		*  @param[in] sParams
		*    Parameters as string
		*/
		RECORE_API virtual void emit(const String &sParams) const;

};


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
typedef SmartPtr<DynEvent> DynEventPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
