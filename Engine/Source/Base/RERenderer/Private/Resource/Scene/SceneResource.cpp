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
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include "RERenderer/Resource/Scene/SceneResourceManager.h"
#include "RERenderer/Resource/Scene/Item/ISceneItem.h"
#include "RERenderer/Resource/Scene/Factory/ISceneFactory.h"
#include "RERenderer/Resource/Scene/Culling/SceneCullingManager.h"
#include "RERenderer/IRenderer.h"
#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	IRenderer& SceneResource::getRenderer() const
	{
		return getResourceManager<SceneResourceManager>().getRenderer();
	}

	void SceneResource::destroyAllSceneNodesAndItems()
	{
		destroyAllSceneNodes();
		destroyAllSceneItems();

		// TODO(naetherm) The following is just for culling kickoff and won't stay this way
		// Re-create scene culling manager
		delete mSceneCullingManager;
		mSceneCullingManager = new SceneCullingManager();

		// Update the resource loading state
		setLoadingState(IResource::LoadingState::UNLOADED);
	}

	SceneNode* SceneResource::createSceneNode(const RECore::Transform& transform)
	{
		SceneNode* sceneNode = new SceneNode(transform);
		mSceneNodes.push_back(sceneNode);
		return sceneNode;
	}

	void SceneResource::destroySceneNode(SceneNode& sceneNode)
	{
		SceneNodes::iterator iterator = std::find(mSceneNodes.begin(), mSceneNodes.end(), &sceneNode);
		if (iterator != mSceneNodes.end())
		{
			mSceneNodes.erase(iterator);
			delete &sceneNode;
		}
		else
		{
			// TODO(naetherm) Error handling
		}
	}

	void SceneResource::destroyAllSceneNodes()
	{
		const size_t numberOfSceneNodes = mSceneNodes.size();
		for (size_t i = 0; i < numberOfSceneNodes; ++i)
		{
			delete mSceneNodes[i];
		}
		mSceneNodes.clear();
	}

	ISceneItem* SceneResource::createSceneItem(SceneItemTypeId sceneItemTypeId, SceneNode& sceneNode)
	{
		if (nullptr == mSceneFactory)
		{
			mSceneFactory = &getResourceManager<SceneResourceManager>().getSceneFactory();
		}
		ASSERT(nullptr != mSceneFactory, "Invalid scene factory")
		ISceneItem* sceneItem = mSceneFactory->createSceneItem(sceneItemTypeId, *this);
		if (nullptr != sceneItem)
		{
			sceneNode.attachSceneItem(*sceneItem);
			mSceneItems.push_back(sceneItem);
		}
		else
		{
			// TODO(naetherm) Error handling
		}
		return sceneItem;
	}

	void SceneResource::destroySceneItem(ISceneItem& sceneItem)
	{
		SceneItems::iterator iterator = std::find(mSceneItems.begin(), mSceneItems.end(), &sceneItem);
		if (iterator != mSceneItems.end())
		{
			mSceneItems.erase(iterator);
			delete &sceneItem;
		}
		else
		{
			// TODO(naetherm) Error handling
		}
	}

	void SceneResource::destroyAllSceneItems()
	{
		const size_t numberOfSceneItems = mSceneItems.size();
		for (size_t i = 0; i < numberOfSceneItems; ++i)
		{
			delete mSceneItems[i];
		}
		mSceneItems.clear();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SceneResource::initializeElement(SceneResourceId sceneResourceId)
	{
		// Sanity checks
		ASSERT(nullptr == mSceneFactory, "Invalid scene factory")
		ASSERT(nullptr == mSceneCullingManager, "Invalid scene culling manager")
		ASSERT(mSceneNodes.empty(), "Invalid scene nodes")
		ASSERT(mSceneItems.empty(), "Invalid scene items")

		// Create scene culling manager
		mSceneCullingManager = new SceneCullingManager();

		// Call base implementation
		IResource::initializeElement(sceneResourceId);
	}

	void SceneResource::deinitializeElement()
	{
		// Reset everything
		destroyAllSceneNodesAndItems();
		mSceneFactory = nullptr;
		delete mSceneCullingManager;
		mSceneCullingManager = nullptr;

		// Call base implementation
		IResource::deinitializeElement();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
