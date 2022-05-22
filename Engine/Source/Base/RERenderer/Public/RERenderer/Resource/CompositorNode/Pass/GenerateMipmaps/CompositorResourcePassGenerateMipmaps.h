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
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;				///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::StringId MaterialPropertyId;	///< Material property identifier, internally just a POD "RECore::uint32", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassGenerateMipmaps final : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("GenerateMipmaps");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline AssetId getTextureAssetId() const
		{
			return mTextureAssetId;
		}

		[[nodiscard]] inline AssetId getMaterialBlueprintAssetId() const
		{
			return mMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline MaterialPropertyId getTextureMaterialBlueprintProperty() const
		{
			return mTextureMaterialBlueprintProperty;
		}


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual CompositorPassTypeId getTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void deserialize(RECore::uint32 numberOfBytes, const RECore::uint8* data) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit CompositorResourcePassGenerateMipmaps(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassGenerateMipmaps() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassGenerateMipmaps(const CompositorResourcePassGenerateMipmaps&) = delete;
		CompositorResourcePassGenerateMipmaps& operator=(const CompositorResourcePassGenerateMipmaps&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId			   mTextureAssetId;
		AssetId			   mMaterialBlueprintAssetId;
		MaterialPropertyId mTextureMaterialBlueprintProperty;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
