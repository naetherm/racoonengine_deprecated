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
#include "RERenderer/Core/Renderer/RenderTargetTextureSignature.h"
#include <RECore/Math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RenderTargetTextureSignature::RenderTargetTextureSignature(RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat, RECore::uint8 flags, float widthScale, float heightScale) :
		mWidth(width),
		mHeight(height),
		mTextureFormat(textureFormat),
		mFlags(flags),
		mWidthScale(widthScale),
		mHeightScale(heightScale),
		mRenderTargetTextureSignatureId(RECore::Math::FNV1a_INITIAL_HASH_32)
	{
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mWidth), sizeof(RECore::uint32), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mHeight), sizeof(RECore::uint32), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mTextureFormat), sizeof(RERHI::TextureFormat::Enum), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mFlags), sizeof(RECore::uint8), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mWidthScale), sizeof(float), mRenderTargetTextureSignatureId);
		mRenderTargetTextureSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mHeightScale), sizeof(float), mRenderTargetTextureSignatureId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
