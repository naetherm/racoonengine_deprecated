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
#include <RECore/Core/Manager.h>
#include <RECore/String/StringId.h>
#include "RERenderer/Core/Renderer/RenderTargetTextureSignature.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <vector>
PRAGMA_WARNING_POP
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class IRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;	///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class RenderTargetTextureManager final : private RECore::Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct RenderTargetTextureElement final
		{
			AssetId						 assetId;
			RenderTargetTextureSignature renderTargetTextureSignature;
			RERHI::RHITexture*				 texture;				///< Can be a null pointer, no "RERHI::RHITexturePtr" to not have overhead when internally reallocating
			RECore::uint32					 numberOfReferences;	///< Number of texture references (don't misuse the RHI texture reference counter for this)

			inline RenderTargetTextureElement() :
				assetId(RECore::getInvalid<AssetId>()),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline explicit RenderTargetTextureElement(const RenderTargetTextureSignature& _renderTargetTextureSignature) :
				assetId(RECore::getInvalid<AssetId>()),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline RenderTargetTextureElement(AssetId _assetId, const RenderTargetTextureSignature& _renderTargetTextureSignature) :
				assetId(_assetId),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(nullptr),
				numberOfReferences(0)
			{
				// Nothing here
			}

			inline RenderTargetTextureElement(AssetId _assetId, const RenderTargetTextureSignature& _renderTargetTextureSignature, RERHI::RHITexture& _texture) :
				assetId(_assetId),
				renderTargetTextureSignature(_renderTargetTextureSignature),
				texture(&_texture),
				numberOfReferences(0)
			{
				// Nothing here
			}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit RenderTargetTextureManager(IRenderer& renderer) :
			mRenderer(renderer)
		{
			// Nothing here
		}

		inline ~RenderTargetTextureManager()
		{
			// Nothing here
		}

		explicit RenderTargetTextureManager(const RenderTargetTextureManager&) = delete;
		RenderTargetTextureManager& operator=(const RenderTargetTextureManager&) = delete;

		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		void clear();
		void clearRhiResources();
		void addRenderTargetTexture(AssetId assetId, const RenderTargetTextureSignature& renderTargetTextureSignature);
		[[nodiscard]] RERHI::RHITexture* getTextureByAssetId(AssetId assetId, const RERHI::RHIRenderTarget& renderTarget, RECore::uint8 numberOfMultisamples, float resolutionScale, const RenderTargetTextureSignature** outRenderTargetTextureSignature);
		void releaseRenderTargetTextureBySignature(const RenderTargetTextureSignature& renderTargetTextureSignature);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<RenderTargetTextureElement>						 SortedRenderTargetTextureVector;
		typedef std::unordered_map<RECore::uint32, RenderTargetTextureSignatureId> AssetIdToRenderTargetTextureSignatureId;	///< Key = "RECore::AssetId"
		typedef std::unordered_map<RECore::uint32, RECore::uint32>						 AssetIdToIndex;							///< Key = "RECore::AssetId"


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&								mRenderer;
		SortedRenderTargetTextureVector			mSortedRenderTargetTextureVector;
		AssetIdToRenderTargetTextureSignatureId	mAssetIdToRenderTargetTextureSignatureId;
		AssetIdToIndex							mAssetIdToIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
