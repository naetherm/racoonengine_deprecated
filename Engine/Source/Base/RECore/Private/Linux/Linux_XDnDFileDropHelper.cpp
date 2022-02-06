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
#include <string.h>
#include "RECore/String/BasicTokenizer.h"
#include "RECore/Linux/WindowOSLinux.h"
#include "RECore/Linux/Linux_XDnDFileDropHelper.h"


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
Linux_XDnDFileDropHelper::Linux_XDnDFileDropHelper(WindowOSLinux &cOSWindowLinux) :
	m_pOSWindowLinux(&cOSWindowLinux),
	m_pDisplay(cOSWindowLinux.m_pDisplay),
	m_nDropWindow(cOSWindowLinux.m_nNativeWindowHandle),
	XdndEnter     (XInternAtom(m_pDisplay, "XdndEnter",      False)),
	XdndPosition  (XInternAtom(m_pDisplay, "XdndPosition",   False)),
	XdndStatus    (XInternAtom(m_pDisplay, "XdndStatus",     False)),
	XdndTypeList  (XInternAtom(m_pDisplay, "XdndTypeList",   False)),
	XdndActionCopy(XInternAtom(m_pDisplay, "XdndActionCopy", False)),
	XdndDrop      (XInternAtom(m_pDisplay, "XdndDrop",       False)),
	XdndFinished  (XInternAtom(m_pDisplay, "XdndFinished",   False)),
	XdndSelection (XInternAtom(m_pDisplay, "XdndSelection",  False)),
	// This is a meta-format for data to be "pasted" in to.
	// Requesting this format acquires a list of possible
	// formats from the application which copied the data.
	XA_TARGETS(XInternAtom(m_pDisplay, "TARGETS", False)),
	m_XdndSourceWindow(XLib::None),
	m_ToBeRequestedType(XLib::None),
	m_XdndVersion(0)
{
	// Announce XDND support
	Atom XdndAware = XInternAtom(m_pDisplay, "XdndAware", False);
	Atom version = 5;
	XChangeProperty(m_pDisplay, m_nDropWindow, XdndAware, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&version), 1);
}

/**
*  @brief
*    Handler for client messages
*/
bool Linux_XDnDFileDropHelper::handleClientMessage(const XClientMessageEvent &sClientMessage)
{
	if (sClientMessage.message_type == XdndEnter)
		handleXdndEnter(sClientMessage);
	else if (sClientMessage.message_type == XdndPosition)
		handleXdndPosition(sClientMessage);
	else if (sClientMessage.message_type == XdndDrop)
		handleXdndDrop(sClientMessage);
	else
		return false;	// Message was not handled

	// Message was handled
	return true;
}

/**
*  @brief
*    Handler for the SelectionNotify message
*/
void Linux_XDnDFileDropHelper::handleXdndSelection(const XSelectionEvent &sSelectionMessage)
{
	if (sSelectionMessage.property != XLib::None && sSelectionMessage.selection == XdndSelection) {
		// Check if target is the requested one
		if (sSelectionMessage.target == m_ToBeRequestedType) {
			PropertyData sPropertyData = readProperty(m_nDropWindow, XdndSelection);

			// Reply OK
			XClientMessageEvent sXClientMessageEvent;
			memset(&sXClientMessageEvent, 0, sizeof(sXClientMessageEvent));
			sXClientMessageEvent.type			= ClientMessage;
			sXClientMessageEvent.display		= m_pDisplay;
			sXClientMessageEvent.window			= m_XdndSourceWindow;
			sXClientMessageEvent.message_type	= XdndFinished;
			sXClientMessageEvent.format			= 32;
			sXClientMessageEvent.data.l[0]		= m_nDropWindow;
			sXClientMessageEvent.data.l[1]		= 1;
			sXClientMessageEvent.data.l[2]		= XdndActionCopy;	// We only ever copy

			// Reply that all is well
			XSendEvent(m_pDisplay, m_XdndSourceWindow, False, NoEventMask, reinterpret_cast<XEvent*>(&sXClientMessageEvent));

			XSync(m_pDisplay, False);

			const String sPath(String(reinterpret_cast<char*>(sPropertyData.data)));

			XFree(sPropertyData.data);

			// Create the file list
			CTokenizer cTokenizer;
			cTokenizer.SetDelimiters("\n\r");
			cTokenizer.SetSingleLineComment("");
			cTokenizer.SetQuotes("");
			cTokenizer.SetSingleChars("");
			cTokenizer.Start(sPath);
			std::vector<String> lstFiles = cTokenizer.GetTokens();

			// Inform the frontend
			m_pOSWindowLinux->onDrop(lstFiles);
		}
	} else {
		// The selection can not be converted or isn't in the XdndSelection buffer -> ignore
	}
}


