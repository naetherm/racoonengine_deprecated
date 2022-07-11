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
#include "RERenderer/Core/Renderer/FramebufferManager.h"
#include "RERenderer/Core/Renderer/RenderPassManager.h"
#include "RERenderer/Core/Renderer/RenderTargetTextureManager.h"
#include "RERenderer/IRenderer.h"

#include <algorithm>


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
		[[nodiscard]] inline bool orderFramebufferElementByFramebufferSignatureId(const RERenderer::FramebufferManager::FramebufferElement& left, const RERenderer::FramebufferManager::FramebufferElement& right)
		{
			return (left.framebufferSignature.getFramebufferSignatureId() < right.framebufferSignature.getFramebufferSignatureId());
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
	void FramebufferManager::clear()
	{
		clearRhiResources();
		mSortedFramebufferVector.clear();
		mCompositorFramebufferIdToFramebufferSignatureId.clear();
	}

	void FramebufferManager::clearRhiResources()
	{
		for (FramebufferElement& framebufferElement : mSortedFramebufferVector)
		{
			if (nullptr != framebufferElement.framebuffer)
			{
				framebufferElement.framebuffer->Release();
				framebufferElement.framebuffer = nullptr;
			}
		}
	}

	void FramebufferManager::addFramebuffer(CompositorFramebufferId compositorFramebufferId, const FramebufferSignature& framebufferSignature)
	{
		FramebufferElement framebufferElement(framebufferSignature);
		SortedFramebufferVector::iterator iterator = std::lower_bound(mSortedFramebufferVector.begin(), mSortedFramebufferVector.end(), framebufferElement, ::detail::orderFramebufferElementByFramebufferSignatureId);
		if (iterator == mSortedFramebufferVector.end() || iterator->framebufferSignature.getFramebufferSignatureId() != framebufferElement.framebufferSignature.getFramebufferSignatureId())
		{
			// Add new framebuffer

			// Register the new framebuffer element
			++framebufferElement.numberOfReferences;
			mSortedFramebufferVector.insert(iterator, framebufferElement);
		}
		else
		{
			// Just increase the number of references
			++iterator->numberOfReferences;
		}
		mCompositorFramebufferIdToFramebufferSignatureId.emplace(compositorFramebufferId, framebufferSignature.getFramebufferSignatureId());
	}

	RERHI::RHIFramebuffer* FramebufferManager::getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId) const
	{
		RERHI::RHIFramebuffer* framebuffer = nullptr;

		// Map compositor framebuffer ID to framebuffer signature ID
		CompositorFramebufferIdToFramebufferSignatureId::const_iterator iterator = mCompositorFramebufferIdToFramebufferSignatureId.find(compositorFramebufferId);
		if (mCompositorFramebufferIdToFramebufferSignatureId.cend() != iterator)
		{
			// TODO(naetherm) Is there need for a more efficient search?
			const FramebufferSignatureId framebufferSignatureId = iterator->second;
			for (const FramebufferElement& framebufferElement : mSortedFramebufferVector)
			{
				const FramebufferSignature& framebufferSignature = framebufferElement.framebufferSignature;
				if (framebufferSignature.getFramebufferSignatureId() == framebufferSignatureId)
				{
					framebuffer = framebufferElement.framebuffer;
					break;
				}
			}
			ASSERT(nullptr != framebuffer, "Invalid framebuffer")
		}
		else
		{
			// Error!
			ASSERT(false, "Unknown compositor framebuffer ID, this shouldn't have happened")
		}

		// Done
		return framebuffer;
	}

	RERHI::RHIFramebuffer* FramebufferManager::getFramebufferByCompositorFramebufferId(CompositorFramebufferId compositorFramebufferId, const RERHI::RHIRenderTarget& renderTarget, RECore::uint8 numberOfMultisamples, float resolutionScale)
	{
		RERHI::RHIFramebuffer* framebuffer = nullptr;

		// Map compositor framebuffer ID to framebuffer signature ID
		CompositorFramebufferIdToFramebufferSignatureId::const_iterator iterator = mCompositorFramebufferIdToFramebufferSignatureId.find(compositorFramebufferId);
		if (mCompositorFramebufferIdToFramebufferSignatureId.cend() != iterator)
		{
			// TODO(naetherm) Is there need for a more efficient search?
			const FramebufferSignatureId framebufferSignatureId = iterator->second;
			for (FramebufferElement& framebufferElement : mSortedFramebufferVector)
			{
				const FramebufferSignature& framebufferSignature = framebufferElement.framebufferSignature;
				if (framebufferSignature.getFramebufferSignatureId() == framebufferSignatureId)
				{
					// Do we need to create the RHI framebuffer instance right now?
					if (nullptr == framebufferElement.framebuffer)
					{
						// Get the color texture instances
						RERHI::TextureFormat::Enum colorTextureFormats[8] = { RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN, RERHI::TextureFormat::Enum::UNKNOWN };
						const RECore::uint8 numberOfColorAttachments = framebufferSignature.getNumberOfColorAttachments();
						ASSERT(numberOfColorAttachments < 8, "Invalid number of color attachments")
						RERHI::FramebufferAttachment colorFramebufferAttachments[8];
						RECore::uint8 usedNumberOfMultisamples = 0;
						for (RECore::uint8 i = 0; i < numberOfColorAttachments; ++i)
						{
							const FramebufferSignatureAttachment& framebufferSignatureAttachment = framebufferSignature.getColorFramebufferSignatureAttachment(i);
							const AssetId colorTextureAssetId = framebufferSignatureAttachment.textureAssetId;
							const RenderTargetTextureSignature* colorRenderTargetTextureSignature = nullptr;
							RERHI::FramebufferAttachment& framebufferAttachment = colorFramebufferAttachments[i];
							framebufferAttachment.texture = RECore::isValid(colorTextureAssetId) ? mRenderTargetTextureManager.getTextureByAssetId(colorTextureAssetId, renderTarget, numberOfMultisamples, resolutionScale, &colorRenderTargetTextureSignature) : nullptr;
							ASSERT(nullptr != framebufferAttachment.texture, "Invalid framebuffer attachment texture")
							framebufferAttachment.mipmapIndex = framebufferSignatureAttachment.mipmapIndex;
							framebufferAttachment.layerIndex = framebufferSignatureAttachment.layerIndex;
							ASSERT(nullptr != colorRenderTargetTextureSignature, "Invalid color render target texture signature")
							if (0 == usedNumberOfMultisamples)
							{
								usedNumberOfMultisamples = ((colorRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0) ? numberOfMultisamples : 1u;
							}
							else
							{
								ASSERT(1 == usedNumberOfMultisamples || ((colorRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0), "Invalid number of multisamples")
							}
							colorTextureFormats[i] = colorRenderTargetTextureSignature->getTextureFormat();
						}

						// Get the depth stencil texture instances
						const FramebufferSignatureAttachment& depthStencilFramebufferSignatureAttachment = framebufferSignature.getDepthStencilFramebufferSignatureAttachment();
						const RenderTargetTextureSignature* depthStencilRenderTargetTextureSignature = nullptr;
						RERHI::FramebufferAttachment depthStencilFramebufferAttachment(RECore::isValid(depthStencilFramebufferSignatureAttachment.textureAssetId) ? mRenderTargetTextureManager.getTextureByAssetId(depthStencilFramebufferSignatureAttachment.textureAssetId, renderTarget, numberOfMultisamples, resolutionScale, &depthStencilRenderTargetTextureSignature) : nullptr, depthStencilFramebufferSignatureAttachment.mipmapIndex, depthStencilFramebufferSignatureAttachment.layerIndex);
						if (nullptr != depthStencilRenderTargetTextureSignature)
						{
							if (0 == usedNumberOfMultisamples)
							{
								usedNumberOfMultisamples = ((depthStencilRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0) ? numberOfMultisamples : 1u;
							}
							else
							{
								ASSERT(1 == usedNumberOfMultisamples || ((depthStencilRenderTargetTextureSignature->getFlags() & RenderTargetTextureSignature::Flag::ALLOW_MULTISAMPLE) != 0), "Invalid number of multisamples")
							}
						}
						const RERHI::TextureFormat::Enum depthStencilTextureFormat = (nullptr != depthStencilRenderTargetTextureSignature) ? depthStencilRenderTargetTextureSignature->getTextureFormat() : RERHI::TextureFormat::Enum::UNKNOWN;

						// Get or create the managed render pass
						RERHI::RHIRenderPass* renderPass = mRenderPassManager.getOrCreateRenderPass(numberOfColorAttachments, colorTextureFormats, depthStencilTextureFormat, usedNumberOfMultisamples);
						ASSERT(nullptr != renderPass, "Invalid render pass")

						// Create the framebuffer object (FBO) instance
						// -> The framebuffer automatically adds a reference to the provided textures
						framebufferElement.framebuffer = mRenderTargetTextureManager.getRenderer().getRhi().createFramebuffer(*renderPass, colorFramebufferAttachments, ((nullptr != depthStencilFramebufferAttachment.texture) ? &depthStencilFramebufferAttachment : nullptr) RHI_RESOURCE_DEBUG_NAME("Framebuffer manager"));
						framebufferElement.framebuffer->AddReference();
					}
					framebuffer = framebufferElement.framebuffer;
					break;
				}
			}
			ASSERT(nullptr != framebuffer, "Invalid framebuffer")
		}
		else
		{
			// Error!
			ASSERT(false, "Unknown compositor framebuffer ID, this shouldn't have happened.")
		}

		// Done
		return framebuffer;
	}

	void FramebufferManager::releaseFramebufferBySignature(const FramebufferSignature& framebufferSignature)
	{
		const FramebufferElement framebufferElement(framebufferSignature);
		SortedFramebufferVector::iterator iterator = std::lower_bound(mSortedFramebufferVector.begin(), mSortedFramebufferVector.end(), framebufferElement, ::detail::orderFramebufferElementByFramebufferSignatureId);
		if (iterator != mSortedFramebufferVector.end() && iterator->framebufferSignature.getFramebufferSignatureId() == framebufferElement.framebufferSignature.getFramebufferSignatureId())
		{
			// Was this the last reference?
			if (1 == iterator->numberOfReferences)
			{
				if (nullptr != iterator->framebuffer)
				{
					iterator->framebuffer->Release();
				}
				mSortedFramebufferVector.erase(iterator);
			}
			else
			{
				--iterator->numberOfReferences;
			}
		}
		else
		{
			// Error!
			ASSERT(false, "Framebuffer signature isn't registered")
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
