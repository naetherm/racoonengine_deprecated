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
#include "RERenderer/Resource/CompositorNode/Loader/CompositorNodeResourceLoader.h"
#include "RERenderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeResourceManager.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeResource.h"


// TODO(naetherm) Error handling


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void nodeTargetDeserialization(RECore::IFile& file, RERenderer::CompositorNodeResource& compositorNodeResource, const RERenderer::ICompositorPassFactory& compositorPassFactory)
		{
			// Read in the compositor node resource target
			RERenderer::v1CompositorNode::Target target;
			file.read(&target, sizeof(RERenderer::v1CompositorNode::Target));

			// Create the compositor node resource target instance
			RERenderer::CompositorTarget& compositorTarget = compositorNodeResource.addCompositorTarget(target.compositorChannelId, target.compositorFramebufferId);

			// Read in the compositor resource node target passes
			compositorTarget.setNumberOfCompositorResourcePasses(target.numberOfPasses);
			for (RECore::uint32 i = 0; i < target.numberOfPasses; ++i)
			{
				// Read the pass header
				RERenderer::v1CompositorNode::PassHeader passHeader;
				file.read(&passHeader, sizeof(RERenderer::v1CompositorNode::PassHeader));

				// Create the compositor resource pass
				RERenderer::ICompositorResourcePass* compositorResourcePass = compositorTarget.addCompositorResourcePass(compositorPassFactory, passHeader.compositorPassTypeId);

				// Deserialize the compositor resource pass
				if (nullptr != compositorResourcePass && 0 != passHeader.numberOfBytes)
				{
					// Load in the compositor resource pass data
					// TODO(naetherm) Get rid of the new/delete in here
					RECore::uint8* data = new RECore::uint8[passHeader.numberOfBytes];
					file.read(data, passHeader.numberOfBytes);

					// Deserialize the compositor resource pass
					compositorResourcePass->deserialize(passHeader.numberOfBytes, data);

					// Cleanup
					delete [] data;
				}
				else
				{
					// TODO(naetherm) Error handling
				}
			}
		}

		void nodeDeserialization(RECore::IFile& file, const RERenderer::v1CompositorNode::CompositorNodeHeader& compositorNodeHeader, RERenderer::CompositorNodeResource& compositorNodeResource, const RERenderer::ICompositorPassFactory& compositorPassFactory)
		{
			// Read in the compositor resource node input channels
			// TODO(naetherm) Read all input channels in a single burst? (need to introduce a maximum number of input channels for this)
			compositorNodeResource.reserveInputChannels(compositorNodeHeader.numberOfInputChannels);
			for (RECore::uint32 i = 0; i < compositorNodeHeader.numberOfInputChannels; ++i)
			{
				RERenderer::CompositorChannelId channelId;
				file.read(&channelId, sizeof(RERenderer::CompositorChannelId));
				compositorNodeResource.addInputChannel(channelId);
			}

			// TODO(naetherm) Read all render target textures in a single burst?
			compositorNodeResource.reserveRenderTargetTextures(compositorNodeHeader.numberOfRenderTargetTextures);
			for (RECore::uint32 i = 0; i < compositorNodeHeader.numberOfRenderTargetTextures; ++i)
			{
				RERenderer::v1CompositorNode::RenderTargetTexture renderTargetTexture;
				file.read(&renderTargetTexture, sizeof(RERenderer::v1CompositorNode::RenderTargetTexture));
				compositorNodeResource.addRenderTargetTexture(renderTargetTexture.assetId, renderTargetTexture.renderTargetTextureSignature);
			}

			// TODO(naetherm) Read all framebuffers in a single burst?
			compositorNodeResource.reserveFramebuffers(compositorNodeHeader.numberOfFramebuffers);
			for (RECore::uint32 i = 0; i < compositorNodeHeader.numberOfFramebuffers; ++i)
			{
				RERenderer::v1CompositorNode::Framebuffer framebuffer;
				file.read(&framebuffer, sizeof(RERenderer::v1CompositorNode::Framebuffer));
				compositorNodeResource.addFramebuffer(framebuffer.compositorFramebufferId, framebuffer.framebufferSignature);
			}

			// Read in the compositor node resource targets
			compositorNodeResource.reserveCompositorTargets(compositorNodeHeader.numberOfTargets);
			for (RECore::uint32 i = 0; i < compositorNodeHeader.numberOfTargets; ++i)
			{
				nodeTargetDeserialization(file, compositorNodeResource, compositorPassFactory);
			}

			// Read in the compositor resource node output channels
			// TODO(naetherm) Read all output channels in a single burst? (need to introduce a maximum number of output channels for this)
			compositorNodeResource.reserveOutputChannels(compositorNodeHeader.numberOfOutputChannels);
			for (RECore::uint32 i = 0; i < compositorNodeHeader.numberOfOutputChannels; ++i)
			{
				RERenderer::CompositorChannelId channelId;
				file.read(&channelId, sizeof(RERenderer::CompositorChannelId));
				compositorNodeResource.addOutputChannel(channelId);
			}
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
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void CompositorNodeResourceLoader::initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mCompositorNodeResource = static_cast<CompositorNodeResource*>(&resource);
	}

	bool CompositorNodeResourceLoader::onDeserialization(RECore::IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1CompositorNode::FORMAT_TYPE, v1CompositorNode::FORMAT_VERSION, file);
	}

	void CompositorNodeResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the compositor node header
		v1CompositorNode::CompositorNodeHeader compositorNodeHeader;
		mMemoryFile.read(&compositorNodeHeader, sizeof(v1CompositorNode::CompositorNodeHeader));

		// Read in the compositor node resource
		::detail::nodeDeserialization(mMemoryFile, compositorNodeHeader, *mCompositorNodeResource, static_cast<CompositorNodeResourceManager&>(getResourceManager()).getCompositorPassFactory());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
