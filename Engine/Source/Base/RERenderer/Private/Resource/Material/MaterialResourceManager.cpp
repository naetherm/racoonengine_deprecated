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
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include "RERenderer/Resource/Material/MaterialTechnique.h"
#include "RERenderer/Resource/Material/Loader/MaterialResourceLoader.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MaterialResource* MaterialResourceManager::getMaterialResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	MaterialResourceId MaterialResourceManager::getMaterialResourceIdByAssetId(AssetId assetId) const
	{
		const MaterialResource* materialResource = getMaterialResourceByAssetId(assetId);
		return (nullptr != materialResource) ? materialResource->getId() : RECore::getInvalid<MaterialResourceId>();
	}

	void MaterialResourceManager::loadMaterialResourceByAssetId(AssetId assetId, MaterialResourceId& materialResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, materialResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	MaterialResourceId MaterialResourceManager::createMaterialResourceByAssetId(AssetId assetId, AssetId materialBlueprintAssetId, MaterialTechniqueId materialTechniqueId)
	{
		// Sanity check
		RHI_ASSERT(nullptr == getMaterialResourceByAssetId(assetId), "Material resource is not allowed to exist, yet")

		// Create the material resource instance
		MaterialResource& materialResource = mInternalResourceManager->getResources().addElement();
		materialResource.setResourceManager(this);
		materialResource.setAssetId(assetId);
		#ifdef DEBUG
		{
			const RECore::AssetManager& assetManager = mRenderer.getAssetManager();
			const VirtualFilename virtualFilename = assetManager.tryGetVirtualFilenameByAssetId(assetId);
			if (nullptr != virtualFilename)
			{
				if (assetId == materialBlueprintAssetId)
				{
					materialResource.setDebugName((RECore::IFileManager::INVALID_CHARACTER + std::string("[CreatedMaterial][InstanceOfMaterialBlueprintAsset=\"") + std::string(virtualFilename) + "\"]").c_str());
				}
				else
				{
					materialResource.setDebugName((RECore::IFileManager::INVALID_CHARACTER + std::string("[CreatedMaterial][Asset=\"") + std::string(virtualFilename) + "\"][MaterialBlueprintAsset=\"" + assetManager.getAssetByAssetId(materialBlueprintAssetId).virtualFilename + "\"]").c_str());
				}
			}
			else
			{
				materialResource.setDebugName((RECore::IFileManager::INVALID_CHARACTER + std::string("[CreatedMaterial][AssetId=") + std::to_string(assetId) + "][MaterialBlueprintAsset=\"" + assetManager.getAssetByAssetId(materialBlueprintAssetId).virtualFilename + "\"]").c_str());
			}
		}
		#endif

		{ // Setup material resource instance
			// Copy over the material properties of the material blueprint resource
			MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
			MaterialBlueprintResourceId materialBlueprintResourceId = RECore::getInvalid<MaterialBlueprintResourceId>();
			materialBlueprintResourceManager.loadMaterialBlueprintResourceByAssetId(materialBlueprintAssetId, materialBlueprintResourceId);
			MaterialBlueprintResource* materialBlueprintResource = materialBlueprintResourceManager.tryGetById(materialBlueprintResourceId);
			if (nullptr != materialBlueprintResource)
			{
				// TODO(naetherm) Possible optimization: Right now we don't filter for "RERenderer::MaterialProperty::Usage::GLOBAL_REFERENCE_FALLBACK" properties.
				//          Only the material blueprint resource needs to store such properties while they're useless inside material resources. The filtering
				//          makes the following more complex and it might not bring any real benefit. So, review this place in here later when we have more pressure on the system.
				materialResource.mMaterialProperties = materialBlueprintResource->mMaterialProperties;

				// Create default material technique
				materialResource.mSortedMaterialTechniqueVector.push_back(new MaterialTechnique(materialTechniqueId, materialResource, materialBlueprintResourceId));
			}
			else
			{
				// Error!
				RHI_ASSERT(false, "Invalid material blueprint resource")
			}
		}

		// Done
		setResourceLoadingState(materialResource, RECore::IResource::LoadingState::LOADED);
		return materialResource.getId();
	}

	MaterialResourceId MaterialResourceManager::createMaterialResourceByCloning(MaterialResourceId parentMaterialResourceId, AssetId assetId)
	{
		RHI_ASSERT(mInternalResourceManager->getResources().getElementById(parentMaterialResourceId).getLoadingState() == RECore::IResource::LoadingState::LOADED, "Invalid parent material resource ID")

		// Create the material resource instance
		MaterialResource& materialResource = mInternalResourceManager->getResources().addElement();
		materialResource.setResourceManager(this);
		materialResource.setAssetId(assetId);
		materialResource.setParentMaterialResourceId(parentMaterialResourceId);
		#ifdef DEBUG
			materialResource.setDebugName((std::string(mInternalResourceManager->getResources().getElementById(parentMaterialResourceId).getDebugName()) + "[Clone]").c_str());
		#endif

		// Done
		setResourceLoadingState(materialResource, RECore::IResource::LoadingState::LOADED);
		return materialResource.getId();
	}

	void MaterialResourceManager::destroyMaterialResource(MaterialResourceId materialResourceId)
	{
		mInternalResourceManager->getResources().removeElement(materialResourceId);
	}

	void MaterialResourceManager::setInvalidResourceId(MaterialResourceId& materialResourceId, RECore::IResourceListener& resourceListener) const
	{
		MaterialResource* materialResource = tryGetById(materialResourceId);
		if (nullptr != materialResource)
		{
			materialResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(materialResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 MaterialResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& MaterialResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& MaterialResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* MaterialResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void MaterialResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* MaterialResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new MaterialResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MaterialResourceManager::MaterialResourceManager(IRenderer& renderer) :
		mRenderer(renderer)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<MaterialResource, MaterialResourceLoader, MaterialResourceId, 4096>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);
	}

	MaterialResourceManager::~MaterialResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
