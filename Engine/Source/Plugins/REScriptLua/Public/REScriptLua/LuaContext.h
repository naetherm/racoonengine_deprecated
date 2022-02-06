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
#include <RECore/Core/AbstractContext.h>
#include "REScriptLua/RTTIObjectPointer.h"
#include "REScriptLua/RTTIObjectSlotPointer.h"
#include "REScriptLua/RTTIObjectMethodPointer.h"
#include "REScriptLua/RTTIObjectSignalPointer.h"
#include "REScriptLua/RTTIObjectSignalMethodPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static Lua context
*/
class LuaContext : public RECore::AbstractContext {


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Adds a context reference
		*
		*  @note
		*    - If this is the first reference, the internal Lua context is created automatically
		*/
		static void addContextReference();

		/**
		*  @brief
		*    Releases a context reference
		*
		*  @note
		*    - If this is the last reference, the internal Lua context is destroyed automatically
		*/
		static void releaseContextReference();

		//[-------------------------------------------------------]
		//[ Lua user data types                                   ]
		//[-------------------------------------------------------]
		// RTTIObjectPointer
		static inline RTTIObjectPointer &getRTTIObjectPointer(Script &cScript, RECore::Object *pRTTIObject);
		static inline void releaseRTTIObjectPointer(RTTIObjectPointer &cInstance);
		// RTTIObjectSlotPointer
		static inline RTTIObjectSlotPointer &getRTTIObjectSlotPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventHandlerPtr pDynEventHandler);
		static inline void releaseRTTIObjectSlotPointer(RTTIObjectSlotPointer &cInstance);
		// RTTIObjectMethodPointer
		static inline RTTIObjectMethodPointer &getRTTIObjectMethodPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynFuncPtr pDynFunc);
		static inline void releaseRTTIObjectMethodPointer(RTTIObjectMethodPointer &cInstance);
		// RTTIObjectSignalPointer
		static inline RTTIObjectSignalPointer &getRTTIObjectSignalPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventPtr pDynEvent);
		static inline void releaseRTTIObjectSignalPointer(RTTIObjectSignalPointer &cInstance);
		// RTTIObjectSignalMethodPointer
		static inline RTTIObjectSignalMethodPointer &getRTTIObjectSignalMethodPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventPtr pDynEvent, RTTIObjectSignalMethodPointer::EMethod nMethod);
		static inline void releaseRTTIObjectSignalMethodPointer(RTTIObjectSignalMethodPointer &cInstance);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static RECore::uint32 m_nContexCounter;	/**< Lua context counter */

		// Lua user data types
		static std::list<RTTIObjectPointer>				m_lstRTTIObjectPointer;				/**< Fast pool for "RTTIObjectPointer"-instances */
		static std::list<RTTIObjectSlotPointer>			m_lstRTTIObjectSlotPointer;			/**< Fast pool for "RTTIObjectSlotPointer"-instances */
		static std::list<RTTIObjectMethodPointer>		m_lstRTTIObjectMethodPointer;		/**< Fast pool for "RTTIObjectMethodPointer"-instances */
		static std::list<RTTIObjectSignalPointer>		m_lstRTTIObjectSignalPointer;		/**< Fast pool for "RTTIObjectSignalPointer"-instances */
		static std::list<RTTIObjectSignalMethodPointer>	m_lstRTTIObjectSignalMethodPointer;	/**< Fast pool for "RTTIObjectSignalMethodPointer"-instances */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "REScriptLua/LuaContext.inl"
