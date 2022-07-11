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
#include "RERenderer/RenderQueue/RenderableManager.h"
#include <RECore/Math/Transform.h>
#include <RECore/Utility/GetInvalid.h>


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
		const RECore::Transform IdentityTransform;


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
	RenderableManager::RenderableManager() :
		#ifdef DEBUG
			mDebugName{},
		#endif
		mNumberOfLods(1),
		mTransform(&::detail::IdentityTransform),
		mVisible(true),
		mCachedDistanceToCamera(RECore::getInvalid<float>()),
		mMinimumRenderQueueIndex(0),
		mMaximumRenderQueueIndex(0),
		mCastShadows(false)
	{
		// Nothing here
	}

	void RenderableManager::setTransform(const RECore::Transform* transform)
	{
		mTransform = (nullptr != transform) ? transform : &::detail::IdentityTransform;
	}

	void RenderableManager::updateCachedRenderablesData()
	{
		if (mRenderables.empty())
		{
			mMinimumRenderQueueIndex = 0;
			mMaximumRenderQueueIndex = 0;
			mCastShadows			 = false;
		}
		else
		{
			{ // Initialize the data using the first renderable
				const Renderable& renderable = mRenderables[0];
				mMinimumRenderQueueIndex = renderable.getRenderQueueIndex();
				mMaximumRenderQueueIndex = mMinimumRenderQueueIndex;
				mCastShadows			 = renderable.getCastShadows();
			}

			// Now incorporate the data from the other renderables
			const size_t numberOfRenderables = mRenderables.size();
			for (size_t i = 1; i < numberOfRenderables; ++i)
			{
				const Renderable& renderable = mRenderables[i];
				const RECore::uint8 renderQueueIndex = renderable.getRenderQueueIndex();
				if (mMinimumRenderQueueIndex > renderQueueIndex)
				{
					mMinimumRenderQueueIndex = renderQueueIndex;
				}
				else if (mMaximumRenderQueueIndex < renderQueueIndex)
				{
					mMaximumRenderQueueIndex = renderQueueIndex;
				}
				if (renderable.getCastShadows())
				{
					mCastShadows = true;
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
