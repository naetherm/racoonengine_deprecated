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
#include "RERenderer/Resource/CompositorNode/CompositorNodeResourceManager.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeResource.h"
#include "RERenderer/Resource/CompositorNode/Pass/CompositorPassFactory.h"
#include "RERenderer/Resource/CompositorNode/Loader/CompositorNodeResourceLoader.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResource.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/Core/Renderer/RenderTargetTextureManager.h"
#include "RERenderer/Core/Renderer/FramebufferManager.h"
#include "RERenderer/Core/Renderer/RenderPassManager.h"
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		static const RERenderer::CompositorPassFactory defaultCompositorPassFactory;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void CompositorNodeResourceManager::loadCompositorNodeResourceByAssetId(AssetId assetId, CompositorNodeResourceId& compositorNodeResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, compositorNodeResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void CompositorNodeResourceManager::setInvalidResourceId(CompositorNodeResourceId& compositorNodeResourceId, RECore::IResourceListener& resourceListener) const
	{
		CompositorNodeResource* compositorNodeResource = tryGetById(compositorNodeResourceId);
		if (nullptr != compositorNodeResource)
		{
			compositorNodeResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(compositorNodeResourceId);
	}

	void CompositorNodeResourceManager::setCompositorPassFactory(const ICompositorPassFactory* compositorPassFactory)
	{
		// There must always be a valid compositor pass factory instance
		mCompositorPassFactory = (nullptr != compositorPassFactory) ? compositorPassFactory : &::detail::defaultCompositorPassFactory;
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 CompositorNodeResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& CompositorNodeResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& CompositorNodeResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* CompositorNodeResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void CompositorNodeResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(naetherm) Experimental implementation (take care of resource cleanup etc.)
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			const CompositorNodeResource& compositorNodeResource = mInternalResourceManager->getResources().getElementByIndex(i);
			if (compositorNodeResource.getAssetId() == assetId)
			{
				CompositorNodeResourceId compositorNodeResourceId = RECore::getInvalid<CompositorNodeResourceId>();
				loadCompositorNodeResourceByAssetId(assetId, compositorNodeResourceId, nullptr, true, compositorNodeResource.getResourceLoaderTypeId());

				{ // Reload all compositor workspace resources using this compositor node resource
					CompositorWorkspaceResourceManager& compositorWorkspaceResourceManager = mRenderer.getCompositorWorkspaceResourceManager();
					const RECore::uint32 numberOfCompositorWorkspaceResources = compositorWorkspaceResourceManager.getNumberOfResources();
					for (RECore::uint32 compositorWorkspaceResourceIndex = 0; compositorWorkspaceResourceIndex < numberOfCompositorWorkspaceResources; ++compositorWorkspaceResourceIndex)
					{
						const CompositorWorkspaceResource& compositorWorkspaceResource = compositorWorkspaceResourceManager.getByIndex(compositorWorkspaceResourceIndex);
						const CompositorWorkspaceResource::CompositorNodeAssetIds& compositorNodeAssetIds = compositorWorkspaceResource.getCompositorNodeAssetIds();
						for (AssetId currentAssetId : compositorNodeAssetIds)
						{
							if (currentAssetId == assetId)
							{
								compositorWorkspaceResourceManager.reloadResourceByAssetId(compositorWorkspaceResource.getAssetId());
								break;
							}
						}
					}
				}

				break;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* CompositorNodeResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new CompositorNodeResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorNodeResourceManager::CompositorNodeResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mCompositorPassFactory(&::detail::defaultCompositorPassFactory),
		mRenderTargetTextureManager(new RenderTargetTextureManager(mRenderer)),
		mRenderPassManager(new RenderPassManager(mRenderer.getRhi())),
		mFramebufferManager(new FramebufferManager(*mRenderTargetTextureManager, *mRenderPassManager))
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<CompositorNodeResource, CompositorNodeResourceLoader, CompositorNodeResourceId, 32>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	CompositorNodeResourceManager::~CompositorNodeResourceManager()
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
