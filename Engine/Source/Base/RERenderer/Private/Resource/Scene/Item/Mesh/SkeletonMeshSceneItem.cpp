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
#include "RERenderer/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.h"
#include "RERenderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Mesh/MeshResource.h"
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SkeletonResourceId SkeletonMeshSceneItem::getSkeletonResourceId() const
	{
		const MeshResource* meshResource = getSceneResource().getRenderer().getMeshResourceManager().tryGetById(getMeshResourceId());
		return (nullptr != meshResource) ? meshResource->getSkeletonResourceId() : RECore::getInvalid<SkeletonResourceId>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	void SkeletonMeshSceneItem::deserialize(uint32_t numberOfBytes, const uint8_t* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1Scene::SkeletonMeshItem) <= numberOfBytes, "Invalid number of bytes")
		const v1Scene::SkeletonMeshItem* skeletonMeshItem = reinterpret_cast<const v1Scene::SkeletonMeshItem*>(data);

		// Read data
		mSkeletonAnimationAssetId = skeletonMeshItem->skeletonAnimationAssetId;

		// Call base implementation
		MeshSceneItem::deserialize(numberOfBytes - sizeof(v1Scene::SkeletonMeshItem), data + sizeof(v1Scene::SkeletonMeshItem));
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	SkeletonMeshSceneItem::~SkeletonMeshSceneItem()
	{
		// Destroy the skeleton animation controller instance, if needed
		delete mSkeletonAnimationController;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	void SkeletonMeshSceneItem::onLoadingStateChange(const RECore::IResource& resource)
	{
		// Create/destroy skeleton animation controller
		if (resource.getId() == getMeshResourceId())
		{
			if (resource.getLoadingState() == RECore::IResource::LoadingState::LOADED)
			{
				if (RECore::isValid(mSkeletonAnimationAssetId))
				{
					RHI_ASSERT(nullptr == mSkeletonAnimationController, "Invalid skeleton animation controller")
					mSkeletonAnimationController = new SkeletonAnimationController(getSceneResource().getRenderer(), static_cast<const MeshResource&>(resource).getSkeletonResourceId());
					mSkeletonAnimationController->startSkeletonAnimationByAssetId(mSkeletonAnimationAssetId);
				}
			}
			else if (nullptr != mSkeletonAnimationController)
			{
				delete mSkeletonAnimationController;
				mSkeletonAnimationController = nullptr;
			}
		}

		// Call the base implementation
		MeshSceneItem::onLoadingStateChange(resource);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
