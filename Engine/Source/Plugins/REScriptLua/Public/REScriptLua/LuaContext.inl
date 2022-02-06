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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Lua user data types                                   ]
//[-------------------------------------------------------]
// RTTIObjectPointer
inline RTTIObjectPointer &LuaContext::getRTTIObjectPointer(Script &cScript, RECore::Object *pRTTIObject)
{
	RTTIObjectPointer &cInstance = m_lstRTTIObjectPointer.emplace_back();
	cInstance.initializeInstance(cScript, pRTTIObject);
	return cInstance;
}

inline void LuaContext::releaseRTTIObjectPointer(RTTIObjectPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectPointer.erase(std::find(m_lstRTTIObjectPointer.begin(), m_lstRTTIObjectPointer.end(), cInstance));
}

// RTTIObjectSlotPointer
inline RTTIObjectSlotPointer &LuaContext::getRTTIObjectSlotPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventHandlerPtr pDynEventHandler)
{
	RTTIObjectSlotPointer &cInstance = m_lstRTTIObjectSlotPointer.emplace_back();
	cInstance.initializeInstance(cScript, pRTTIObject, pDynEventHandler);
	return cInstance;
}

inline void LuaContext::releaseRTTIObjectSlotPointer(RTTIObjectSlotPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSlotPointer.erase(std::find(m_lstRTTIObjectSlotPointer.begin(), m_lstRTTIObjectSlotPointer.end(), cInstance));
}

// RTTIObjectMethodPointer
inline RTTIObjectMethodPointer &LuaContext::getRTTIObjectMethodPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynFuncPtr pDynFunc)
{
	RTTIObjectMethodPointer &cInstance = m_lstRTTIObjectMethodPointer.emplace_back();
	cInstance.initializeInstance(cScript, pRTTIObject, pDynFunc);
	return cInstance;
}

inline void LuaContext::releaseRTTIObjectMethodPointer(RTTIObjectMethodPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectMethodPointer.erase(std::find(m_lstRTTIObjectMethodPointer.begin(), m_lstRTTIObjectMethodPointer.end(), cInstance));
}

// RTTIObjectSignalPointer
inline RTTIObjectSignalPointer &LuaContext::getRTTIObjectSignalPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventPtr pDynEvent)
{
	RTTIObjectSignalPointer &cInstance = m_lstRTTIObjectSignalPointer.emplace_back();
	cInstance.initializeInstance(cScript, pRTTIObject, pDynEvent);
	return cInstance;
}

inline void LuaContext::releaseRTTIObjectSignalPointer(RTTIObjectSignalPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSignalPointer.erase(std::find(m_lstRTTIObjectSignalPointer.begin(), m_lstRTTIObjectSignalPointer.end(),cInstance));
}

// RTTIObjectSignalMethodPointer
inline RTTIObjectSignalMethodPointer &LuaContext::getRTTIObjectSignalMethodPointer(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventPtr pDynEvent, RTTIObjectSignalMethodPointer::EMethod nMethod)
{
	RTTIObjectSignalMethodPointer &cInstance = m_lstRTTIObjectSignalMethodPointer.emplace_back();
	cInstance.initializeInstance(cScript, pRTTIObject, pDynEvent, nMethod);
	return cInstance;
}

inline void LuaContext::releaseRTTIObjectSignalMethodPointer(RTTIObjectSignalMethodPointer &cInstance)
{
	// Remove the given instance (added to the list of free elements)
	m_lstRTTIObjectSignalMethodPointer.erase(std::find(m_lstRTTIObjectSignalMethodPointer.begin(), m_lstRTTIObjectSignalMethodPointer.end(), cInstance));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
