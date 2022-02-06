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
#include "RERenderer/Resource/VertexAttributes/VertexAttributesResourceManager.h"
#include "RERenderer/Resource/VertexAttributes/VertexAttributesResource.h"
#include "RERenderer/Resource/VertexAttributes/Loader/VertexAttributesResourceLoader.h"
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
	VertexAttributesResource* VertexAttributesResourceManager::getVertexAttributesResourceByAssetId(RECore::AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void VertexAttributesResourceManager::loadVertexAttributesResourceByAssetId(RECore::AssetId assetId, VertexAttributesResourceId& vertexAttributesResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, vertexAttributesResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	VertexAttributesResourceId VertexAttributesResourceManager::createVertexAttributesResourceByAssetId(RECore::AssetId assetId)
	{
		VertexAttributesResource& vertexAttributesResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(vertexAttributesResource, RECore::IResource::LoadingState::LOADED);
		return vertexAttributesResource.getId();
	}

	void VertexAttributesResourceManager::setInvalidResourceId(VertexAttributesResourceId& vertexAttributesResourceId, RECore::IResourceListener& resourceListener) const
	{
		VertexAttributesResource* vertexAttributesResource = tryGetById(vertexAttributesResourceId);
		if (nullptr != vertexAttributesResource)
		{
			vertexAttributesResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(vertexAttributesResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t VertexAttributesResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& VertexAttributesResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& VertexAttributesResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* VertexAttributesResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void VertexAttributesResourceManager::reloadResourceByAssetId(RECore::AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void VertexAttributesResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* VertexAttributesResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new VertexAttributesResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	VertexAttributesResourceManager::VertexAttributesResourceManager(IRenderer& renderer)
  : mRenderer(renderer)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<VertexAttributesResource, VertexAttributesResourceLoader, VertexAttributesResourceId, 32>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	VertexAttributesResourceManager::~VertexAttributesResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
