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
#include <RECore/Resource/IResource.h>
#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer {
class TextureResource;
}
namespace RECore {
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t TextureResourceId;	///< POD texture resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Texture resource class
	*/
	class TextureResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TextureResourceManager;
		friend class ITextureResourceLoader;
		friend RECore::PackedElementManager<TextureResource, TextureResourceId, 2048>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline bool isRgbHardwareGammaCorrection() const
		{
			return mRgbHardwareGammaCorrection;
		}

		[[nodiscard]] inline const RERHI::RHITexturePtr& getTexturePtr() const
		{
			return mTexture;
		}

		inline void setTexture(RERHI::RHITexture* texture)
		{
			// Sanity check
			ASSERT(LoadingState::LOADED == getLoadingState() || LoadingState::UNLOADED == getLoadingState(), "Texture resource change while in-flight inside the resource streamer")

			// Set new RHI texture
			if (nullptr != mTexture)
			{
				setLoadingState(LoadingState::UNLOADED);
			}
			mTexture = texture;
			setLoadingState(LoadingState::LOADED);
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline TextureResource() :
			mRgbHardwareGammaCorrection(false)
		{
			// Nothing here
		}

		inline virtual ~TextureResource() override
		{
			// Sanity check
			ASSERT(nullptr == mTexture.getPointer(), "Invalid texture")
		}

		explicit TextureResource(const TextureResource&) = delete;
		TextureResource& operator=(const TextureResource&) = delete;

		inline TextureResource& operator=(TextureResource&& textureResource) noexcept
		{
			// Call base implementation
			IResource::operator=(std::move(textureResource));

			// Swap data
			std::swap(mRgbHardwareGammaCorrection, textureResource.mRgbHardwareGammaCorrection);
			std::swap(mTexture,					   textureResource.mTexture);

			// Done
			return *this;
		}

		//[-------------------------------------------------------]
		//[ "RERenderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(TextureResourceId textureResourceId)
		{
			// Sanity check
			ASSERT(nullptr == mTexture.getPointer(), "Invalid texture")

			// Call base implementation
			IResource::initializeElement(textureResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			mTexture = nullptr;

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			 mRgbHardwareGammaCorrection;	///< If true, sRGB texture formats will be used meaning the GPU will return linear space colors instead of gamma space colors when fetching texels inside a shader (the alpha channel always remains linear)
		RERHI::RHITexturePtr mTexture;						///< RHI texture, can be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
