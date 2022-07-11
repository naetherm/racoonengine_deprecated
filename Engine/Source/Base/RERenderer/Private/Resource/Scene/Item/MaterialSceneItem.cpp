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
#include "RERenderer/Resource/Scene/Item/MaterialSceneItem.h"
#include "RERenderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::deserialize([[maybe_unused]] RECore::uint32 numberOfBytes, const RECore::uint8* data)
	{
		// Sanity check
		RHI_ASSERT(sizeof(v1Scene::MaterialData) <= numberOfBytes, "Invalid number of bytes")

		// Read data
		const v1Scene::MaterialData* materialData = reinterpret_cast<const v1Scene::MaterialData*>(data);
		RHI_ASSERT(sizeof(v1Scene::MaterialData) + sizeof(MaterialProperty) * materialData->numberOfMaterialProperties == numberOfBytes, "Invalid number of bytes")
		mMaterialAssetId = materialData->materialAssetId;
		mMaterialTechniqueId = materialData->materialTechniqueId;
		mMaterialBlueprintAssetId = materialData->materialBlueprintAssetId;

		{ // Read material properties
			// TODO(naetherm) Get rid of the evil const-cast
			MaterialProperties::SortedPropertyVector& sortedPropertyVector = const_cast<MaterialProperties::SortedPropertyVector&>(mMaterialProperties.getSortedPropertyVector());
			sortedPropertyVector.resize(materialData->numberOfMaterialProperties);
			memcpy(reinterpret_cast<char*>(sortedPropertyVector.data()), data + sizeof(v1Scene::MaterialData), sizeof(MaterialProperty) * materialData->numberOfMaterialProperties);
		}

		// Sanity checks
		RHI_ASSERT(RECore::isValid(mMaterialAssetId) || RECore::isValid(mMaterialBlueprintAssetId), "Invalid data")
		RHI_ASSERT(!(RECore::isValid(mMaterialAssetId) && RECore::isValid(mMaterialBlueprintAssetId)), "Invalid data")
	}

	void MaterialSceneItem::onAttachedToSceneNode(SceneNode& sceneNode)
	{
		mRenderableManager.setTransform(&sceneNode.getGlobalTransform());

		// Call the base implementation
		ISceneItem::onAttachedToSceneNode(sceneNode);
	}

	const RenderableManager* MaterialSceneItem::getRenderableManager() const
	{
		if (!RECore::isValid(getMaterialResourceId()))
		{
			// TODO(naetherm) Get rid of the nasty delayed initialization in here, including the evil const-cast. For this, full asynchronous material blueprint loading must work. See "TODO(naetherm) Currently material blueprint resource loading is a blocking process.".
			const_cast<MaterialSceneItem*>(this)->initialize();
		}
		return &mRenderableManager;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::initialize()
	{
		// Sanity checks
		RHI_ASSERT(RECore::isValid(mMaterialAssetId) || RECore::isValid(mMaterialBlueprintAssetId), "Invalid data")
		RHI_ASSERT(!(RECore::isValid(mMaterialAssetId) && RECore::isValid(mMaterialBlueprintAssetId)), "Invalid data")

		// Get parent material resource ID and initiate creating the material resource
		MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
		if (RECore::isValid(mMaterialAssetId))
		{
			// Get or load material resource
			MaterialResourceId materialResourceId = RECore::getInvalid<MaterialResourceId>();
			materialResourceManager.loadMaterialResourceByAssetId(mMaterialAssetId, materialResourceId, this);
		}
		else
		{
			// Get or load material blueprint resource
			const AssetId materialBlueprintAssetId = mMaterialBlueprintAssetId;
			if (RECore::isValid(materialBlueprintAssetId))
			{
				MaterialResourceId parentMaterialResourceId = materialResourceManager.getMaterialResourceIdByAssetId(materialBlueprintAssetId);
				if (RECore::isInvalid(parentMaterialResourceId))
				{
					parentMaterialResourceId = materialResourceManager.createMaterialResourceByAssetId(materialBlueprintAssetId, materialBlueprintAssetId, mMaterialTechniqueId);
				}
				createMaterialResource(parentMaterialResourceId);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	void MaterialSceneItem::onLoadingStateChange(const RECore::IResource& resource)
	{
		RHI_ASSERT(resource.getAssetId() == mMaterialAssetId, "Invalid asset ID")
		if (resource.getLoadingState() == RECore::IResource::LoadingState::LOADED)
		{
			mRenderableManager.getRenderables().clear();

			// Destroy the material resource we created
			if (RECore::isValid(mMaterialResourceId))
			{
				getSceneResource().getRenderer().getMaterialResourceManager().destroyMaterialResource(mMaterialResourceId);
				RECore::setInvalid(mMaterialResourceId);
			}

			// Create material resource
			createMaterialResource(resource.getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	MaterialSceneItem::~MaterialSceneItem()
	{
		if (RECore::isValid(mMaterialResourceId))
		{
			// Clear the renderable manager right now
			mRenderableManager.getRenderables().clear();

			// Destroy the material resource we created
			getSceneResource().getRenderer().getMaterialResourceManager().destroyMaterialResource(mMaterialResourceId);
		}
	}

	void MaterialSceneItem::createMaterialResource(MaterialResourceId parentMaterialResourceId)
	{
		// Sanity checks
		RHI_ASSERT(RECore::isInvalid(mMaterialResourceId), "Invalid data")
		RHI_ASSERT(RECore::isValid(parentMaterialResourceId), "Invalid data")

		// Each material user instance must have its own material resource since material property values might vary
		MaterialResourceManager& materialResourceManager = getSceneResource().getRenderer().getMaterialResourceManager();
		mMaterialResourceId = materialResourceManager.createMaterialResourceByCloning(parentMaterialResourceId);

		{ // Set material properties
			const MaterialProperties::SortedPropertyVector& sortedPropertyVector = mMaterialProperties.getSortedPropertyVector();
			if (!sortedPropertyVector.empty())
			{
				MaterialResource& materialResource = materialResourceManager.getById(mMaterialResourceId);
				for (const MaterialProperty& materialProperty : sortedPropertyVector)
				{
					if (materialProperty.isOverwritten())
					{
						materialResource.setPropertyById(materialProperty.getMaterialPropertyId(), materialProperty);
					}
				}
			}
		}

		// Tell the world
		onMaterialResourceCreated();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