//[-------------------------------------------------------]
//[ Private helper functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fetches all data from a property (Atom)
*/
Linux_XDnDFileDropHelper::PropertyData Linux_XDnDFileDropHelper::readProperty(Window nNativeWindowHandle, Atom sProperty)
{
	Atom 		   sActualType;
	int			   nActualFormat;
	unsigned long  nNumOfItems;
	unsigned long  nBytesAfter;
	unsigned char *pnReturn = nullptr;
	int 		   nReadBytes = 1024;

	// Keep trying to read the property until there are no bytes unread
	do {
		if (pnReturn)
			XFree(pnReturn);
		XGetWindowProperty(m_pDisplay, nNativeWindowHandle, sProperty, 0, nReadBytes, False, AnyPropertyType,
						   &sActualType, &nActualFormat, &nNumOfItems, &nBytesAfter, &pnReturn);
		nReadBytes *= 2;
	} while (nBytesAfter != 0);

	// Done
	PropertyData sPropertyData = {pnReturn, nActualFormat, nNumOfItems, sActualType};
	return sPropertyData;
}

/**
*  @brief
*    Convert an atom name in to a String
*/
String Linux_XDnDFileDropHelper::getAtomName(Atom sAtom)
{
	return (sAtom == XLib::None) ? "None" : XGetAtomName(m_pDisplay, sAtom);
}

/**
*  @brief
*    Checks if the drop source supports the "text/uri-list" drop target type
*/
Atom Linux_XDnDFileDropHelper::checkForSupportedTargetTypeFromAtomList(Atom *pAtomList, int nNumOfItems)
{
	Atom sToBeRequested = XLib::None;

	for (int i=0; i<nNumOfItems; i++) {
		if (getAtomName(pAtomList[i]) == "text/uri-list")
			sToBeRequested = pAtomList[i];
	}

	return sToBeRequested;
}

/**
*  @brief
*    Checks if the drop source supports the "text/uri-list" drop target type (from three Atoms)
*/
Atom Linux_XDnDFileDropHelper::checkForSupportedTargetTypFromAtoms(Atom sAtom1, Atom sAtom2, Atom sAtom3)
{
	Atom sAtoms[3];
	int nNumOfAtoms = 0;

	if (sAtom1 != XLib::None)
		sAtoms[nNumOfAtoms++] = sAtom1;
	if (sAtom2 != XLib::None)
		sAtoms[nNumOfAtoms++] = sAtom2;
	if (sAtom3 != XLib::None)
		sAtoms[nNumOfAtoms++] = sAtom3;

	return checkForSupportedTargetTypeFromAtomList(sAtoms, nNumOfAtoms);
}

