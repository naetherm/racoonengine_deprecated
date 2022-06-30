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
#include "RECore/Log/Log.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Frontend/Frontend.h"
#include "RECore/Frontend/FrontendContext.h"
#include "RECore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FrontendImpl, "RECore", RECore::Object, "Abstract frontend implementation base class")
re_class_metadata_end(FrontendImpl)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendImpl::FrontendImpl() :
	m_pFrontend(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FrontendImpl::~FrontendImpl()
{
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
void FrontendImpl::onCreate()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onCreate();
}

void FrontendImpl::onRestart()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onRestart();
}

bool FrontendImpl::onStart()
{
	// Call virtual function from frontend
	return m_pFrontend ? m_pFrontend->onStart() : true;
}

void FrontendImpl::onResume()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onResume();
}

void FrontendImpl::onPause()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onPause();
}

void FrontendImpl::onStop()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onStop();
}

void FrontendImpl::onDestroy()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onDestroy();
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
void FrontendImpl::onSize()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onSize();
}

void FrontendImpl::onFullscreenMode()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onFullscreenMode();
}

void FrontendImpl::onDraw()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onDraw();
}

void FrontendImpl::onUpdate()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onUpdate();
}

void FrontendImpl::onDrop(const std::vector<String> &lstFiles)
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onDrop(lstFiles);
}


//[-------------------------------------------------------]
//[ Protected virtual FrontendImpl functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the frontend is run
*/
void FrontendImpl::onRun(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->onRun(sExecutableFilename, lstArguments);
}

/**
*  @brief
*    Called when the frontend should run
*/
int FrontendImpl::run(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
	// Nothing to do in here
	return 0;
}


//[-------------------------------------------------------]
//[ Protected static functions                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a frontend instance
*/
Frontend *FrontendImpl::createFrontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl)
{
	// Get the frontend RTTI class
	const Class *pClass = ClassManager::instance().getClass(cFrontendContext.getFrontend());
	if (pClass && pClass->isDerivedFrom("RECore::Frontend")) {
		// Create the frontend RTTI class instance
		Object *pObject = nullptr;
		if (cFrontendContext.getFrontendConstructor().length())
			pObject = pClass->create(cFrontendContext.getFrontendConstructor(), "FrontendContext=\"" + Type<const FrontendContext&>::convertToString(cFrontendContext) + "\" FrontendImpl=\"" + Type<FrontendImpl&>::convertToString(cFrontendImpl) + "\" " + cFrontendContext.getFrontendConstructorParameters());
		else
			pObject = pClass->create(Params<Object*, const FrontendContext&, FrontendImpl&>(cFrontendContext, cFrontendImpl));

		// Do we now have an instance?
		if (pObject) {
			// Cast the pointer to a frontend pointer
			Frontend *pFrontend = static_cast<Frontend*>(pObject);

			// Write down a log message
			RE_LOG(Info, "Using frontend '" + pClass->getClassName() + "': " + pClass->getDescription())

			// Set parameters for the instanced frontend RTTI class
			if (cFrontendContext.getFrontendParameters().length())
				pObject->setValues(cFrontendContext.getFrontendParameters());

			// Done
			return pFrontend;
		} else {
			// Error!
			RE_LOG(Error, "Failed instancing frontend '" + pClass->getClassName() + "': " + pClass->getDescription())
		}
	} else {
		// Error!
		RE_LOG(Error, "Frontend '" + cFrontendContext.getFrontend() + "' is no valid frontend RTTI class")
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Correct frontend position and size settings
*/
void FrontendImpl::correctPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight, int nScreenLeft, int nScreenTop, int nScreenWidth, int nScreenHeight, uint32 nMinWidth, uint32 nMinHeight)
{
	// The frontend position shouldn't leave the visible screen
	if (nX < nScreenLeft)
		nX = nScreenLeft;
	if (nY < nScreenTop)
		nY = nScreenTop;

	// The frontend position shouldn't be outside the visible screen
	if (nX > nScreenWidth-static_cast<int>(nMinHeight))
		nX = nScreenWidth-nMinHeight;
	if (nY > static_cast<int>(nScreenHeight-nMinHeight))
		nY = nScreenHeight-nMinHeight;

	// The width of the frontend shouldn't leave the visible screen
	int nDelta = (nX + nWidth) - nScreenWidth;
	if (nDelta > 0)
		nWidth = (nWidth-nDelta > 0) ? (nWidth-nDelta) : nMinHeight;

	// The height of the frontend shouldn't leave the visible screen
	nDelta = (nY + nHeight) - nScreenHeight;
	if (nDelta > 0)
		nHeight = (nHeight-nDelta > 0) ? (nHeight-nDelta) : nMinHeight;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
