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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/String/StringId.h>
#include <RECore/Resource/IResourceListener.h>
#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class LightSceneItem;
	class CameraSceneItem;
	class IRenderer;
	class ISceneItem;
	class RenderableManager;
	class CompositorNodeInstance;
	class ICompositorInstancePass;
	class CompositorInstancePassShadowMap;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;						///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::uint32 CompositorWorkspaceResourceId;	///< POD compositor workspace resource identifier
	typedef RECore::StringId CompositorPassTypeId;			///< Compositor pass type identifier, internally just a POD "RECore::uint32"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compositor workspace instance
	*
	*  @remarks
	*    Compositors can get quite complex with a lot of individual compositor passes which several of them rendering portions of the scene.
	*    We really only want to perform expensive culling once for a compositor workspace frame rendering. Some renderable managers might never
	*    get rendered because none of the renderables is inside a render queue index range touched by the compositor passes. As a result,
	*    an compositor workspace instance keeps a list of render queue index ranges covered by the compositor instance passes. Before compositor
	*    instance passes are executed, a culling step is performed gathering all renderable managers which should currently be taken into account
	*    during rendering. The result of this culling step is that each render queue index range has renderable managers to consider assigned to them.
	*    Executed compositor instances passes only access this prepared render queue index information to fill their render queues.
	*/
	class CompositorWorkspaceInstance final : protected RECore::IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<RenderableManager*> RenderableManagers;
		typedef std::vector<CompositorNodeInstance*> CompositorNodeInstances;

		struct RenderQueueIndexRange final
		{
			RECore::uint8			   minimumRenderQueueIndex;	///< Fixed during runtime
			RECore::uint8			   maximumRenderQueueIndex;	///< Fixed during runtime
			RenderableManagers renderableManagers;		///< Dynamic during runtime

			inline RenderQueueIndexRange(RECore::uint8 _minimumRenderQueueIndex, RECore::uint8 _maximumRenderQueueIndex) :
				minimumRenderQueueIndex(_minimumRenderQueueIndex),
				maximumRenderQueueIndex(_maximumRenderQueueIndex)
			{}
		};
		typedef std::vector<RenderQueueIndexRange> RenderQueueIndexRanges;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		CompositorWorkspaceInstance(IRenderer& renderer, AssetId compositorWorkspaceAssetId);
		virtual ~CompositorWorkspaceInstance() override;

		[[nodiscard]] inline const IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline RECore::uint8 getNumberOfMultisamples() const
		{
			return mNumberOfMultisamples;
		}

		void setNumberOfMultisamples(RECore::uint8 numberOfMultisamples);	// The number of multisamples per pixel (valid values: 1, 2, 4, 8); Changes are considered to be expensive since internal RHI resources might need to be updated when rendering the next time

		[[nodiscard]] inline float getResolutionScale() const
		{
			return mResolutionScale;
		}

		inline void setResolutionScale(float resolutionScale)	// Changes are considered to be expensive since internal RHI resources might need to be updated when rendering the next time
		{
			mResolutionScale = resolutionScale;
		}

		[[nodiscard]] inline const RenderQueueIndexRanges& getRenderQueueIndexRanges() const	// Renderable manager pointers are only considered to be safe directly after the "Renderer::CompositorWorkspaceInstance::execute()" call
		{
			return mRenderQueueIndexRanges;
		}

		[[nodiscard]] const RenderQueueIndexRange* getRenderQueueIndexRangeByRenderQueueIndex(RECore::uint8 renderQueueIndex) const;	// Can be a null pointer, don't destroy the instance
		[[nodiscard]] const ICompositorInstancePass* getFirstCompositorInstancePassByCompositorPassTypeId(CompositorPassTypeId compositorPassTypeId) const;
		void executeVr(RERHI::RHIRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem);	// If "Renderer::IVrManager::isRunning()" is true, virtual reality rendering is used, don't use this method if you want to render e.g. into a texture for other purposes
		void execute(RERHI::RHIRenderTarget& renderTarget, const CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem, bool singlePassStereoInstancing = false);

		[[nodiscard]] inline const CompositorNodeInstances& getSequentialCompositorNodeInstances() const
		{
			return mSequentialCompositorNodeInstances;
		}

		[[nodiscard]] inline RERHI::RHIRenderTarget* getExecutionRenderTarget() const	// Only valid during compositor workspace instance execution
		{
			return mExecutionRenderTarget;
		}

		[[nodiscard]] inline const RERHI::RHICommandBuffer& getCommandBuffer() const
		{
			return mCommandBuffer;
		}

		#ifdef RHI_STATISTICS
			[[nodiscard]] inline const RERHI::PipelineStatisticsQueryResult& getPipelineStatisticsQueryResult() const
			{
				return mPipelineStatisticsQueryResult;
			}
		#endif


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const RECore::IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		CompositorWorkspaceInstance() = delete;
		explicit CompositorWorkspaceInstance(const CompositorWorkspaceInstance&) = delete;
		CompositorWorkspaceInstance& operator=(const CompositorWorkspaceInstance&) = delete;
		void destroySequentialCompositorNodeInstances();
		void createFramebuffersAndRenderTargetTextures(const RERHI::RHIRenderTarget& mainRenderTarget);
		void destroyFramebuffersAndRenderTargetTextures(bool clearManagers = false);
		void clearRenderQueueIndexRangesRenderableManagers();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&						 mRenderer;
		RECore::uint8							 mNumberOfMultisamples;
		RECore::uint8							 mCurrentlyUsedNumberOfMultisamples;
		float							 mResolutionScale;
		RECore::uint32						 mRenderTargetWidth;
		RECore::uint32						 mRenderTargetHeight;
		CompositorWorkspaceResourceId	 mCompositorWorkspaceResourceId;
		CompositorNodeInstances			 mSequentialCompositorNodeInstances;	///< We're responsible to destroy the compositor node instances if we no longer need them
		bool							 mFramebufferManagerInitialized;
		RenderQueueIndexRanges			 mRenderQueueIndexRanges;				///< The render queue index ranges layout is fixed during runtime

		// The rest is temporary "CompositorWorkspaceInstance::execute()" data to e.g. avoid reallocations
		RERHI::RHIRenderTarget*				 mExecutionRenderTarget;				///< Only valid during compositor workspace instance execution
		std::vector<ISceneItem*>		 mExecuteOnRenderingSceneItems;			///< Scene items which requested an execute call on rendering, no duplicates allowed
		RERHI::RHICommandBuffer				 mCommandBuffer;						///< RHI command buffer
		CompositorInstancePassShadowMap* mCompositorInstancePassShadowMap;		///< Can be a null pointer, don't destroy the instance
		#ifdef RHI_STATISTICS
			RERHI::RHIQueryPoolPtr				   mPipelineStatisticsQueryPoolPtr;					///< Double buffered asynchronous pipeline statistics query pool, can be a null pointer
			RECore::uint32						   mPreviousCurrentPipelineStatisticsQueryIndex;	///< Can be "RECore::getInvalid<RECore::uint32>()"
			RECore::uint32						   mCurrentPipelineStatisticsQueryIndex;			///< Toggles between 0 or 1
			RERHI::PipelineStatisticsQueryResult mPipelineStatisticsQueryResult;					///< Due to double buffered asynchronous pipeline statistics query pool, this is the pipeline statistics query result of the previous frame
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
