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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/Reflect/Func/DynParams.h"
#include "RECore/Reflect/Event/DynEventHandler.h"
#include "RECore/Reflect/Event/DynEvent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DynEvent::DynEvent()
{
}

/**
*  @brief
*    Destructor
*/
DynEvent::~DynEvent()
{

}

/**
*  @brief
*    Connect event handler to the event
*/
void DynEvent::connect(DynEventHandler &cHandler)
{
}

/**
*  @brief
*    Disconnect a handler from the event
*/
void DynEvent::disconnect(DynEventHandler &cHandler)
{
}


//[-------------------------------------------------------]
//[ Public virtual DynEvent functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get event descriptor
*/
const EventDesc *DynEvent::getDesc() const
{
	// No descriptor by default
	return nullptr;
}

/**
*  @brief
*    Create a generic event handler which is compatible with this dynamic event
*/
DynEventHandler *DynEvent::createGenericEventHandler(const FUNC &pFunc, void *pUserData) const
{
	// No function
	return nullptr;
}

/**
*  @brief
*    Call function
*/
void DynEvent::emit(DynParams &cParams) const
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynEvent::emit(const DynParams &cParams) const
{
	// No function
}

/**
*  @brief
*    emit event
*/
void DynEvent::emit(const String &sParams) const
{
	// No function
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
