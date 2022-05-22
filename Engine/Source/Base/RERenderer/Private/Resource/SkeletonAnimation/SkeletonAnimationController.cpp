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
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationEvaluator.h"
#include "RERenderer/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "RERenderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "RERenderer/Resource/Skeleton/SkeletonResource.h"
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
	void SkeletonAnimationController::startSkeletonAnimationByResourceId(SkeletonAnimationResourceId skeletonAnimationResourceId)
	{
		clear();
		mSkeletonAnimationResourceId = skeletonAnimationResourceId;
		if (RECore::isValid(skeletonAnimationResourceId))
		{
			mRenderer.getSkeletonAnimationResourceManager().getResourceByResourceId(skeletonAnimationResourceId).connectResourceListener(*this);
		}
	}

	void SkeletonAnimationController::startSkeletonAnimationByAssetId(AssetId skeletonAnimationAssetId)
	{
		clear();
		mRenderer.getSkeletonAnimationResourceManager().loadSkeletonAnimationResourceByAssetId(skeletonAnimationAssetId, mSkeletonAnimationResourceId, this);
	}

	void SkeletonAnimationController::clear()
	{
		if (RECore::isValid(mSkeletonAnimationResourceId))
		{
			disconnectFromResourceById(mSkeletonAnimationResourceId);
			RECore::setInvalid(mSkeletonAnimationResourceId);
		}
		destroySkeletonAnimationEvaluator();
		mTimeInSeconds = 0.0f;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::onLoadingStateChange(const RECore::IResource& resource)
	{
		if (resource.getLoadingState() == RECore::IResource::LoadingState::LOADED)
		{
			createSkeletonAnimationEvaluator();
		}
		else
		{
			destroySkeletonAnimationEvaluator();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::createSkeletonAnimationEvaluator()
	{
		RHI_ASSERT(nullptr == mSkeletonAnimationEvaluator, "No useless update calls, please")
		SkeletonAnimationResourceManager& skeletonAnimationResourceManager = mRenderer.getSkeletonAnimationResourceManager();
		mSkeletonAnimationEvaluator = new SkeletonAnimationEvaluator(skeletonAnimationResourceManager, mSkeletonAnimationResourceId);

		// Register skeleton animation controller
		skeletonAnimationResourceManager.mSkeletonAnimationControllers.push_back(this);
	}

	void SkeletonAnimationController::destroySkeletonAnimationEvaluator()
	{
		if (nullptr != mSkeletonAnimationEvaluator)
		{
			{ // Unregister skeleton animation controller
				SkeletonAnimationResourceManager::SkeletonAnimationControllers& skeletonAnimationControllers = mRenderer.getSkeletonAnimationResourceManager().mSkeletonAnimationControllers;
				SkeletonAnimationResourceManager::SkeletonAnimationControllers::iterator iterator = std::find(skeletonAnimationControllers.begin(), skeletonAnimationControllers.end(), this);
				RHI_ASSERT(iterator != skeletonAnimationControllers.end(), "Invalid skeleton animation controller")
				skeletonAnimationControllers.erase(iterator);
			}

			// Destroy skeleton animation evaluator
			delete mSkeletonAnimationEvaluator;
			mSkeletonAnimationEvaluator = nullptr;
		}
	}

	void SkeletonAnimationController::update(float pastSecondsSinceLastFrame)
	{
		// Sanity check
		RHI_ASSERT(pastSecondsSinceLastFrame > 0.0f, "No negative time, please")
		RHI_ASSERT(nullptr != mSkeletonAnimationEvaluator, "No useless update calls, please")

		// Advance time and evaluate state
		mTimeInSeconds += pastSecondsSinceLastFrame;
		mSkeletonAnimationEvaluator->evaluate(mTimeInSeconds);

		{ // Tell the controlled skeleton resource about the new state
			SkeletonResource& skeletonResource = mRenderer.getSkeletonResourceManager().getById(mSkeletonResourceId);
			const SkeletonAnimationEvaluator::BoneIds& boneIds = mSkeletonAnimationEvaluator->getBoneIds();
			const SkeletonAnimationEvaluator::TransformMatrices& transformMatrices = mSkeletonAnimationEvaluator->getTransformMatrices();
			glm::mat4* localBoneMatrices = skeletonResource.getLocalBoneMatrices();
			for (size_t i = 0; i < boneIds.size(); ++i)
			{
				const RECore::uint32 boneIndex = skeletonResource.getBoneIndexByBoneId(boneIds[i]);
				if (RECore::isValid(boneIndex))
				{
					localBoneMatrices[boneIndex] = transformMatrices[i];
				}
			}
			skeletonResource.localToGlobalPose();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
