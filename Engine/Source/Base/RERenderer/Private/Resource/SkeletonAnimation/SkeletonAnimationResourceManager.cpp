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
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationResource.h"
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "RERenderer/Resource/SkeletonAnimation/Loader/SkeletonAnimationResourceLoader.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/IRenderer.h"
#include <RECore/Time/TimeManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonAnimationResource* SkeletonAnimationResourceManager::getSkeletonAnimationResourceByAssetId(RECore::AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void SkeletonAnimationResourceManager::loadSkeletonAnimationResourceByAssetId(RECore::AssetId assetId, SkeletonAnimationResourceId& skeletonAnimationResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, skeletonAnimationResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	SkeletonAnimationResourceId SkeletonAnimationResourceManager::createSkeletonAnimationResourceByAssetId(RECore::AssetId assetId)
	{
		SkeletonAnimationResource& skeletonAnimationResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(skeletonAnimationResource, RECore::IResource::LoadingState::LOADED);
		return skeletonAnimationResource.getId();
	}

	void SkeletonAnimationResourceManager::setInvalidResourceId(SkeletonAnimationResourceId& skeletonAnimationResourceId, RECore::IResourceListener& resourceListener) const
	{
		SkeletonAnimationResource* skeletonAnimationResource = tryGetById(skeletonAnimationResourceId);
		if (nullptr != skeletonAnimationResource)
		{
			skeletonAnimationResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(skeletonAnimationResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 SkeletonAnimationResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& SkeletonAnimationResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& SkeletonAnimationResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* SkeletonAnimationResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void SkeletonAnimationResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void SkeletonAnimationResourceManager::update()
	{
		// Update skeleton animation controllers
		const float pastSecondsSinceLastFrame = mRenderer.getTimeManager().getPastSecondsSinceLastFrame();
		for (SkeletonAnimationController* skeletonAnimationController : mSkeletonAnimationControllers)
		{
			skeletonAnimationController->update(pastSecondsSinceLastFrame);
		}
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* SkeletonAnimationResourceManager::createResourceLoaderInstance([[maybe_unused]] RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// We only support our own format
		RHI_ASSERT(mRenderer.getContext(), resourceLoaderTypeId == SkeletonAnimationResourceLoader::TYPE_ID, "Invalid resource loader type ID")
		#ifdef DEBUG
			return new SkeletonAnimationResourceLoader(mInternalResourceManager->getResourceManager(), mRenderer);
		#else
			return new SkeletonAnimationResourceLoader(mInternalResourceManager->getResourceManager());
		#endif
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	SkeletonAnimationResourceManager::SkeletonAnimationResourceManager(IRenderer& renderer)
  : mRenderer(renderer)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<SkeletonAnimationResource, SkeletonAnimationResourceLoader, SkeletonAnimationResourceId, 2048>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	SkeletonAnimationResourceManager::~SkeletonAnimationResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
