/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResource.h"
#include "RERenderer/Resource/CompositorWorkspace/Loader/CompositorWorkspaceResourceLoader.h"
#include "RERenderer/Core/Renderer/RenderPassManager.h"
#include "RERenderer/Core/Renderer/FramebufferManager.h"
#include "RERenderer/Core/Renderer/RenderTargetTextureManager.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void CompositorWorkspaceResourceManager::loadCompositorWorkspaceResourceByAssetId(AssetId assetId, CompositorWorkspaceResourceId& compositorWorkspaceResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, compositorWorkspaceResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void CompositorWorkspaceResourceManager::setInvalidResourceId(CompositorWorkspaceResourceId& compositorWorkspaceResourceId, RECore::IResourceListener& resourceListener) const
	{
		CompositorWorkspaceResource* compositorWorkspaceResource = tryGetById(compositorWorkspaceResourceId);
		if (nullptr != compositorWorkspaceResource)
		{
			compositorWorkspaceResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(compositorWorkspaceResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 CompositorWorkspaceResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& CompositorWorkspaceResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& CompositorWorkspaceResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* CompositorWorkspaceResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void CompositorWorkspaceResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* CompositorWorkspaceResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new CompositorWorkspaceResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorWorkspaceResourceManager::CompositorWorkspaceResourceManager(IRenderer& renderer) :
    mRenderer(renderer),
		mRenderTargetTextureManager(new RenderTargetTextureManager(renderer)),
		mRenderPassManager(new RenderPassManager(renderer.getRhi())),
		mFramebufferManager(new FramebufferManager(*mRenderTargetTextureManager, *mRenderPassManager))
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<CompositorWorkspaceResource, CompositorWorkspaceResourceLoader, CompositorWorkspaceResourceId, 32>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	CompositorWorkspaceResourceManager::~CompositorWorkspaceResourceManager()
	{
		delete mFramebufferManager;
		delete mRenderPassManager;
		delete mRenderTargetTextureManager;
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
