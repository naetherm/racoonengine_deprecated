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
#include "RERenderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "RERenderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include <RECore/Math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CompositorContextData::CompositorContextData(const CompositorWorkspaceInstance* compositorWorkspaceInstance, const CameraSceneItem* cameraSceneItem, bool singlePassStereoInstancing, const LightSceneItem* lightSceneItem, const CompositorInstancePassShadowMap* compositorInstancePassShadowMap) :
		mCompositorWorkspaceInstance(compositorWorkspaceInstance),
		mCameraSceneItem(cameraSceneItem),
		mSinglePassStereoInstancing(singlePassStereoInstancing),
		mLightSceneItem(lightSceneItem),
		mCompositorInstancePassShadowMap(compositorInstancePassShadowMap),
		mWorldSpaceCameraPosition((nullptr != cameraSceneItem) ? cameraSceneItem->getWorldSpaceCameraPosition() : RECore::Math::DVEC3_ZERO),
		mCurrentlyBoundMaterialBlueprintResource(nullptr),
		mGlobalComputeSize{0, 0, 0}
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
