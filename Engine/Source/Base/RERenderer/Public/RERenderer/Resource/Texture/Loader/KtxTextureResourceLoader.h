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
#include "RERenderer/Resource/Texture/Loader/ITextureResourceLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class KtxTextureResourceLoader final : public ITextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t TYPE_ID = STRING_ID("ktx");


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual RECore::ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] virtual bool onDeserialization(RECore::IFile& file) override;

		inline virtual void onProcessing() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Protected RERenderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual RERHI::RHITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline KtxTextureResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer) :
			ITextureResourceLoader(resourceManager, renderer),
			mWidth(0),
			mHeight(0),
			mTextureFormat(0),
			mCubeMap(false),
			mDataContainsMipmaps(false),
			mNumberOfImageDataBytes(0),
			mNumberOfUsedImageDataBytes(0),
			mImageData(nullptr)
		{
			// Nothing here
		}

		inline virtual ~KtxTextureResourceLoader() override
		{
			delete [] mImageData;
		}

		explicit KtxTextureResourceLoader(const KtxTextureResourceLoader&) = delete;
		KtxTextureResourceLoader& operator=(const KtxTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		uint32_t mWidth;
		uint32_t mHeight;
		uint8_t  mTextureFormat;	// "RERHI::TextureFormat", don't want to include the header in here
		bool	 mCubeMap;
		bool	 mDataContainsMipmaps;

		// Temporary image data
		uint32_t mNumberOfImageDataBytes;
		uint32_t mNumberOfUsedImageDataBytes;
		uint8_t* mImageData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
