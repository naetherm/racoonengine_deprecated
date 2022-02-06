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
#include "RERendererToolkit/Project/ProjectAssetMonitor.h"
#include "RERendererToolkit/Project/ProjectImpl.h"
#include "RERendererToolkit/IRendererToolkit.h"
#include "RERendererToolkit/Context.h"

#include <RERenderer/IRenderer.h>
#include <RERenderer/Core/Platform/PlatformManager.h>
#include <RECore/Log/Log.h>

//#include <SimpleFileWatcher/FileWatcher.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{


	namespace detail
	{

	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ProjectAssetMonitor::ProjectAssetMonitor(ProjectImpl& projectImpl, RERenderer::IRenderer& renderer, const std::string& rhiTarget) :
		mProjectImpl(projectImpl),
		mRenderer(renderer),
		mRhiTarget(rhiTarget),
		mShutdownThread(false)
	{
		mThread = std::thread(&ProjectAssetMonitor::threadWorker, this);
	}

	ProjectAssetMonitor::~ProjectAssetMonitor()
	{
		mShutdownThread = true;
		if (mProjectImpl.getRendererToolkit().getState() != IRendererToolkit::State::IDLE)
		{
			RE_LOG(Info, "Shutdown of project asset monitor while still in busy state, the thread join might take a moment, please wait")
		}
		mThread.join();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ProjectAssetMonitor::threadWorker()
	{
		RENDERER_SET_CURRENT_THREAD_DEBUG_NAME("Asset monitor", "Renderer toolkit: Project asset monitor")

		// Create the file watcher object
		//FW::FileWatcher fileWatcher;
		//detail::FileWatchListener fileWatchListener(*this);
		//fileWatcher.addWatch(mProjectImpl.getAbsoluteProjectDirectory(), &fileWatchListener, true);

		// On startup we need to check for changes which were done while the project asset monitor wasn't running
		try
		{
			mProjectImpl.compileAllAssets(mRhiTarget.c_str());
		}
		catch (const std::exception& e)
		{
			RE_LOG(Critical, std::string("Project compilation failed: ") + e.what())
			mProjectImpl.onCompilationRunFinished();
		}

		// Update the file watcher object as long as the project asset monitor is up-and-running
		while (!mShutdownThread)
		{
			//fileWatcher.update();
			//fileWatchListener.processFileActions();

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
