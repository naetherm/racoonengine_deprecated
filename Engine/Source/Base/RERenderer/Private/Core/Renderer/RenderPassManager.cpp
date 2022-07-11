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
#include "RERenderer/Core/Renderer/RenderPassManager.h"
#include <RECore/Math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RenderPassManager::~RenderPassManager()
	{
		for (auto& renderPassPair : mRenderPasses)
		{
			renderPassPair.second->Release();
		}
	}

	RERHI::RHIRenderPass* RenderPassManager::getOrCreateRenderPass(RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::uint8 numberOfMultisamples)
	{
		// Calculate the render pass signature
		// TODO(naetherm) Tiny performance optimization: It should be possible to pre-calculate a partial render pass signature using "numberOfColorAttachments", "colorAttachmentTextureFormats" and "depthStencilAttachmentTextureFormat" inside the renderer toolkit for the normal use-cases
		RECore::uint32 renderPassSignature = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&numberOfColorAttachments), sizeof(RECore::uint32), RECore::Math::FNV1a_INITIAL_HASH_32);
		for (RECore::uint32 i = 0; i < numberOfColorAttachments; ++i)
		{
			renderPassSignature = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&colorAttachmentTextureFormats[i]), sizeof(RERHI::TextureFormat::Enum), renderPassSignature);
		}
		renderPassSignature = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&depthStencilAttachmentTextureFormat), sizeof(RERHI::TextureFormat::Enum), renderPassSignature);
		renderPassSignature = RECore::Math::calculateFNV1a32(&numberOfMultisamples, sizeof(RECore::uint8), renderPassSignature);

		// Does the render pass instance already exist?
		const RenderPasses::const_iterator iterator = mRenderPasses.find(renderPassSignature);
		if (mRenderPasses.cend() != iterator)
		{
			// The render pass instance already exists
			return iterator->second;
		}
		else
		{
			// Create the render pass instance
			RERHI::RHIRenderPass* renderPass = mRhi.createRenderPass(numberOfColorAttachments, colorAttachmentTextureFormats, depthStencilAttachmentTextureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_NAME("Render pass manager"));
			renderPass->AddReference();
			mRenderPasses.emplace(renderPassSignature, renderPass);
			return renderPass;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