/**
*  @brief
*    Checks if the drop source supports the "text/uri-list" drop target type (from an Property)
*/
Atom Linux_XDnDFileDropHelper::checkForSupportedTargetTypFromProperty(PropertyData sPropertyData)
{
	// The list of targets is a list of atoms, so it should have type XA_ATOM
	// but it may have the type TARGETS instead
	if ((sPropertyData.type != XA_ATOM && sPropertyData.type != XA_TARGETS) || sPropertyData.format != 32) {
		// This would be really broken. Targets have to be an atom list
		// and applications should support this. Nevertheless, some
		// seem broken (MATLAB 7, for instance), so ask for STRING
		// next instead as the lowest common denominator
		return XA_STRING;
	} else {
		return checkForSupportedTargetTypeFromAtomList(reinterpret_cast<Atom*>(sPropertyData.data), sPropertyData.nNumOfItems);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Linux_XDnDFileDropHelper::Linux_XDnDFileDropHelper(const Linux_XDnDFileDropHelper &cOther)
{
}

/**
*  @brief
*    Assignment operator
*/
Linux_XDnDFileDropHelper &Linux_XDnDFileDropHelper::operator =(const Linux_XDnDFileDropHelper &cOther)
{
	return *this;
}

/**
*  @brief
*    Handler for the XDnDEnter client message
*/
void Linux_XDnDFileDropHelper::handleXdndEnter(const XClientMessageEvent &sClientMessage)
{
	const bool bMoreThan3 = (sClientMessage.data.l[1] & 1);
	const Window nSource = sClientMessage.data.l[0];

	m_XdndVersion = (sClientMessage.data.l[1] >> 24);

	// Query which conversions are available and pick the best
	if (bMoreThan3) {
		// Fetch the list of possible conversions
		PropertyData sPropertyData = readProperty(nSource, XdndTypeList);
		m_ToBeRequestedType = checkForSupportedTargetTypFromProperty(sPropertyData);
		XFree(sPropertyData.data);
	} else {
		// Use the available list
		m_ToBeRequestedType = checkForSupportedTargetTypFromAtoms(sClientMessage.data.l[2], sClientMessage.data.l[3], sClientMessage.data.l[4]);
	}
}

/**
*  @brief
*    Handler for the XDnDPosition client message
*/
void Linux_XDnDFileDropHelper::handleXdndPosition(const XClientMessageEvent &sClientMessage)
{
	Atom sAction = XdndActionCopy;
	if (m_XdndVersion >= 2)
		sAction = sClientMessage.data.l[4];

	// Xdnd: Reply with an XDND status message
	XClientMessageEvent sXClientMessageEvent;
	memset(&sXClientMessageEvent, 0, sizeof(sXClientMessageEvent));
	sXClientMessageEvent.type			= ClientMessage;
	sXClientMessageEvent.display		= sClientMessage.display;
	sXClientMessageEvent.window			= sClientMessage.data.l[0];
	sXClientMessageEvent.message_type	= XdndStatus;
	sXClientMessageEvent.format			= 32;
	sXClientMessageEvent.data.l[0]		= m_nDropWindow;
	sXClientMessageEvent.data.l[1]		= (m_ToBeRequestedType != XLib::None);
	sXClientMessageEvent.data.l[2]		= m_pOSWindowLinux->getWidth();
	sXClientMessageEvent.data.l[3]		= m_pOSWindowLinux->getHeight();
	sXClientMessageEvent.data.l[4]		= XdndActionCopy;	// We only accept copying anyway

	// Send the X-event
	XSendEvent(m_pDisplay, sClientMessage.data.l[0], False, NoEventMask, reinterpret_cast<XEvent*>(&sXClientMessageEvent));
	XFlush(m_pDisplay);
}

/**
*  @brief
*    Handler for the XDnDDrop client message
*/
void Linux_XDnDFileDropHelper::handleXdndDrop(const XClientMessageEvent &sClientMessage)
{
	if (m_ToBeRequestedType == XLib::None) {
		// It's sending anyway, despite instructions to the contrary.
		// So reply that we're not interested.
		XClientMessageEvent sXClientMessageEvent;
		memset(&sXClientMessageEvent, 0, sizeof(sXClientMessageEvent));
		sXClientMessageEvent.type			= ClientMessage;
		sXClientMessageEvent.display		= sClientMessage.display;
		sXClientMessageEvent.window			= sClientMessage.data.l[0];
		sXClientMessageEvent.message_type	= XdndFinished;
		sXClientMessageEvent.format			= 32;
		sXClientMessageEvent.data.l[0]		= m_nDropWindow;
		sXClientMessageEvent.data.l[1]		= 0;
		sXClientMessageEvent.data.l[2]		= XLib::None;	// Failed
		XSendEvent(m_pDisplay, sClientMessage.data.l[0], False, NoEventMask, reinterpret_cast<XEvent*>(&sXClientMessageEvent));
	} else {
		m_XdndSourceWindow = sClientMessage.data.l[0];
		if (m_XdndVersion >= 1)
			XConvertSelection(m_pDisplay, XdndSelection, m_ToBeRequestedType, XdndSelection, m_nDropWindow, sClientMessage.data.l[2]);
		else
			XConvertSelection(m_pDisplay, XdndSelection, m_ToBeRequestedType, XdndSelection, m_nDropWindow, CurrentTime);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}
