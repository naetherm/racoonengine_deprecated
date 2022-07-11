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
#include "RERenderer/Resource/Texture/Loader/CrnTextureResourceLoader.h"
#include <RECore/File/MemoryFile.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CrnArrayTextureResourceLoader final : public CrnTextureResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("crn_array");


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual RECore::ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] virtual bool onDeserialization(RECore::IFile& file) override;
		virtual void onProcessing() override;


	//[-------------------------------------------------------]
	//[ Protected RERenderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	protected:
		[[nodiscard]] virtual RERHI::RHITexture* createRhiTexture() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CrnArrayTextureResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer) :
			CrnTextureResourceLoader(resourceManager, renderer),
			mNumberOfSlices(0)
		{
			// Nothing here
		}

		inline virtual ~CrnArrayTextureResourceLoader() override
		{
			// Nothing here
		}

		explicit CrnArrayTextureResourceLoader(const CrnArrayTextureResourceLoader&) = delete;
		CrnArrayTextureResourceLoader& operator=(const CrnArrayTextureResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct SliceFileMetadata final
		{
			const RECore::Asset& asset;
			RECore::uint32	 offset;
			RECore::uint32	 numberOfBytes;

			inline SliceFileMetadata(const RECore::Asset& _asset, RECore::uint32 _offset, RECore::uint32 _numberOfBytes) :
				asset(_asset),
				offset(_offset),
				numberOfBytes(_numberOfBytes)
			{};
			SliceFileMetadata& operator =(const SliceFileMetadata& sliceFile) = delete;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temporary data
		RECore::uint32 mNumberOfSlices;

		// Temporary file data
		RECore::MemoryFile					   mMemoryFile;
		std::vector<RECore::AssetId>		   mAssetIds;
		std::vector<SliceFileMetadata> mSliceFileMetadata;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
