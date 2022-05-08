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
#include "RERenderer/Resource/CompositorNode/Pass/ShadowMap/CompositorInstancePassShadowMap.h"
#include "RERenderer/Resource/CompositorNode/Pass/Compute/CompositorInstancePassCompute.h"
#include "RERenderer/Resource/CompositorNode/Pass/Compute/CompositorResourcePassCompute.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include "RERenderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "RERenderer/Resource/Scene/Item/Light/LightSceneItem.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include "RERenderer/RenderQueue/RenderableManager.h"
#include <RECore/Math/Math.h>
#include "RERenderer/Core/IProfiler.h"
#include "RERenderer/IRenderer.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/gtc/matrix_transform.hpp>
PRAGMA_WARNING_POP

#include <algorithm>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr float	  SHADOW_MAP_FILTER_SIZE							  = 7.0f;
		static constexpr uint8_t  INTERMEDIATE_CASCADE_INDEX						  = 3;
		static constexpr uint32_t DEPTH_SHADOW_MAP_TEXTURE_ASSET_ID					  = ASSET_ID("RacoonEngine/Texture/DynamicByCode/DepthShadowMap");
		static constexpr uint32_t INTERMEDIATE_DEPTH_BLUR_SHADOW_MAP_TEXTURE_ASSET_ID = ASSET_ID("RacoonEngine/Texture/DynamicByCode/IntermediateDepthBlurShadowMap");


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] glm::vec4 transformVectorByMatrix(const glm::mat4& matrix, const glm::vec4& vector)
		{
			const glm::vec4 temporaryVector = matrix * vector;
			return temporaryVector / temporaryVector.w;
		}


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
	void CompositorInstancePassShadowMap::setNumberOfShadowCascades(uint8_t numberOfShadowCascades)
	{
		if (mNumberOfShadowCascades != numberOfShadowCascades)
		{
			RHI_ASSERT(numberOfShadowCascades <= CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES, "Invalid number of shadow cascades")
			mNumberOfShadowCascades = numberOfShadowCascades;
			++mSettingsGenerationCounter;
		}
	}

	void CompositorInstancePassShadowMap::setNumberOfShadowMultisamples(uint8_t numberOfShadowMultisamples)
	{
		if (mNumberOfShadowMultisamples != numberOfShadowMultisamples)
		{
			RHI_ASSERT(numberOfShadowMultisamples >= 1, "Invalid number of shadow multisamples")
			RHI_ASSERT(numberOfShadowMultisamples <= getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getRhi().getCapabilities().maximumNumberOfMultisamples, "Invalid number of shadow multisamples")
			mNumberOfShadowMultisamples = numberOfShadowMultisamples;
			++mSettingsGenerationCounter;
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassShadowMap::onFillCommandBuffer([[maybe_unused]] const RERHI::RHIRenderTarget* renderTarget, const CompositorContextData& compositorContextData, RERHI::RHICommandBuffer& commandBuffer)
	{
		const CompositorWorkspaceInstance& compositorWorkspaceInstance = getCompositorNodeInstance().getCompositorWorkspaceInstance();
		const IRenderer& renderer = compositorWorkspaceInstance.getRenderer();

		// Sanity check
		RHI_ASSERT(nullptr == renderTarget, "The shadow map compositor instance pass needs an invalid render target")

		// Settings update handling
		if (mUsedSettingsGenerationCounter != mSettingsGenerationCounter)
		{
			mUsedSettingsGenerationCounter = mSettingsGenerationCounter;
			destroyShadowMapRenderTarget();
			createShadowMapRenderTarget();
		}

		// Early escape if shadow is disabled
		if (!mEnabled)
		{
			return;
		}

		// Fill command buffer
		const CameraSceneItem* cameraSceneItem = compositorContextData.getCameraSceneItem();
		const LightSceneItem* lightSceneItem = compositorContextData.getLightSceneItem();
		if (nullptr != mDepthFramebufferPtr && nullptr != cameraSceneItem && cameraSceneItem->getParentSceneNode() && nullptr != lightSceneItem && lightSceneItem->getParentSceneNode())
		{
			const glm::vec3 worldSpaceSunlightDirection = lightSceneItem->getParentSceneNode()->getGlobalTransform().rotation * RECore::Math::VEC3_FORWARD;
			mPassData.shadowMapSize = static_cast<int>(mShadowMapSize);

			// TODO(naetherm) Stabilize cascades Reversed-Z update
			const bool stabilizeCascades = false;
//			const bool stabilizeCascades = mStabilizeCascades;

			// TODO(naetherm) The minimum and maximum distance need to be calculated dynamically via depth buffer reduction as seen inside e.g. https://github.com/TheRealMJP/MSAAFilter/tree/master/MSAAFilter
			const float minimumDistance = 0.0f;
			const float maximumDistance = 0.5f;

			// Compute the split distances based on the partitioning mode
			float cascadeSplits[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			{
				const float nearClip = cameraSceneItem->getNearZ();
				const float farClip = cameraSceneItem->getFarZ();
				const float clipRange = farClip - nearClip;
				const float minimumZ = nearClip + minimumDistance * clipRange;
				const float maximumZ = nearClip + maximumDistance * clipRange;
				const float range = maximumZ - minimumZ;
				const float ratio = maximumZ / minimumZ;

				for (uint8_t cascadeIndex = 0; cascadeIndex < mNumberOfShadowCascades; ++cascadeIndex)
				{
					const float p = static_cast<float>(cascadeIndex + 1) / static_cast<float>(mNumberOfShadowCascades);
					const float log = minimumZ * std::pow(ratio, p);
					const float uniform = minimumZ + range * p;
					const float d = mCascadeSplitsLambda * (log - uniform) + uniform;
					cascadeSplits[cascadeIndex] = (d - nearClip) / clipRange;
				}
			}

			// Coordinate system related adjustments
			// -> Vulkan and Direct3D: Left-handed coordinate system with clip space depth value range 0..1
			// -> OpenGL without "GL_ARB_clip_control"-extension: Right-handed coordinate system with clip space depth value range -1..1
			const float nearZ = renderer.getRhi().getCapabilities().zeroToOneClipZ ? 0.0f : -1.0f;

			// Get the 8 points of the view frustum in world space
			glm::vec4 worldSpaceFrustumCorners[8] =
			{
				// Near
				{-1.0f,  1.0f, nearZ, 1.0f},	// 0: Near top left
				{ 1.0f,  1.0f, nearZ, 1.0f},	// 1: Near top right
				{-1.0f, -1.0f, nearZ, 1.0f},	// 2: Near bottom left
				{ 1.0f, -1.0f, nearZ, 1.0f},	// 3: Near bottom right
				// Far
				{-1.0f,  1.0f, 1.0f, 1.0f},		// 4: Far top left
				{ 1.0f,  1.0f, 1.0f, 1.0f},		// 5: Far top right
				{-1.0f, -1.0f, 1.0f, 1.0f},		// 6: Far bottom left
				{ 1.0f, -1.0f, 1.0f, 1.0f}		// 7: Far bottom right
			};
			{
				uint32_t renderTargetWidth = 0;
				uint32_t renderTargetHeight = 0;
				RHI_ASSERT(nullptr != compositorWorkspaceInstance.getExecutionRenderTarget(), "Invalid compositor workspace instance execution render target")
				compositorWorkspaceInstance.getExecutionRenderTarget()->getWidthAndHeight(renderTargetWidth, renderTargetHeight);
				if (compositorContextData.getSinglePassStereoInstancing())
				{
					renderTargetWidth /= 2;
				}
				const glm::mat4 worldSpaceToClipSpaceMatrix = cameraSceneItem->getViewSpaceToClipSpaceMatrix(static_cast<float>(renderTargetWidth) / static_cast<float>(renderTargetHeight)) * cameraSceneItem->getCameraRelativeWorldSpaceToViewSpaceMatrix();
				const glm::mat4 clipSpaceToWorldSpaceMatrix = glm::inverse(worldSpaceToClipSpaceMatrix);
				for (int i = 0; i < 8; ++i)
				{
					worldSpaceFrustumCorners[i] = ::detail::transformVectorByMatrix(clipSpaceToWorldSpaceMatrix, worldSpaceFrustumCorners[i]);
				}
			}

			// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
			RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(renderer.getContext(), commandBuffer, static_cast<const CompositorResourcePassShadowMap&>(getCompositorResourcePass()).getDebugName())

			// Render the meshes to each cascade
			// -> Shadows should never be rendered via single pass stereo instancing
			const CompositorContextData shadowCompositorContextData(compositorContextData.getCompositorWorkspaceInstance(), compositorContextData.getCameraSceneItem(), false, compositorContextData.getLightSceneItem(), compositorContextData.getCompositorInstancePassShadowMap());
			for (uint8_t cascadeIndex = 0; cascadeIndex < mNumberOfShadowCascades; ++cascadeIndex)
			{
				// Scoped debug event
				RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(renderer.getContext(), commandBuffer, ("Shadow cascade " + std::to_string(cascadeIndex)).c_str())

				// Compute the MVP matrix from the light's point of view
				glm::mat4 depthProjectionMatrix;
				glm::mat4 depthViewMatrix;
				glm::vec3 minimumExtents;
				glm::vec3 maximumExtents;
				glm::vec3 cascadeExtents;
				const float splitDistance = cascadeSplits[cascadeIndex];
				{
					const float previousSplitDistance = (0 == cascadeIndex) ? minimumDistance : cascadeSplits[cascadeIndex - 1];

					// Get the corners of the current cascade slice of the view frustum
					glm::vec4 cascadeSliceWorldSpaceFrustumCorners[8];
					for (int i = 0; i < 4; ++i)
					{
						const glm::vec4 cornerRay = worldSpaceFrustumCorners[i + 4] - worldSpaceFrustumCorners[i];
						const glm::vec4 nearCornerRay = cornerRay * previousSplitDistance;
						const glm::vec4 farCornerRay = cornerRay * splitDistance;
						cascadeSliceWorldSpaceFrustumCorners[i + 4] = worldSpaceFrustumCorners[i] + farCornerRay;
						cascadeSliceWorldSpaceFrustumCorners[i] = worldSpaceFrustumCorners[i] + nearCornerRay;
					}

					// Calculate the centroid of the view frustum slice
					glm::vec4 temporaryFrustumCenter = RECore::Math::VEC4_ZERO;
					for (int i = 0; i < 8; ++i)
					{
						temporaryFrustumCenter += cascadeSliceWorldSpaceFrustumCorners[i];
					}
					const glm::vec3 frustumCenter = temporaryFrustumCenter / 8.0f;

					// Pick the right vector to use for the light camera, this needs to be constant for it to be stable
					const glm::vec3 rightDirection = stabilizeCascades ? RECore::Math::VEC3_RIGHT : (cameraSceneItem->getParentSceneNodeSafe().getTransform().rotation * RECore::Math::VEC3_RIGHT);

					// Calculate the minimum and maximum extents
					if (stabilizeCascades)
					{
						// Calculate the radius of a bounding sphere surrounding the frustum corners
						float sphereRadius = 0.0f;
						for (int i = 0; i < 8; ++i)
						{
							const float distance = glm::distance(glm::vec3(cascadeSliceWorldSpaceFrustumCorners[i]), frustumCenter);
							sphereRadius = std::max(sphereRadius, distance);
						}
						sphereRadius = std::ceil(sphereRadius * 16.0f) / 16.0f;
						maximumExtents = glm::vec3(sphereRadius, sphereRadius, sphereRadius);
						minimumExtents = -maximumExtents;
					}
					else
					{
						// Create a temporary view matrix for the light
						const glm::vec3& lightCameraPosition = frustumCenter;
						const glm::vec3 lightCameraTarget = frustumCenter - worldSpaceSunlightDirection;
						const glm::mat4 lightView = glm::lookAt(lightCameraPosition, lightCameraTarget, rightDirection);

						// Calculate an AABB around the frustum corners
						glm::vec4 mins(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
						glm::vec4 maxes(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
						for (int i = 0; i < 8; ++i)
						{
							const glm::vec4 corner = ::detail::transformVectorByMatrix(lightView, cascadeSliceWorldSpaceFrustumCorners[i]);
							mins = glm::min(mins, corner);
							maxes = glm::max(maxes, corner);
						}
						minimumExtents = mins;
						maximumExtents = maxes;

						// Adjust the minimum/maximum to accommodate the filtering size
						const float scale = (static_cast<float>(mShadowMapSize) + ::detail::SHADOW_MAP_FILTER_SIZE) / static_cast<float>(mShadowMapSize);
						minimumExtents.x *= scale;
						minimumExtents.y *= scale;
						maximumExtents.x *= scale;
						maximumExtents.x *= scale;
					}
					cascadeExtents = maximumExtents - minimumExtents;

					// Get position of the shadow camera
					const glm::vec3 shadowCameraPosition = frustumCenter + worldSpaceSunlightDirection * -minimumExtents.z;

					// Come up with a new orthographic camera for the shadow caster
					depthProjectionMatrix = glm::ortho(minimumExtents.x, maximumExtents.x, minimumExtents.y, maximumExtents.y, 0.0f, cascadeExtents.z);
					depthViewMatrix = glm::lookAt(shadowCameraPosition, frustumCenter, rightDirection);
				}

				// Create the rounding matrix, by projecting the world-space origin and determining the fractional offset in texel space
				glm::mat4 viewSpaceToClipSpace = depthProjectionMatrix * depthViewMatrix;
				if (stabilizeCascades)
				{
					glm::vec4 shadowOrigin(0.0f, 0.0f, 0.0f, 1.0f);
					shadowOrigin = ::detail::transformVectorByMatrix(viewSpaceToClipSpace, shadowOrigin);
					shadowOrigin *= static_cast<float>(mShadowMapSize) * 0.5f;

					const glm::vec4 roundedOrigin = glm::round(shadowOrigin);
					glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
					roundOffset *= 2.0f / static_cast<float>(mShadowMapSize);
					roundOffset.z = 0.0f;
					roundOffset.w = 0.0f;

					depthProjectionMatrix[3] += roundOffset;
					viewSpaceToClipSpace = depthProjectionMatrix * depthViewMatrix;
				}

				// Set custom camera matrices
				const_cast<CameraSceneItem*>(cameraSceneItem)->setCustomWorldSpaceToViewSpaceMatrix(depthViewMatrix);
				const_cast<CameraSceneItem*>(cameraSceneItem)->setCustomViewSpaceToClipSpaceMatrix(depthProjectionMatrix, glm::ortho(minimumExtents.x, maximumExtents.x, minimumExtents.y, maximumExtents.y, cascadeExtents.z, 0.0f));

				{ // Render shadow casters
					// Scoped debug event
					RENDERER_SCOPED_PROFILER_EVENT(renderer.getContext(), commandBuffer, "Render shadow casters")

					// Set graphics render target
					RERHI::Command::SetGraphicsRenderTarget::create(commandBuffer, mDepthFramebufferPtr);

					// Set the graphics viewport and scissor rectangle
					RERHI::Command::SetGraphicsViewportAndScissorRectangle::create(commandBuffer, 0, 0, mShadowMapSize, mShadowMapSize);

					{ // Clear the graphics depth buffer of the current render target
						const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
						RERHI::Command::ClearGraphics::create(commandBuffer, RERHI::ClearFlag::DEPTH, color);
					}

					// Render shadow casters
					// TODO(naetherm) Optimization: Do only render stuff which calls into the current shadow cascade
					RHI_ASSERT(nullptr != mRenderQueueIndexRange, "Invalid render queue index range")
					const MaterialTechniqueId materialTechniqueId = static_cast<const CompositorResourcePassScene&>(getCompositorResourcePass()).getMaterialTechniqueId();
					for (const RenderableManager* renderableManager : mRenderQueueIndexRange->renderableManagers)
					{
						// The render queue index range covered by this compositor instance pass scene might be smaller than the range of the
						// cached render queue index range. So, we could add a range check in here to reject renderable managers, but it's not
						// really worth to do so since the render queue only considers renderables inside the render queue range anyway.
						if (renderableManager->getCastShadows())
						{
							mRenderQueue.addRenderablesFromRenderableManager(*renderableManager, materialTechniqueId, shadowCompositorContextData, true);
						}
					}
					if (mRenderQueue.getNumberOfDrawCalls() > 0)
					{
						mRenderQueue.fillGraphicsCommandBuffer(*mDepthFramebufferPtr, shadowCompositorContextData, commandBuffer);
						mRenderQueue.clear();
					}
				}

				// Unset custom camera matrices
				const_cast<CameraSceneItem*>(cameraSceneItem)->unsetCustomWorldSpaceToViewSpaceMatrix();
				const_cast<CameraSceneItem*>(cameraSceneItem)->unsetCustomViewSpaceToClipSpaceMatrix();

				// Apply the scale/offset matrix, which transforms from [-1,1] post-projection space to [0,1] UV space
				const glm::mat4 shadowMatrix = RECore::Math::getTextureScaleBiasMatrix(renderer.getRhi().getCapabilities().upperLeftOrigin, renderer.getRhi().getCapabilities().zeroToOneClipZ) * viewSpaceToClipSpace;

				// Store the split distance in terms of view space depth
				const float clipDistance = cameraSceneItem->getFarZ() - cameraSceneItem->getNearZ();
				mPassData.shadowCascadeSplits[cascadeIndex] = cameraSceneItem->getNearZ() + splitDistance * clipDistance;
				if (0 == cascadeIndex)
				{
					mPassData.shadowMatrix = shadowMatrix;
					mPassData.shadowCascadeOffsets[0] = RECore::Math::VEC4_ZERO;
					mPassData.shadowCascadeScales[0] = RECore::Math::VEC4_ONE;
				}
				else
				{
					// Calculate the position of the lower corner of the cascade partition, in the UV space of the first cascade partition
					const glm::mat4 inverseShadowMatrix = glm::inverse(shadowMatrix);
					glm::vec4 cascadeCorner = ::detail::transformVectorByMatrix(inverseShadowMatrix, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
					cascadeCorner = ::detail::transformVectorByMatrix(mPassData.shadowMatrix, cascadeCorner);

					// Do the same for the upper corner
					glm::vec4 otherCorner = ::detail::transformVectorByMatrix(inverseShadowMatrix, RECore::Math::VEC4_ONE);
					otherCorner = ::detail::transformVectorByMatrix(mPassData.shadowMatrix, otherCorner);

					// Calculate the scale and offset
					const glm::vec4 cascadeScale = RECore::Math::VEC4_ONE / (otherCorner - cascadeCorner);
					mPassData.shadowCascadeOffsets[cascadeIndex] = glm::vec4(-glm::vec3(cascadeCorner), 0.0f);
					mPassData.shadowCascadeScales[cascadeIndex] = glm::vec4(glm::vec3(cascadeScale), 1.0f);
				}
				mPassData.currentShadowCascadeIndex = cascadeIndex;

				// Calculate exponential variance shadow map (EVSM) and blur if necessary
				const glm::vec4& cascadeScale = mPassData.shadowCascadeScales[cascadeIndex];
				const float filterSizeX = std::max(mShadowFilterSize * cascadeScale.x, 1.0f);
				const float filterSizeY = std::max(mShadowFilterSize * cascadeScale.y, 1.0f);
				if (filterSizeX > 1.0f || filterSizeY > 1.0f)
				{
					// Execute compositor instance pass compute, use cascade index three as intermediate render target
					RHI_ASSERT(nullptr != mVarianceFramebufferPtr[::detail::INTERMEDIATE_CASCADE_INDEX], "Invalid variance framebuffer")
					RERHI::Command::SetGraphicsRenderTarget::create(commandBuffer, mVarianceFramebufferPtr[::detail::INTERMEDIATE_CASCADE_INDEX]);
					mDepthToExponentialVarianceCompositorInstancePassCompute->onFillCommandBuffer(mVarianceFramebufferPtr[::detail::INTERMEDIATE_CASCADE_INDEX], shadowCompositorContextData, commandBuffer);
					mDepthToExponentialVarianceCompositorInstancePassCompute->onPostCommandBufferDispatch();

					// Horizontal blur
					mPassData.shadowFilterSize = filterSizeX;
					RERHI::Command::SetGraphicsRenderTarget::create(commandBuffer, mIntermediateFramebufferPtr);
					mHorizontalBlurCompositorInstancePassCompute->onFillCommandBuffer(mIntermediateFramebufferPtr, shadowCompositorContextData, commandBuffer);
					mHorizontalBlurCompositorInstancePassCompute->onPostCommandBufferDispatch();

					// Vertical blur
					mPassData.shadowFilterSize = filterSizeY;
					RHI_ASSERT(nullptr != mVarianceFramebufferPtr[cascadeIndex], "Invalid variance framebuffer")
					RERHI::Command::SetGraphicsRenderTarget::create(commandBuffer, mVarianceFramebufferPtr[cascadeIndex]);
					mVerticalBlurCompositorInstancePassCompute->onFillCommandBuffer(mVarianceFramebufferPtr[cascadeIndex], shadowCompositorContextData, commandBuffer);
					mVerticalBlurCompositorInstancePassCompute->onPostCommandBufferDispatch();
				}
				else
				{
					// Execute compositor instance pass compute
					RHI_ASSERT(nullptr != mVarianceFramebufferPtr[cascadeIndex], "Invalid variance framebuffer")
					RERHI::Command::SetGraphicsRenderTarget::create(commandBuffer, mVarianceFramebufferPtr[cascadeIndex]);
					mDepthToExponentialVarianceCompositorInstancePassCompute->onFillCommandBuffer(mVarianceFramebufferPtr[cascadeIndex], shadowCompositorContextData, commandBuffer);
					mDepthToExponentialVarianceCompositorInstancePassCompute->onPostCommandBufferDispatch();
				}
			}
		}
		else
		{
			// Error!
			RHI_ASSERT(false, "We should never end up in here")
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassShadowMap::CompositorInstancePassShadowMap(const CompositorResourcePassShadowMap& compositorResourcePassShadowMap, const CompositorNodeInstance& compositorNodeInstance) :
		CompositorInstancePassScene(compositorResourcePassShadowMap, compositorNodeInstance, true),
		// Settings
		mEnabled(true),
		mShadowMapSize(1024),
		mNumberOfShadowCascades(4),
		mNumberOfShadowMultisamples(2),
		mCascadeSplitsLambda(0.99f),
		mShadowFilterSize(8.0f),
		mStabilizeCascades(true),
		// Internal
		mSettingsGenerationCounter(0),
		mUsedSettingsGenerationCounter(0),
		mDepthTextureResourceId(RECore::getInvalid<TextureResourceId>()),
		mVarianceTextureResourceId(RECore::getInvalid<TextureResourceId>()),
		mIntermediateDepthBlurTextureResourceId(RECore::getInvalid<TextureResourceId>()),
		mDepthToExponentialVarianceCompositorResourcePassCompute(nullptr),
		mDepthToExponentialVarianceCompositorInstancePassCompute(nullptr),
		mHorizontalBlurCompositorResourcePassCompute(nullptr),
		mHorizontalBlurCompositorInstancePassCompute(nullptr),
		mVerticalBlurCompositorResourcePassCompute(nullptr),
		mVerticalBlurCompositorInstancePassCompute(nullptr)
	{
		mPassData.shadowMatrix = RECore::Math::MAT4_IDENTITY;
		for (int i = 0; i < CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES; ++i)
		{
			mPassData.shadowCascadeScales[i] = RECore::Math::VEC4_ONE;
		}
		createShadowMapRenderTarget();
	}

	void CompositorInstancePassShadowMap::createShadowMapRenderTarget()
	{
		const CompositorResourcePassShadowMap& compositorResourcePassShadowMap = static_cast<const CompositorResourcePassShadowMap&>(getCompositorResourcePass());
		const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();
		const AssetId assetId = compositorResourcePassShadowMap.getTextureAssetId();

		// Tell the texture resource manager about our render target texture so it can be referenced inside e.g. compositor nodes
		TextureResourceManager& textureResourceManager = renderer.getTextureResourceManager();
		TextureResource* textureResource = textureResourceManager.getTextureResourceByAssetId(assetId);
		if (nullptr == textureResource)
		{
			RERHI::RHIDynamicRHI& rhi = renderer.getRhi();
			if (mEnabled)
			{
				// Check shadow map settings
				RHI_ASSERT(mNumberOfShadowCascades <= CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES, "Invalid number of shadow cascades")
				RHI_ASSERT(mNumberOfShadowMultisamples >= 1, "Invalid number of shadow multisamples")
				uint8_t numberOfShadowMultisamples = mNumberOfShadowMultisamples;
				{ // Multisamples sanity check
					const uint8_t maximumNumberOfMultisamples = rhi.getCapabilities().maximumNumberOfMultisamples;
					if (numberOfShadowMultisamples > maximumNumberOfMultisamples)
					{
						RHI_ASSERT(false, "Number of shadow multisamples not supported by the RHI implementation")
						numberOfShadowMultisamples = maximumNumberOfMultisamples;
					}
				}

				{ // Depth shadow map
					const RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::D32_FLOAT;
					RERHI::RHITexture* texture = renderer.getTextureManager().createTexture2D(mShadowMapSize, mShadowMapSize, textureFormat, nullptr, RERHI::TextureFlag::SHADER_RESOURCE | RERHI::TextureFlag::RENDER_TARGET, RERHI::TextureUsage::DEFAULT, numberOfShadowMultisamples, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass depth shadow map"));

					{ // Create the framebuffer object (FBO) instance
						const RERHI::FramebufferAttachment depthStencilFramebufferAttachment(texture);
						mDepthFramebufferPtr = rhi.createFramebuffer(*rhi.createRenderPass(0, nullptr, textureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Compositor instance pass depth shadow map")), nullptr, &depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME("Compositor instance pass depth shadow map"));
					}

					// Create texture resource
					mDepthTextureResourceId = textureResourceManager.createTextureResourceByAssetId(::detail::DEPTH_SHADOW_MAP_TEXTURE_ASSET_ID, *texture);
				}

				{ // Depth to exponential variance
					MaterialProperties materialProperties;
					materialProperties.setPropertyById(STRING_ID("DepthMap"), MaterialPropertyValue::fromTextureAssetId(::detail::DEPTH_SHADOW_MAP_TEXTURE_ASSET_ID), MaterialProperty::Usage::UNKNOWN, true);
					materialProperties.setPropertyById(STRING_ID("NumberOfMultisamples"), MaterialPropertyValue::fromInteger((numberOfShadowMultisamples == 1) ? 0 : numberOfShadowMultisamples), MaterialProperty::Usage::UNKNOWN, true);
					mDepthToExponentialVarianceCompositorResourcePassCompute = new CompositorResourcePassCompute(compositorResourcePassShadowMap.getCompositorTarget(), compositorResourcePassShadowMap.getDepthToExponentialVarianceMaterialBlueprintAssetId(), materialProperties);
					#if defined(DEBUG) || defined(RENDERER_PROFILER)
						mDepthToExponentialVarianceCompositorResourcePassCompute->setDebugName("Depth to exponential variance");
					#endif
					mDepthToExponentialVarianceCompositorInstancePassCompute = new CompositorInstancePassCompute(*mDepthToExponentialVarianceCompositorResourcePassCompute, getCompositorNodeInstance());
				}

				{ // Variance shadow map
					const RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::R32G32B32A32F;
					RERHI::RHITexture* texture = renderer.getTextureManager().createTexture2DArray(mShadowMapSize, mShadowMapSize, CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES, textureFormat, nullptr, RERHI::TextureFlag::SHADER_RESOURCE | RERHI::TextureFlag::RENDER_TARGET, RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME("Compositor instance pass variance shadow map"));

					// Create the framebuffer object (FBO) instances
					RERHI::RHIRenderPass* renderPass = rhi.createRenderPass(1, &textureFormat, RERHI::TextureFormat::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("Compositor instance pass variance shadow map"));
					for (uint8_t cascadeIndex = 0; cascadeIndex < CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES; ++cascadeIndex)
					{
						const RERHI::FramebufferAttachment colorFramebufferAttachment(texture, 0, cascadeIndex);
						mVarianceFramebufferPtr[cascadeIndex] = rhi.createFramebuffer(*renderPass, &colorFramebufferAttachment, nullptr RHI_RESOURCE_DEBUG_NAME(("Compositor instance pass variance shadow map " + std::to_string(cascadeIndex)).c_str()));
					}
					for (uint8_t cascadeIndex = CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES; cascadeIndex < CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES; ++cascadeIndex)
					{
						mVarianceFramebufferPtr[cascadeIndex] = nullptr;
					}

					// Create texture resource
					mVarianceTextureResourceId = textureResourceManager.createTextureResourceByAssetId(assetId, *texture);
				}

				{ // Intermediate depth blur shadow map
					const RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::R32G32B32A32F;
					RERHI::RHITexture* texture = renderer.getTextureManager().createTexture2D(mShadowMapSize, mShadowMapSize, textureFormat, nullptr, RERHI::TextureFlag::SHADER_RESOURCE | RERHI::TextureFlag::RENDER_TARGET, RERHI::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass intermediate depth blur shadow map"));

					{ // Create the framebuffer object (FBO) instance
						const RERHI::FramebufferAttachment colorFramebufferAttachment(texture);
						mIntermediateFramebufferPtr = rhi.createFramebuffer(*rhi.createRenderPass(1, &textureFormat, RERHI::TextureFormat::UNKNOWN, 1 RHI_RESOURCE_DEBUG_NAME("Compositor instance pass intermediate depth blur shadow map")), &colorFramebufferAttachment, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass intermediate depth blur shadow map"));
					}

					// Create texture resource
					mIntermediateDepthBlurTextureResourceId = textureResourceManager.createTextureResourceByAssetId(::detail::INTERMEDIATE_DEPTH_BLUR_SHADOW_MAP_TEXTURE_ASSET_ID, *texture);
				}

				{ // Horizontal blur
					MaterialProperties materialProperties;
					materialProperties.setPropertyById(STRING_ID("VerticalBlur"), MaterialPropertyValue::fromBoolean(false), MaterialProperty::Usage::UNKNOWN, true);
					materialProperties.setPropertyById(STRING_ID("ColorMap"), MaterialPropertyValue::fromTextureAssetId(assetId), MaterialProperty::Usage::UNKNOWN, true);
					mHorizontalBlurCompositorResourcePassCompute = new CompositorResourcePassCompute(compositorResourcePassShadowMap.getCompositorTarget(), compositorResourcePassShadowMap.getBlurMaterialBlueprintAssetId(), materialProperties);
					#if defined(DEBUG) || defined(RENDERER_PROFILER)
						mHorizontalBlurCompositorResourcePassCompute->setDebugName("Horizontal blur");
					#endif
					mHorizontalBlurCompositorInstancePassCompute = new CompositorInstancePassCompute(*mHorizontalBlurCompositorResourcePassCompute, getCompositorNodeInstance());
				}

				{ // Vertical blur
					MaterialProperties materialProperties;
					materialProperties.setPropertyById(STRING_ID("VerticalBlur"), MaterialPropertyValue::fromBoolean(true), MaterialProperty::Usage::UNKNOWN, true);
					materialProperties.setPropertyById(STRING_ID("ColorMap"), MaterialPropertyValue::fromTextureAssetId(::detail::INTERMEDIATE_DEPTH_BLUR_SHADOW_MAP_TEXTURE_ASSET_ID), MaterialProperty::Usage::UNKNOWN, true);
					mVerticalBlurCompositorResourcePassCompute = new CompositorResourcePassCompute(compositorResourcePassShadowMap.getCompositorTarget(), compositorResourcePassShadowMap.getBlurMaterialBlueprintAssetId(), materialProperties);
					#if defined(DEBUG) || defined(RENDERER_PROFILER)
						mVerticalBlurCompositorResourcePassCompute->setDebugName("Vertical blur");
					#endif
					mVerticalBlurCompositorInstancePassCompute = new CompositorInstancePassCompute(*mVerticalBlurCompositorResourcePassCompute, getCompositorNodeInstance());
				}
			}
			else
			{
				// If shadow is disabled, we still need to create at least a dummy for the resulting main variance shadow map resource
				const float data[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				mVarianceTextureResourceId = textureResourceManager.createTextureResourceByAssetId(
					assetId,
					*renderer.getTextureManager().createTexture2DArray(1, 1, 1, RERHI::TextureFormat::R32G32B32A32F, data, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME("Compositor instance pass variance shadow map")));
			}
		}
		else
		{
			// This is not allowed to happen
			RHI_ASSERT(false, "We should never end up in here")
		}
	}

	void CompositorInstancePassShadowMap::destroyShadowMapRenderTarget()
	{
		RHI_ASSERT(RECore::isValid(mVarianceTextureResourceId), "Invalid compositor instance pass resource")

		// Depth to exponential variance
		delete mDepthToExponentialVarianceCompositorInstancePassCompute;
		mDepthToExponentialVarianceCompositorInstancePassCompute = nullptr;
		delete mDepthToExponentialVarianceCompositorResourcePassCompute;
		mDepthToExponentialVarianceCompositorResourcePassCompute = nullptr;

		// Horizontal blur
		delete mHorizontalBlurCompositorResourcePassCompute;
		mHorizontalBlurCompositorResourcePassCompute = nullptr;
		delete mHorizontalBlurCompositorInstancePassCompute;
		mHorizontalBlurCompositorInstancePassCompute = nullptr;

		// Vertical blur
		delete mVerticalBlurCompositorResourcePassCompute;
		mVerticalBlurCompositorResourcePassCompute = nullptr;
		delete mVerticalBlurCompositorInstancePassCompute;
		mVerticalBlurCompositorInstancePassCompute = nullptr;

		// Release the framebuffers and other RHI resources referenced by the framebuffers
		mDepthFramebufferPtr = nullptr;
		for (uint8_t cascadeIndex = 0; cascadeIndex < CompositorResourcePassShadowMap::MAXIMUM_NUMBER_OF_SHADOW_CASCADES; ++cascadeIndex)
		{
			mVarianceFramebufferPtr[cascadeIndex] = nullptr;
		}
		mIntermediateFramebufferPtr = nullptr;

		// Inform the texture resource manager that our render target texture is gone now
		TextureResourceManager& textureResourceManager = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getTextureResourceManager();
		if (RECore::isValid(mDepthTextureResourceId))
		{
			textureResourceManager.destroyTextureResource(mDepthTextureResourceId);
			RECore::setInvalid(mDepthTextureResourceId);
		}
		textureResourceManager.destroyTextureResource(mVarianceTextureResourceId);
		RECore::setInvalid(mVarianceTextureResourceId);
		if (RECore::isValid(mIntermediateDepthBlurTextureResourceId))
		{
			textureResourceManager.destroyTextureResource(mIntermediateDepthBlurTextureResourceId);
			RECore::setInvalid(mIntermediateDepthBlurTextureResourceId);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
