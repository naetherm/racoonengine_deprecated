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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/Resource/IResource.h>
#include "RERenderer/Resource/CompositorNode/CompositorTarget.h"
#include "RERenderer/Resource/CompositorNode/CompositorFramebuffer.h"
#include "RERenderer/Resource/CompositorNode/CompositorRenderTargetTexture.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
template<class ELEMENT_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS>
class PackedElementManager;

template<class TYPE, class LOADER_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS>
class ResourceManagerTemplate;
}
namespace RERenderer {
	class CompositorNodeResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 CompositorNodeResourceId;	///< POD compositor node resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorNodeResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorNodeResourceLoader;
		friend RECore::PackedElementManager<CompositorNodeResource, CompositorNodeResourceId, 32>;									// Type definition of template class
		friend RECore::ResourceManagerTemplate<CompositorNodeResource, CompositorNodeResourceLoader, CompositorNodeResourceId, 32>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<CompositorChannelId>		   CompositorChannels;	// TODO(naetherm) Get rid of "std::vector" and dynamic memory handling in here? (need to introduce a maximum number of input channels for this)
		typedef std::vector<CompositorRenderTargetTexture> CompositorRenderTargetTextures;
		typedef std::vector<CompositorFramebuffer>		   CompositorFramebuffers;
		typedef std::vector<CompositorTarget>			   CompositorTargets;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// TODO(naetherm) Asynchronous loading completion, we might want to move this into "RECore::IResource"
		void enforceFullyLoaded();

		//[-------------------------------------------------------]
		//[ Input channels                                        ]
		//[-------------------------------------------------------]
		inline void reserveInputChannels(RECore::uint32 numberOfInputChannels)
		{
			mInputChannels.reserve(numberOfInputChannels);
		}

		inline void addInputChannel(CompositorChannelId compositorChannelId)
		{
			mInputChannels.push_back(compositorChannelId);
		}

		[[nodiscard]] inline const CompositorChannels& getInputChannels() const
		{
			return mInputChannels;
		}

		//[-------------------------------------------------------]
		//[ Render target textures                                ]
		//[-------------------------------------------------------]
		inline void reserveRenderTargetTextures(RECore::uint32 numberOfRenderTargetTextures)
		{
			mCompositorRenderTargetTextures.reserve(numberOfRenderTargetTextures);
		}

		inline void addRenderTargetTexture(AssetId assetId, const RenderTargetTextureSignature& renderTargetTextureSignature)
		{
			mCompositorRenderTargetTextures.emplace_back(assetId, renderTargetTextureSignature);
		}

		[[nodiscard]] inline const CompositorRenderTargetTextures& getRenderTargetTextures() const
		{
			return mCompositorRenderTargetTextures;
		}

		//[-------------------------------------------------------]
		//[ Framebuffers                                          ]
		//[-------------------------------------------------------]
		inline void reserveFramebuffers(RECore::uint32 numberOfFramebuffers)
		{
			mCompositorFramebuffers.reserve(numberOfFramebuffers);
		}

		inline void addFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature)
		{
			mCompositorFramebuffers.emplace_back(compositorFramebufferId, framebufferSignature);
		}

		[[nodiscard]] inline const CompositorFramebuffers& getFramebuffers() const
		{
			return mCompositorFramebuffers;
		}

		//[-------------------------------------------------------]
		//[ Targets                                               ]
		//[-------------------------------------------------------]
		inline void reserveCompositorTargets(RECore::uint32 numberOfCompositorTargets)
		{
			mCompositorTargets.reserve(numberOfCompositorTargets);
		}

		[[nodiscard]] inline CompositorTarget& addCompositorTarget(CompositorChannelId compositorChannelId, CompositorFramebufferId compositorFramebufferId)
		{
			mCompositorTargets.emplace_back(compositorChannelId, compositorFramebufferId);
			return mCompositorTargets.back();
		}

		[[nodiscard]] inline const CompositorTargets& getCompositorTargets() const
		{
			return mCompositorTargets;
		}

		//[-------------------------------------------------------]
		//[ Output channels                                       ]
		//[-------------------------------------------------------]
		inline void reserveOutputChannels(RECore::uint32 numberOfOutputChannels)
		{
			mOutputChannels.reserve(numberOfOutputChannels);
		}

		inline void addOutputChannel(CompositorChannelId compositorChannelId)
		{
			mOutputChannels.push_back(compositorChannelId);
		}

		[[nodiscard]] inline const CompositorChannels& getOutputChannels() const
		{
			return mOutputChannels;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CompositorNodeResource()
		{
			// Nothing here
		}

		inline virtual ~CompositorNodeResource() override
		{
			// Sanity checks
			ASSERT(mInputChannels.empty(), "Invalid input channels")
			ASSERT(mCompositorRenderTargetTextures.empty(), "Invalid compositor render target textures")
			ASSERT(mCompositorFramebuffers.empty(), "Invalid compositor framebuffers")
			ASSERT(mCompositorTargets.empty(), "Invalid compositor targets")
			ASSERT(mOutputChannels.empty(), "Invalid output channels")
		}

		explicit CompositorNodeResource(const CompositorNodeResource&) = delete;
		CompositorNodeResource& operator=(const CompositorNodeResource&) = delete;

		//[-------------------------------------------------------]
		//[ "RERenderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(CompositorNodeResourceId compositorNodeResourceId)
		{
			// Sanity checks
			ASSERT(mInputChannels.empty(), "Invalid input channels")
			ASSERT(mCompositorRenderTargetTextures.empty(), "Invalid compositor render target textures")
			ASSERT(mCompositorFramebuffers.empty(), "Invalid compositor framebuffers")
			ASSERT(mCompositorTargets.empty(), "Invalid compositor targets")
			ASSERT(mOutputChannels.empty(), "Invalid output channels")

			// Call base implementation
			IResource::initializeElement(compositorNodeResourceId);
		}

		void deinitializeElement();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorChannels			   mInputChannels;
		CompositorRenderTargetTextures mCompositorRenderTargetTextures;
		CompositorFramebuffers		   mCompositorFramebuffers;
		CompositorTargets			   mCompositorTargets;
		CompositorChannels			   mOutputChannels;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
