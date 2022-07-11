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
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include "RERenderer/Resource/Texture/TextureResource.h"
#include "RERenderer/Resource/Texture/Loader/CrnTextureResourceLoader.h"
#include "RERenderer/Resource/Texture/Loader/CrnArrayTextureResourceLoader.h"
#include "RERenderer/Resource/Texture/Loader/Lz4DdsTextureResourceLoader.h"
#include "RERenderer/Resource/Texture/Loader/KtxTextureResourceLoader.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#ifdef RENDERER_OPENVR
	#include "RERenderer/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.h"
#endif
#include "RERenderer/IRenderer.h"
#include <RERHI/Rhi.h>


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
		void createDefaultDynamicTextureAssets(RERenderer::IRenderer& renderer, RERenderer::TextureResourceManager& textureResourceManager)
		{
			RERHI::RHITextureManager& textureManager = renderer.getTextureManager();

			// White RGBA texture
			const RECore::uint8 whiteRgbaData[] = {
				255, 255, 255, 255,	// Face 0
				255, 255, 255, 255,	// Face 1
				255, 255, 255, 255,	// Face 2
				255, 255, 255, 255,	// Face 3
				255, 255, 255, 255,	// Face 4
				255, 255, 255, 255	// Face 5
			};
			RERHI::RHITexturePtr whiteRgba1DTexturePtr(textureManager.createTexture1D(1, RERHI::TextureFormat::R8G8B8A8, whiteRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			RERHI::RHITexturePtr whiteRgba2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8G8B8A8, whiteRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			RERHI::RHITexturePtr whiteRgbaCubeTexturePtr(textureManager.createTextureCube(1, RERHI::TextureFormat::R8G8B8A8, whiteRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));
			RERHI::RHITexturePtr whiteRgba3DTexturePtr(textureManager.createTexture3D(1, 1, 1, RERHI::TextureFormat::R8G8B8A8, whiteRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("White RGBA")));

			// Normal map identity texture
			const RECore::uint8 normalMapIdentityData[] = { 128, 128, 255, 255 };
			RERHI::RHITexturePtr normalMapIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8G8B8A8, normalMapIdentityData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Normal map identity")));

			// White alpha texture
			const RECore::uint8 whiteAData[] = { 255 };
			RERHI::RHITexturePtr whiteA2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8, whiteAData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("White alpha")));

			// Black RGBA texture
			const RECore::uint8 blackRgbaData[] = {
				0, 0, 0, 0,	// Face 0
				0, 0, 0, 0,	// Face 1
				0, 0, 0, 0,	// Face 2
				0, 0, 0, 0,	// Face 3
				0, 0, 0, 0,	// Face 4
				0, 0, 0, 0	// Face 5
			};
			RERHI::RHITexturePtr blackRgba1DTexturePtr(textureManager.createTexture1D(1, RERHI::TextureFormat::R8G8B8A8, blackRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			RERHI::RHITexturePtr blackRgba2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8G8B8A8, blackRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			RERHI::RHITexturePtr blackRgbaCubeTexturePtr(textureManager.createTextureCube(1, RERHI::TextureFormat::R8G8B8A8, blackRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));
			RERHI::RHITexturePtr blackRgba3DTexturePtr(textureManager.createTexture3D(1, 1, 1, RERHI::TextureFormat::R8G8B8A8, blackRgbaData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE RHI_RESOURCE_DEBUG_NAME("Black RGBA")));

			// Black alpha texture
			const RECore::uint8 blackAData[] = { 0 };
			RERHI::RHITexturePtr blackA2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8, blackAData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Black alpha")));

			// "_argb_nxa"-texture
			// -> Must be white so e.g. albedo color can be multiplied in
			const RECore::uint8 _argb_nxaIdentityData[] = { 255, 255, 255, 128 };
			RERHI::RHITexturePtr _argb_nxaIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8G8B8A8, _argb_nxaIdentityData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("_argb_nxa identity")));

			// "_hr_rg_mb_nya"-texture
			const RECore::uint8 _hr_rg_mb_nyaIdentityData[] = { 0, 255, 0, 128 };
			RERHI::RHITexturePtr _hr_rg_mb_nyaIdentity2DTexturePtr(textureManager.createTexture2D(1, 1, RERHI::TextureFormat::R8G8B8A8, _hr_rg_mb_nyaIdentityData, RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::IMMUTABLE, 1, nullptr RHI_RESOURCE_DEBUG_NAME("_hr_rg_mb_nya identity")));

			// Define helper macro
			#define CREATE_TEXTURE(name, texturePtr) textureResourceManager.createTextureResourceByAssetId(ASSET_ID("RacoonEngine/Texture/DynamicByCode/"#name), *texturePtr);

			// Create default dynamic texture assets
			CREATE_TEXTURE(WhiteMap1D,				whiteRgba1DTexturePtr)
			CREATE_TEXTURE(WhiteMap2D,				whiteRgba2DTexturePtr)
			CREATE_TEXTURE(WhiteMapCube,			whiteRgbaCubeTexturePtr)
			CREATE_TEXTURE(WhiteMap3D,				whiteRgba3DTexturePtr)
			CREATE_TEXTURE(BlackMap1D,				blackRgba1DTexturePtr)
			CREATE_TEXTURE(BlackMap2D,				blackRgba2DTexturePtr)
			CREATE_TEXTURE(BlackMapCube,			blackRgbaCubeTexturePtr)
			CREATE_TEXTURE(BlackMap3D,				blackRgba3DTexturePtr)
			CREATE_TEXTURE(IdentityAlbedoMap2D,		whiteRgba2DTexturePtr)	// Must be white so e.g. albedo color can be multiplied in
			CREATE_TEXTURE(IdentityAlphaMap2D,		whiteA2DTexturePtr)
			CREATE_TEXTURE(IdentityNormalMap2D,		normalMapIdentity2DTexturePtr)
			CREATE_TEXTURE(IdentityRoughnessMap2D,	whiteA2DTexturePtr)
			CREATE_TEXTURE(DielectricMetallicMap2D,	blackA2DTexturePtr)
			CREATE_TEXTURE(IdentityEmissiveMap2D,	blackRgba2DTexturePtr)
			CREATE_TEXTURE(Identity_argb_nxa2D,		_argb_nxaIdentity2DTexturePtr)
			CREATE_TEXTURE(Identity_hr_rg_mb_nya2D,	_hr_rg_mb_nyaIdentity2DTexturePtr)

			// Undefine helper macro
			#undef CREATE_TEXTURE
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
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void TextureResourceManager::getDefaultTextureAssetIds(AssetIds& assetIds)
	{
		// Define helper macros
		#define ADD_ASSET_ID(name) assetIds.push_back(ASSET_ID(name));

		// Add asset IDs
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/WhiteMap1D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/WhiteMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/WhiteMapCube")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/WhiteMap3D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/BlackMap1D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/BlackMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/BlackMapCube")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/BlackMap3D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityAlbedoMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityAlphaMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityNormalMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityRoughnessMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/DielectricMetallicMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityEmissiveMap2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/Identity_argb_nxa2D")
		ADD_ASSET_ID("RacoonEngine/Texture/DynamicByCode/Identity_hr_rg_mb_nya2D")

		// Undefine helper macros
		#undef ADD_ASSET_ID
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void TextureResourceManager::setNumberOfTopMipmapsToRemove(RECore::uint8 numberOfTopMipmapsToRemove)
	{
		if (mNumberOfTopMipmapsToRemove != numberOfTopMipmapsToRemove)
		{
			mNumberOfTopMipmapsToRemove = numberOfTopMipmapsToRemove;

			// Update the already loaded textures, but really only textures loaded from texture assets and not e.g. textures dynamically created during runtime
			const RECore::AssetManager& assetManager = mRenderer.getAssetManager();
			const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				const TextureResource& textureResource = mInternalResourceManager->getResources().getElementByIndex(i);
				const AssetId assetId = textureResource.getAssetId();
				if (nullptr != assetManager.tryGetAssetByAssetId(assetId) && textureResource.getLoadingState() == RECore::IResource::LoadingState::LOADED)
				{
					TextureResourceId textureResourceId = RECore::getInvalid<TextureResourceId>();
					loadTextureResourceByAssetId(assetId, RECore::getInvalid<AssetId>(), textureResourceId, nullptr, textureResource.isRgbHardwareGammaCorrection(), true);
				}
			}
		}
	}

	TextureResource* TextureResourceManager::getTextureResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	TextureResourceId TextureResourceManager::getTextureResourceIdByAssetId(AssetId assetId) const
	{
		const TextureResource* textureResource = getTextureResourceByAssetId(assetId);
		return (nullptr != textureResource) ? textureResource->getId() : RECore::getInvalid<TextureResourceId>();
	}

	void TextureResourceManager::loadTextureResourceByAssetId(AssetId assetId, AssetId fallbackTextureAssetId, TextureResourceId& textureResourceId, RECore::IResourceListener* resourceListener, bool rgbHardwareGammaCorrection, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// Check whether or not the texture resource already exists
		TextureResource* textureResource = getTextureResourceByAssetId(assetId);

		// Create the resource instance
		// -> In case the texture asset ID is unknown it might be a runtime dynamic created texture which will be created by someone later one
		// -> Please note that the fallback texture asset ID is intentionally only used if the texture asset ID is valid, it's a fallback as long as the real texture data has not been loaded yet
		const IRenderer& renderer = mRenderer;
		const RECore::Asset* asset = renderer.getAssetManager().tryGetAssetByAssetId(assetId);
		// RHI_ASSERT(nullptr != asset, "Unknown asset ID")	// For texture assets there's no assert by intent since it's not unusual that e.g. referenced compositor texture assets get created later on
		bool load = (reload && nullptr != asset);
		if (nullptr == textureResource && nullptr != asset)
		{
			textureResource = &mInternalResourceManager->getResources().addElement();
			textureResource->setResourceManager(this);
			textureResource->setAssetId(assetId);
			textureResource->setResourceLoaderTypeId(resourceLoaderTypeId);
			textureResource->mRgbHardwareGammaCorrection = rgbHardwareGammaCorrection;
			load = true;
		}

		// Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
		if (nullptr != textureResource)
		{
			textureResourceId = textureResource->getId();
			if (nullptr != resourceListener)
			{
				textureResource->connectResourceListener(*resourceListener);
			}
		}
		else
		{
			textureResourceId = RECore::getInvalid<TextureResourceId>();
		}

		// Load the resource, if required
		if (load)
		{
			// Prepare the resource loader
			if (RECore::isInvalid(resourceLoaderTypeId))
			{
				// The totally primitive texture resource loader type detection is sufficient for now
				const char* filenameExtension = strrchr(&asset->virtualFilename[0], '.');
				if (nullptr != filenameExtension)
				{
					resourceLoaderTypeId = RECore::StringId(filenameExtension + 1);
				}
				else
				{
					// Error!
					RHI_ASSERT(false, "We should never ever be able to be in here, it's the renderer toolkit responsible to ensure the renderer only works with sane data")
				}
			}
			if (RECore::isValid(resourceLoaderTypeId))
			{
				// Commit resource streamer asset load request
				renderer.getResourceStreamer().commitLoadRequest(RECore::ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, *this, textureResourceId));

				// Since it might take a moment to load the texture resource, we'll use a fallback placeholder RHI texture resource so we don't have to wait until the real thing is there
				// -> In case there's already a RHI texture, keep that as long as possible (for example there might be a change in the number of top mipmaps to remove)
				if (nullptr == textureResource->mTexture)
				{
					if (RECore::isValid(fallbackTextureAssetId))
					{
						const TextureResource* fallbackTextureResource = getTextureResourceByAssetId(fallbackTextureAssetId);
						if (nullptr != fallbackTextureResource)
						{
							textureResource->mTexture = fallbackTextureResource->getTexturePtr();
							textureResource->setLoadingState(RECore::IResource::LoadingState::LOADED);
						}
						else
						{
							// Error! 
							RHI_ASSERT(false, "Fallback texture asset ID not found")
						}
					}
					else
					{
						// Hiccups / lags warning
						RHI_ASSERT(false, "There should always be a fallback texture asset ID (better be safe than sorry)")
					}
				}
			}
			else
			{
				// Error!
				RHI_ASSERT(false, "We should never ever be able to be in here, it's the renderer toolkit responsible to ensure the renderer only works with sane data")
			}
		}
	}

	TextureResourceId TextureResourceManager::createTextureResourceByAssetId(AssetId assetId, RERHI::RHITexture& texture, bool rgbHardwareGammaCorrection)
	{
		// Sanity check
		RHI_ASSERT(mRenderer.getContext(), nullptr == getTextureResourceByAssetId(assetId), "The texture resource isn't allowed to exist, yet")

		// Create the texture resource instance
		TextureResource& textureResource = mInternalResourceManager->getResources().addElement();
		textureResource.setResourceManager(this);
		textureResource.setAssetId(assetId);
		textureResource.mRgbHardwareGammaCorrection = rgbHardwareGammaCorrection;	// TODO(naetherm) We might need to extend "RERHI::RHITexture" so we can readback the texture format
		textureResource.mTexture = &texture;

		// Done
		setResourceLoadingState(textureResource, RECore::IResource::LoadingState::LOADED);
		return textureResource.getId();
	}

	void TextureResourceManager::destroyTextureResource(TextureResourceId textureResourceId)
	{
		mInternalResourceManager->getResources().removeElement(textureResourceId);
	}

	void TextureResourceManager::setInvalidResourceId(TextureResourceId& textureResourceId, RECore::IResourceListener& resourceListener) const
	{
		TextureResource* textureResource = tryGetById(textureResourceId);
		if (nullptr != textureResource)
		{
			textureResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(textureResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 TextureResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& TextureResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& TextureResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* TextureResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void TextureResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(naetherm) Experimental implementation (take care of resource cleanup etc.)
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			const TextureResource& textureResource = mInternalResourceManager->getResources().getElementByIndex(i);
			if (textureResource.getAssetId() == assetId)
			{
				TextureResourceId textureResourceId = RECore::getInvalid<TextureResourceId>();
				loadTextureResourceByAssetId(assetId, RECore::getInvalid<AssetId>(), textureResourceId, nullptr, textureResource.isRgbHardwareGammaCorrection(), true, textureResource.getResourceLoaderTypeId());
				break;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* TextureResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		switch (resourceLoaderTypeId)
		{
			case CrnTextureResourceLoader::TYPE_ID:
				return new CrnTextureResourceLoader(*this, mRenderer);

			case CrnArrayTextureResourceLoader::TYPE_ID:
				return new CrnArrayTextureResourceLoader(*this, mRenderer);

			case Lz4DdsTextureResourceLoader::TYPE_ID:
				return new Lz4DdsTextureResourceLoader(*this, mRenderer);

			case KtxTextureResourceLoader::TYPE_ID:
				return new KtxTextureResourceLoader(*this, mRenderer);

			#ifdef RENDERER_OPENVR
				case OpenVRTextureResourceLoader::TYPE_ID:
					return new OpenVRTextureResourceLoader(*this, mRenderer);
			#endif

			default:
				// TODO(naetherm) Error handling
				RHI_ASSERT(mRenderer.getContext(), false, "Invalid resource loader type ID")
				return nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	TextureResourceManager::TextureResourceManager(IRenderer& renderer) :
    mRenderer(renderer),
		mNumberOfTopMipmapsToRemove(0)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<TextureResource, ITextureResourceLoader, TextureResourceId, 2048>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
		::detail::createDefaultDynamicTextureAssets(renderer, *this);
	}

	TextureResourceManager::~TextureResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
