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
#include "RERenderer/Resource/Texture/Loader/ITextureResourceLoader.h"
#include <RECore/File/MemoryFile.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class Lz4DdsTextureResourceLoader final : public ITextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID		 = STRING_ID("lz4dds");
		static constexpr RECore::uint32 FORMAT_TYPE	 = TYPE_ID;
		static constexpr RECore::uint32 FORMAT_VERSION = 1;


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual RECore::ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] inline virtual bool onDeserialization(RECore::IFile& file) override
		{
			// Tell the memory mapped file about the LZ4 compressed data
			return mMemoryFile.loadLz4CompressedDataFromFile(FORMAT_TYPE, FORMAT_VERSION, file);
		}

		virtual void onProcessing() override;


	//[-------------------------------------------------------]
	//[ Protected RERenderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual RERHI::RHITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline Lz4DdsTextureResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer) :
			ITextureResourceLoader(resourceManager, renderer),
			mWidth(0),
			mHeight(0),
			mDepth(0),
			mNumberOfSlices(0),
			mTextureFormat(0),
			mDataContainsMipmaps(false),
			mNumberOfImageDataBytes(0),
			mNumberOfUsedImageDataBytes(0),
			mImageData(nullptr)
		{
			// Nothing here
		}

		inline virtual ~Lz4DdsTextureResourceLoader() override
		{
			delete [] mImageData;
		}

		explicit Lz4DdsTextureResourceLoader(const Lz4DdsTextureResourceLoader&) = delete;
		Lz4DdsTextureResourceLoader& operator=(const Lz4DdsTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		RECore::MemoryFile mMemoryFile;
		RECore::uint32   mWidth;
		RECore::uint32   mHeight;
		RECore::uint32   mDepth;
		RECore::uint32   mNumberOfSlices;
		RECore::uint8    mTextureFormat;	// "RERHI::TextureFormat", don't want to include the header in here
		bool	   mDataContainsMipmaps;

		// Temporary image data
		RECore::uint32 mNumberOfImageDataBytes;
		RECore::uint32 mNumberOfUsedImageDataBytes;
		RECore::uint8* mImageData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
