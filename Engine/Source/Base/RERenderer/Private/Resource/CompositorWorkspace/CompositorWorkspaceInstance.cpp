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
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceResource.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeResource.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeResourceManager.h"
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorPassFactory.h"
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorInstancePass.h"
#include "RERenderer/Resource/CompositorNode/Pass/ShadowMap/CompositorInstancePassShadowMap.h"
#include "RERenderer/Resource/CompositorNode/Pass/ShadowMap/CompositorResourcePassShadowMap.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/IndirectBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/LightBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/PassBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/Scene/Item/Camera/CameraSceneItem.h"
#include "RERenderer/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.h"
#include "RERenderer/Resource/Scene/Culling/SceneCullingManager.h"
#include "RERenderer/Core/IProfiler.h"
#include "RERenderer/Core/Renderer/FramebufferManager.h"
#include "RERenderer/Core/Renderer/RenderTargetTextureManager.h"
#ifdef RENDERER_GRAPHICS_DEBUGGER
	#include "RERenderer/Core/IGraphicsDebugger.h"
#endif
#ifdef RENDERER_OPENVR
	#include "RERenderer/Vr/IVrManager.h"
#endif
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
	CompositorWorkspaceInstance::CompositorWorkspaceInstance(IRenderer& renderer, AssetId compositorWorkspaceAssetId) :
		mRenderer(renderer),
		mNumberOfMultisamples(1),
		mCurrentlyUsedNumberOfMultisamples(1),
		mResolutionScale(1.0f),
		mRenderTargetWidth(RECore::getInvalid<RECore::uint32>()),
		mRenderTargetHeight(RECore::getInvalid<RECore::uint32>()),
		mCompositorWorkspaceResourceId(RECore::getInvalid<CompositorWorkspaceResourceId>()),
		mFramebufferManagerInitialized(false),
		mExecutionRenderTarget(nullptr),
		mCompositorInstancePassShadowMap(nullptr)
		#ifdef RHI_STATISTICS
			, mPipelineStatisticsQueryPoolPtr((renderer.getRhi().getNameId() == RERHI::NameId::OPENGL && strstr(renderer.getRhi().getCapabilities().deviceName, "AMD ") != nullptr) ? nullptr : renderer.getRhi().createQueryPool(RERHI::QueryType::PIPELINE_STATISTICS, 2 RHI_RESOURCE_DEBUG_NAME("Compositor workspace instance"))),	// TODO(naetherm) When using OpenGL "GL_ARB_pipeline_statistics_query" features, "glCopyImageSubData()" will horribly stall/freeze on Windows using AMD Radeon 18.12.2 (tested on 16 December 2018). No issues with NVIDIA GeForce game ready driver 417.35 (release data 12/12/2018).
			mPreviousCurrentPipelineStatisticsQueryIndex(RECore::getInvalid<RECore::uint32>()),
			mCurrentPipelineStatisticsQueryIndex(0),
			mPipelineStatisticsQueryResult{}
		#endif
	{
		renderer.getCompositorWorkspaceResourceManager().loadCompositorWorkspaceResourceByAssetId(compositorWorkspaceAssetId, mCompositorWorkspaceResourceId, this);
	}

	CompositorWorkspaceInstance::~CompositorWorkspaceInstance()
	{
		// Cleanup
		destroySequentialCompositorNodeInstances();
	}

	void CompositorWorkspaceInstance::setNumberOfMultisamples(RECore::uint8 numberOfMultisamples)
	{
		// Sanity checks
		RHI_ASSERT(numberOfMultisamples == 1 || numberOfMultisamples == 2 || numberOfMultisamples == 4 || numberOfMultisamples == 8, "Invalid number of multisamples")
		RHI_ASSERT(numberOfMultisamples <= mRenderer.getRhi().getCapabilities().maximumNumberOfMultisamples, "Invalid number of multisamples")

		// Set the value
		mNumberOfMultisamples = numberOfMultisamples;
	}

	const CompositorWorkspaceInstance::RenderQueueIndexRange* CompositorWorkspaceInstance::getRenderQueueIndexRangeByRenderQueueIndex(RECore::uint8 renderQueueIndex) const
	{
		for (const RenderQueueIndexRange& renderQueueIndexRange : mRenderQueueIndexRanges)
		{
			if (renderQueueIndex >= renderQueueIndexRange.minimumRenderQueueIndex && renderQueueIndex <= renderQueueIndexRange.maximumRenderQueueIndex)
			{
				return &renderQueueIndexRange;
			}
		}
		return nullptr;
	}

	const ICompositorInstancePass* CompositorWorkspaceInstance::getFirstCompositorInstancePassByCompositorPassTypeId(CompositorPassTypeId compositorPassTypeId) const
	{
		for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
		{
			for (const ICompositorInstancePass* compositorInstancePass : compositorNodeInstance->mCompositorInstancePasses)
			{
				if (compositorInstancePass->getCompositorResourcePass().getTypeId() == compositorPassTypeId)
				{
					return compositorInstancePass;
				}
			}
		}

		// No compositor instance pass with the provided compositor pass type ID found
		return nullptr;
	}

	void CompositorWorkspaceInstance::executeVr(RERHI::RHIRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem)
	{
		// Decide whether or not the VR-manager is used for rendering
		#ifdef RENDERER_OPENVR
			IVrManager& vrManager = mRenderer.getVrManager();
			if (vrManager.isRunning())
			{
				// Update the VR-manager just before rendering
				vrManager.updateHmdMatrixPose(cameraSceneItem);

				// Execute the compositor workspace instance
				vrManager.executeCompositorWorkspaceInstance(*this, renderTarget, cameraSceneItem, lightSceneItem);
			}
			else
		#endif
		{
			// Execute the compositor workspace instance
			execute(renderTarget, cameraSceneItem, lightSceneItem);
		}
	}

	void CompositorWorkspaceInstance::execute(RERHI::RHIRenderTarget& renderTarget, const CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem, bool singlePassStereoInstancing)
	{
		// Clear the command buffer from the previous frame
		// -> We didn't request a clear together with the dispatch so e.g. "RERenderer::DebugGuiHelper::drawMetricsWindow()" can show some command buffer statistics
		mCommandBuffer.clear();

		// We could directly clear the render queue index ranges renderable managers as soon as the frame rendering has been finished to avoid evil dangling pointers,
		// but on the other hand a responsible user might be interested in the potentially on-screen renderable managers to perform work which should only be performed
		// on potentially on-screen stuff
		// -> Ensure that this clear step is really always performed when calling this execute method (evil dangling alert)
		clearRenderQueueIndexRangesRenderableManagers();

		// Is the compositor workspace resource ready?
		const CompositorWorkspaceResource* compositorWorkspaceResource = mRenderer.getCompositorWorkspaceResourceManager().tryGetById(mCompositorWorkspaceResourceId);
		if (nullptr != compositorWorkspaceResource && compositorWorkspaceResource->getLoadingState() == RECore::IResource::LoadingState::LOADED)
		{
			// Tell the global material properties managed by the material blueprint resource manager about the number of multisamples
			// -> Since there can be multiple compositor workspace instances we can't do this once inside "RERenderer::CompositorWorkspaceInstance::setNumberOfMultisamples()"
			MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
			materialBlueprintResourceManager.getGlobalMaterialProperties().setPropertyById(STRING_ID("GlobalNumberOfMultisamples"), MaterialPropertyValue::fromInteger((mNumberOfMultisamples == 1) ? 0 : mNumberOfMultisamples));

			// Add reference to the render target
			renderTarget.AddReference();
			mExecutionRenderTarget = &renderTarget;

			// Get the main render target size
			RECore::uint32 renderTargetWidth  = 1;
			RECore::uint32 renderTargetHeight = 1;
			renderTarget.getWidthAndHeight(renderTargetWidth, renderTargetHeight);

			{ // Do we need to destroy previous framebuffers and render target textures?
				bool destroy = false;
				if (mCurrentlyUsedNumberOfMultisamples != mNumberOfMultisamples)
				{
					mCurrentlyUsedNumberOfMultisamples = mNumberOfMultisamples;
					destroy = true;
				}
				{
					const RECore::uint32 currentRenderTargetWidth  = static_cast<RECore::uint32>(static_cast<float>(renderTargetWidth) * mResolutionScale);
					const RECore::uint32 currentRenderTargetHeight = static_cast<RECore::uint32>(static_cast<float>(renderTargetHeight) * mResolutionScale);
					if (mRenderTargetWidth != currentRenderTargetWidth || mRenderTargetHeight != currentRenderTargetHeight)
					{
						mRenderTargetWidth  = currentRenderTargetWidth;
						mRenderTargetHeight = currentRenderTargetHeight;
						destroy = true;
					}
				}
				if (destroy)
				{
					destroyFramebuffersAndRenderTargetTextures();
				}
			}

			// Create framebuffers and render target textures, if required
			if (!mFramebufferManagerInitialized)
			{
				createFramebuffersAndRenderTargetTextures(renderTarget);
			}

			// Begin scene rendering
			RERHI::RHIDynamicRHI& rhi = renderTarget.getRhi();
			{
				#ifdef RENDERER_GRAPHICS_DEBUGGER
					IGraphicsDebugger& graphicsDebugger = mRenderer.getContext().getGraphicsDebugger();
					if (graphicsDebugger.getCaptureNextFrame())
					{
						graphicsDebugger.startFrameCapture((renderTarget.getResourceType() == RERHI::ResourceType::SWAP_CHAIN) ? static_cast<RERHI::RHISwapChain&>(renderTarget).getNativeWindowHandle() : NULL_HANDLE);
					}
				#endif
				#ifdef RHI_STATISTICS
					if (nullptr != mPipelineStatisticsQueryPoolPtr)
					{
						RERHI::Command::ResetAndBeginQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPoolPtr, mCurrentPipelineStatisticsQueryIndex);
					}
				#endif

				const CompositorContextData compositorContextData(this, cameraSceneItem, singlePassStereoInstancing, lightSceneItem, mCompositorInstancePassShadowMap);
				if (nullptr != cameraSceneItem)
				{
					// Gather render queue index ranges renderable managers
					mExecuteOnRenderingSceneItems.clear();
					cameraSceneItem->getSceneResource().getSceneCullingManager().gatherRenderQueueIndexRangesRenderableManagers(renderTarget, compositorContextData, mRenderQueueIndexRanges, mExecuteOnRenderingSceneItems);

					// Execute on rendering scene items
					for (ISceneItem* sceneItem : mExecuteOnRenderingSceneItems)
					{
						sceneItem->onExecuteOnRendering(renderTarget, compositorContextData, mCommandBuffer);
					}

					// Fill the light buffer manager
					materialBlueprintResourceManager.getLightBufferManager().fillBuffer(compositorContextData.getWorldSpaceCameraPosition(), cameraSceneItem->getSceneResource(), mCommandBuffer);
				}

				{ // Scene rendering
					// Combined scoped profiler CPU and GPU sample as well as RHI debug event command
					RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), mCommandBuffer, "Compositor workspace")

					// Fill command buffer
					RERHI::RHIRenderTarget* currentRenderTarget = &renderTarget;
					for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
					{
						currentRenderTarget = &compositorNodeInstance->fillCommandBuffer(*currentRenderTarget, compositorContextData, mCommandBuffer);
					}
				}

				{ // Dispatch command buffer to the RHI implementation
					// The command buffer is about to be dispatched, inform everyone who cares about this
					materialBlueprintResourceManager.onPreCommandBufferDispatch();

					// Dispatch command buffer to the RHI implementation
					#ifdef RHI_STATISTICS
						if (nullptr != mPipelineStatisticsQueryPoolPtr)
						{
							RERHI::Command::EndQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPoolPtr, mCurrentPipelineStatisticsQueryIndex);
						}
					#endif
					mCommandBuffer.dispatchToRhi(rhi);

					// The command buffer has been dispatched, inform everyone who cares about this
					for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
					{
						compositorNodeInstance->onPostCommandBufferDispatch();
					}
					{
						const RECore::uint32 numberOfResources = materialBlueprintResourceManager.getNumberOfResources();
						for (RECore::uint32 i = 0; i < numberOfResources; ++i)
						{
							PassBufferManager* passBufferManager = materialBlueprintResourceManager.getByIndex(i).getPassBufferManager();
							if (nullptr != passBufferManager)
							{
								passBufferManager->onPostCommandBufferDispatch();
							}
						}
					}
				}

				// End scene rendering
				#ifdef RENDERER_GRAPHICS_DEBUGGER
					if (graphicsDebugger.getCaptureNextFrame())
					{
						graphicsDebugger.endFrameCapture((renderTarget.getResourceType() == RERHI::ResourceType::SWAP_CHAIN) ? static_cast<RERHI::RHISwapChain&>(renderTarget).getNativeWindowHandle() : NULL_HANDLE);
					}
				#endif
			}

			// In case the render target is a swap chain, present the content of the current back buffer
			if (renderTarget.getResourceType() == RERHI::ResourceType::SWAP_CHAIN)
			{
				static_cast<RERHI::RHISwapChain&>(renderTarget).present();
			}

			// Pipeline statistics query pool
			#ifdef RHI_STATISTICS
				if (nullptr != mPipelineStatisticsQueryPoolPtr)
				{
					// We explicitly wait if the previous result isn't available yet to avoid
					// "D3D11 WARNING: ID3D10Query::Begin: Begin is being invoked on a Query, where the previous results have not been obtained with GetData. This is valid; but unusual. The previous results are being abandoned, and new Query results will be generated. [ EXECUTION WARNING #408: QUERY_BEGIN_ABANDONING_PREVIOUS_RESULTS]"
					if (RECore::isValid(mPreviousCurrentPipelineStatisticsQueryIndex) && !rhi.getQueryPoolResults(*mPipelineStatisticsQueryPoolPtr, sizeof(RERHI::PipelineStatisticsQueryResult), reinterpret_cast<RECore::uint8*>(&mPipelineStatisticsQueryResult), mPreviousCurrentPipelineStatisticsQueryIndex, 1, 0, RERHI::QueryResultFlags::WAIT))
					{
						mPipelineStatisticsQueryResult = {};
					}
					mPreviousCurrentPipelineStatisticsQueryIndex = mCurrentPipelineStatisticsQueryIndex;
					mCurrentPipelineStatisticsQueryIndex = (0 == mCurrentPipelineStatisticsQueryIndex) ? 1u : 0u;
				}
			#endif

			// Release reference from the render target
			mExecutionRenderTarget = nullptr;
			renderTarget.Release();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	void CompositorWorkspaceInstance::onLoadingStateChange(const RECore::IResource& resource)
	{
		// Destroy the previous stuff
		destroySequentialCompositorNodeInstances();

		// Handle loaded state
		if (resource.getLoadingState() == RECore::IResource::LoadingState::LOADED)
		{
			// TODO(naetherm) Just a first test, need to complete and refine the implementation
			CompositorNodeResourceManager& compositorNodeResourceManager = mRenderer.getCompositorNodeResourceManager();
			const ICompositorPassFactory& compositorPassFactory = compositorNodeResourceManager.getCompositorPassFactory();
			RenderTargetTextureManager& renderTargetTextureManager = mRenderer.getCompositorWorkspaceResourceManager().getRenderTargetTextureManager();
			FramebufferManager& framebufferManager = mRenderer.getCompositorWorkspaceResourceManager().getFramebufferManager();

			// For render queue index ranges gathering and merging
			typedef std::pair<RECore::uint8, RECore::uint8> LocalRenderQueueIndexRange;
			typedef std::vector<LocalRenderQueueIndexRange> LocalRenderQueueIndexRanges;
			LocalRenderQueueIndexRanges individualRenderQueueIndexRanges;

			// Compositor node resources
			const CompositorWorkspaceResource::CompositorNodeAssetIds& compositorNodeAssetIds = static_cast<const CompositorWorkspaceResource&>(resource).getCompositorNodeAssetIds();
			const size_t numberOfCompositorResourceNodes = compositorNodeAssetIds.size();
			for (size_t nodeIndex = 0; nodeIndex < numberOfCompositorResourceNodes; ++nodeIndex)
			{
				// Get the compositor node resource instance
				CompositorNodeResourceId compositorNodeResourceId = RECore::getInvalid<CompositorNodeResourceId>();
				compositorNodeResourceManager.loadCompositorNodeResourceByAssetId(compositorNodeAssetIds[nodeIndex], compositorNodeResourceId);
				CompositorNodeResource& compositorNodeResource = compositorNodeResourceManager.getById(compositorNodeResourceId);

				// TODO(naetherm) Ensure compositor node resource loading is done. Such blocking waiting is no good thing.
				compositorNodeResource.enforceFullyLoaded();

				// Add render target textures and framebuffers (doesn't directly allocate RHI resources, just announces them)
				for (const CompositorRenderTargetTexture& compositorRenderTargetTexture : compositorNodeResource.getRenderTargetTextures())
				{
					renderTargetTextureManager.addRenderTargetTexture(compositorRenderTargetTexture.getAssetId(), compositorRenderTargetTexture.getRenderTargetTextureSignature());
				}
				for (const CompositorFramebuffer& compositorFramebuffer : compositorNodeResource.getFramebuffers())
				{
					framebufferManager.addFramebuffer(compositorFramebuffer.getCompositorFramebufferId(), compositorFramebuffer.getFramebufferSignature());
				}

				// Create the compositor node instance
				CompositorNodeInstance* compositorNodeInstance = new CompositorNodeInstance(compositorNodeResourceId, *this);
				mSequentialCompositorNodeInstances.push_back(compositorNodeInstance);

				{ // Compositor node resource targets
					const CompositorNodeResource::CompositorTargets& compositorTargets = compositorNodeResource.getCompositorTargets();
					const size_t numberOfCompositorTargets = compositorTargets.size();
					for (size_t targetIndex = 0; targetIndex < numberOfCompositorTargets; ++targetIndex)
					{
						// Get the compositor node resource target instance
						const CompositorTarget& compositorTarget = compositorTargets[targetIndex];

						{ // Compositor node resource target passes
							const CompositorTarget::CompositorResourcePasses& compositorResourcePasses = compositorTarget.getCompositorResourcePasses();
							const size_t numberOfCompositorResourcePasses = compositorResourcePasses.size();
							for (size_t passIndex = 0; passIndex < numberOfCompositorResourcePasses; ++passIndex)
							{
								// Get the compositor resource target instance
								const ICompositorResourcePass* compositorResourcePass = compositorResourcePasses[passIndex];

								// Create the compositor instance pass
								if (nullptr != compositorResourcePass)
								{
									// Create the compositor instance pass
									ICompositorInstancePass* compositorInstancePass = compositorPassFactory.createCompositorInstancePass(*compositorResourcePass, *compositorNodeInstance);
									if (compositorResourcePass->getTypeId() == CompositorResourcePassShadowMap::TYPE_ID)
									{
										RHI_ASSERT(nullptr == mCompositorInstancePassShadowMap, "Invalid compositor instance pass shadow map")
										mCompositorInstancePassShadowMap = static_cast<CompositorInstancePassShadowMap*>(compositorInstancePass);
									}
									compositorNodeInstance->mCompositorInstancePasses.push_back(compositorInstancePass);

									// Gather render queue index range
									RECore::uint8 minimumRenderQueueIndex = 0;
									RECore::uint8 maximumRenderQueueIndex = 0;
									if (compositorResourcePass->getRenderQueueIndexRange(minimumRenderQueueIndex, maximumRenderQueueIndex))
									{
										individualRenderQueueIndexRanges.emplace_back(minimumRenderQueueIndex, maximumRenderQueueIndex);
									}
								}
							}
						}
					}
				}
			}

			// Merge the render queue index ranges using the algorithm described at http://stackoverflow.com/a/5276789
			if (!individualRenderQueueIndexRanges.empty())
			{
				LocalRenderQueueIndexRanges mergedRenderQueueIndexRanges;
				mergedRenderQueueIndexRanges.reserve(individualRenderQueueIndexRanges.size());
				std::sort(individualRenderQueueIndexRanges.begin(), individualRenderQueueIndexRanges.end());
				LocalRenderQueueIndexRanges::iterator iterator = individualRenderQueueIndexRanges.begin();
				LocalRenderQueueIndexRange current = *(iterator)++;
				while (iterator != individualRenderQueueIndexRanges.end())
				{
					if (current.second >= iterator->first)
					{
						current.second = std::max(current.second, iterator->second);
					}
					else
					{
						mergedRenderQueueIndexRanges.push_back(current);
						current = *iterator;
					}
					++iterator;
				}
				mergedRenderQueueIndexRanges.push_back(current);

				// Fill our final render queue index ranges data structure
				mRenderQueueIndexRanges.reserve(mergedRenderQueueIndexRanges.size());
				for (const LocalRenderQueueIndexRange& localRenderQueueIndexRange : mergedRenderQueueIndexRanges)
				{
					mRenderQueueIndexRanges.emplace_back(localRenderQueueIndexRange.first, localRenderQueueIndexRange.second);
				}
			}

			// Tell all compositor node instances that the compositor workspace instance loading has been finished
			for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
			{
				compositorNodeInstance->compositorWorkspaceInstanceLoadingFinished();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CompositorWorkspaceInstance::destroySequentialCompositorNodeInstances()
	{
		for (CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
		{
			delete compositorNodeInstance;
		}
		mSequentialCompositorNodeInstances.clear();
		mRenderQueueIndexRanges.clear();
		mCompositorInstancePassShadowMap = nullptr;

		// Destroy framebuffers and render target textures
		destroyFramebuffersAndRenderTargetTextures(true);
	}

	void CompositorWorkspaceInstance::createFramebuffersAndRenderTargetTextures(const RERHI::RHIRenderTarget& mainRenderTarget)
	{
		RHI_ASSERT(!mFramebufferManagerInitialized, "Framebuffer manager is already initialized")
		CompositorWorkspaceResourceManager& compositorWorkspaceResourceManager = mRenderer.getCompositorWorkspaceResourceManager();

		{ // Framebuffers
			FramebufferManager& framebufferManager = compositorWorkspaceResourceManager.getFramebufferManager();
			for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
			{
				for (ICompositorInstancePass* compositorInstancePass : compositorNodeInstance->mCompositorInstancePasses)
				{
					const CompositorFramebufferId compositorFramebufferId = compositorInstancePass->getCompositorResourcePass().getCompositorTarget().getCompositorFramebufferId();
					if (RECore::isValid(compositorFramebufferId))
					{
						compositorInstancePass->mRenderTarget = framebufferManager.getFramebufferByCompositorFramebufferId(compositorFramebufferId, mainRenderTarget, mCurrentlyUsedNumberOfMultisamples, mResolutionScale);
					}
				}
			}
		}

		{ // Textures not referenced by a framebuffer (e.g. used for unordered access or resource copy)
			RenderTargetTextureManager& renderTargetTextureManager = compositorWorkspaceResourceManager.getRenderTargetTextureManager();
			const CompositorNodeResourceManager& compositorNodeResourceManager = mRenderer.getCompositorNodeResourceManager();
			for (const CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
			{
				const CompositorNodeResource& compositorNodeResource = compositorNodeResourceManager.getById(compositorNodeInstance->getCompositorNodeResourceId());
				for (const CompositorRenderTargetTexture& compositorRenderTargetTexture : compositorNodeResource.getRenderTargetTextures())
				{
					const RenderTargetTextureSignature& renderTargetTextureSignature = compositorRenderTargetTexture.getRenderTargetTextureSignature();
					if ((renderTargetTextureSignature.getFlags() & RenderTargetTextureSignature::Flag::RENDER_TARGET) == 0)
					{
						// Force creating the texture in case it doesn't exist yet
						[[maybe_unused]] RERHI::RHITexture* texture = renderTargetTextureManager.getTextureByAssetId(compositorRenderTargetTexture.getAssetId(), mainRenderTarget, mCurrentlyUsedNumberOfMultisamples, mResolutionScale, nullptr);
					}
				}
			}
		}

		mFramebufferManagerInitialized = true;
	}

	void CompositorWorkspaceInstance::destroyFramebuffersAndRenderTargetTextures(bool clearManagers)
	{
		// All compositor instance passes need to forget about the render targets
		for (CompositorNodeInstance* compositorNodeInstance : mSequentialCompositorNodeInstances)
		{
			for (ICompositorInstancePass* compositorInstancePass : compositorNodeInstance->mCompositorInstancePasses)
			{
				compositorInstancePass->mRenderTarget = nullptr;
				compositorInstancePass->mNumberOfExecutionRequests = 0;
			}
		}

		// Destroy RHI resources of framebuffers and render target textures
		CompositorWorkspaceResourceManager& compositorWorkspaceResourceManager = mRenderer.getCompositorWorkspaceResourceManager();
		if (clearManagers)
		{
			compositorWorkspaceResourceManager.getFramebufferManager().clear();
			compositorWorkspaceResourceManager.getRenderTargetTextureManager().clear();
		}
		else
		{
			compositorWorkspaceResourceManager.getFramebufferManager().clearRhiResources();
			compositorWorkspaceResourceManager.getRenderTargetTextureManager().clearRhiResources();
		}
		mFramebufferManagerInitialized = false;
	}

	void CompositorWorkspaceInstance::clearRenderQueueIndexRangesRenderableManagers()
	{
		// Forget about all previously gathered renderable managers
		for (RenderQueueIndexRange& renderQueueIndexRange : mRenderQueueIndexRanges)
		{
			renderQueueIndexRange.renderableManagers.clear();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
