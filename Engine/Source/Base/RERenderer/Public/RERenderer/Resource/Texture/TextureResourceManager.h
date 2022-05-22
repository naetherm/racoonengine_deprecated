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
#include <RECore/Resource/ResourceManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERHI
{
	class RHITexture;
}
namespace RERenderer {
class TextureResource;

class IRenderer;

class ITextureResourceLoader;
}
namespace RECore {
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32			 TextureResourceId;	///< POD texture resource identifier
	typedef RECore::StringId			 AssetId;			///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Texture resource manager class
	*/
	class TextureResourceManager final : public RECore::ResourceManager<TextureResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The texture manager automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "RacoonEngine/Texture/DynamicByCode/WhiteMap1D"
		*    - "RacoonEngine/Texture/DynamicByCode/WhiteMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/WhiteMapCube"
		*    - "RacoonEngine/Texture/DynamicByCode/WhiteMap3D"
		*    - "RacoonEngine/Texture/DynamicByCode/BlackMap1D"
		*    - "RacoonEngine/Texture/DynamicByCode/BlackMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/BlackMapCube"
		*    - "RacoonEngine/Texture/DynamicByCode/BlackMap3D"
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityAlbedoMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityAlphaMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityNormalMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityRoughnessMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/DielectricMetallicMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityEmissiveMap2D"
		*    - "RacoonEngine/Texture/DynamicByCode/Identity_argb_nxa2D"
		*    - "RacoonEngine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint8 getNumberOfTopMipmapsToRemove() const
		{
			return mNumberOfTopMipmapsToRemove;
		}

		void setNumberOfTopMipmapsToRemove(RECore::uint8 numberOfTopMipmapsToRemove);
		[[nodiscard]] TextureResource* getTextureResourceByAssetId(AssetId assetId) const;		// Considered to be inefficient, avoid method whenever possible
		[[nodiscard]] TextureResourceId getTextureResourceIdByAssetId(AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadTextureResourceByAssetId(AssetId assetId, AssetId fallbackTextureAssetId, TextureResourceId& textureResourceId, RECore::IResourceListener* resourceListener = nullptr, bool rgbHardwareGammaCorrection = false, bool reload = false, RECore::ResourceLoaderTypeId resourceLoaderTypeId = RECore::getInvalid<RECore::ResourceLoaderTypeId>());	// Asynchronous
		TextureResourceId createTextureResourceByAssetId(AssetId assetId, RERHI::RHITexture& texture, bool rgbHardwareGammaCorrection = false);	// Texture resource is not allowed to exist, yet
		void destroyTextureResource(TextureResourceId textureResourceId);
		void setInvalidResourceId(TextureResourceId& textureResourceId, RECore::IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual RECore::uint32 getNumberOfResources() const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByIndex(RECore::uint32 index) const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByResourceId(RECore::ResourceId resourceId) const override;
		[[nodiscard]] virtual RECore::IResource* tryGetResourceByResourceId(RECore::ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;

		inline virtual void update() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual RECore::IResourceLoader* createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit TextureResourceManager(IRenderer& renderer);
		virtual ~TextureResourceManager() override;
		explicit TextureResourceManager(const TextureResourceManager&) = delete;
		TextureResourceManager& operator=(const TextureResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
    IRenderer& mRenderer;
		RECore::uint8 mNumberOfTopMipmapsToRemove;	///< The number of top mipmaps to remove while loading textures for efficient texture quality reduction. By setting this to e.g. two a 4096x4096 texture will become 1024x1024.

		// Internal resource manager implementation
    RECore::ResourceManagerTemplate<TextureResource, ITextureResourceLoader, TextureResourceId, 2048>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
