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
#include "RERenderer/Resource/Scene/SceneResourceManager.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/Scene/Factory/SceneFactory.h"
#include "RERenderer/Resource/Scene/Loader/SceneResourceLoader.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
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
		static const RERenderer::SceneFactory defaultSceneFactory;


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
	void SceneResourceManager::setSceneFactory(const ISceneFactory* sceneFactory)
	{
		// There must always be a valid scene factory instance
		mSceneFactory = (nullptr != sceneFactory) ? sceneFactory : &::detail::defaultSceneFactory;

		// Tell the scene resource instances about the new scene factory in town
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			mInternalResourceManager->getResources().getElementByIndex(i).mSceneFactory = mSceneFactory;
		}
	}

	SceneResource* SceneResourceManager::getSceneResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	SceneResourceId SceneResourceManager::getSceneResourceIdByAssetId(AssetId assetId) const
	{
		const SceneResource* sceneResource = getSceneResourceByAssetId(assetId);
		return (nullptr != sceneResource) ? sceneResource->getId() : RECore::getInvalid<SceneResourceId>();
	}

	void SceneResourceManager::loadSceneResourceByAssetId(AssetId assetId, SceneResourceId& sceneResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, sceneResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void SceneResourceManager::destroySceneResource(SceneResourceId sceneResourceId)
	{
		mInternalResourceManager->getResources().removeElement(sceneResourceId);
	}

	void SceneResourceManager::setInvalidResourceId(SceneResourceId& sceneResourceId, RECore::IResourceListener& resourceListener) const
	{
		SceneResource* sceneResource = tryGetById(sceneResourceId);
		if (nullptr != sceneResource)
		{
			sceneResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(sceneResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 SceneResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& SceneResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& SceneResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* SceneResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void SceneResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		SceneResource* sceneResource = mInternalResourceManager->getResourceByAssetId(assetId);
		if (nullptr != sceneResource)
		{
			sceneResource->destroyAllSceneNodesAndItems();
		}
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* SceneResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new SceneResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	SceneResourceManager::SceneResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mSceneFactory(&::detail::defaultSceneFactory)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<SceneResource, SceneResourceLoader, SceneResourceId, 16>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	SceneResourceManager::~SceneResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
