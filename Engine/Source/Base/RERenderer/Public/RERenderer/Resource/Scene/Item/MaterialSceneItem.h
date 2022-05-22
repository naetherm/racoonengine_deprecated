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
#include "RERenderer/RenderQueue/RenderableManager.h"
#include <RECore/Resource/IResourceListener.h>
#include "RERenderer/Resource/Scene/Item/ISceneItem.h"
#include "RERenderer/Resource/Material/MaterialProperties.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;				///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::uint32 MaterialTechniqueId;	///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"
	typedef RECore::uint32 MaterialResourceId;	///< POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract material item
	*/
	class MaterialSceneItem : public ISceneItem, public RECore::IResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline AssetId getMaterialAssetId() const
		{
			return mMaterialAssetId;
		}

		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
		}

		[[nodiscard]] inline AssetId getMaterialBlueprintAssetId() const
		{
			return mMaterialBlueprintAssetId;
		}

		[[nodiscard]] inline const MaterialProperties& getMaterialProperties() const
		{
			return mMaterialProperties;
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(RECore::uint32 numberOfBytes, const RECore::uint8* data) override;
		virtual void onAttachedToSceneNode(SceneNode& sceneNode) override;

		inline virtual void onDetachedFromSceneNode(SceneNode& sceneNode) override
		{
			mRenderableManager.setTransform(nullptr);

			// Call the base implementation
			ISceneItem::onDetachedFromSceneNode(sceneNode);
		}

		inline virtual void setVisible(bool visible) override
		{
			mRenderableManager.setVisible(visible);
		}

		[[nodiscard]] virtual const RenderableManager* getRenderableManager() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void initialize();
		virtual void onMaterialResourceCreated() = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual RECore::IResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onLoadingStateChange(const RECore::IResource& resource) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline MaterialSceneItem(SceneResource& sceneResource, bool cullable = true) :
			ISceneItem(sceneResource, cullable),
			mMaterialTechniqueId(RECore::getInvalid<MaterialTechniqueId>()),
			mMaterialResourceId(RECore::getInvalid<MaterialResourceId>())
		{
			// Nothing here
		}

		virtual ~MaterialSceneItem() override;
		explicit MaterialSceneItem(const MaterialSceneItem&) = delete;
		MaterialSceneItem& operator=(const MaterialSceneItem&) = delete;
		void createMaterialResource(MaterialResourceId parentMaterialResourceId);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		RenderableManager mRenderableManager;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetId				mMaterialAssetId;			///< If material blueprint asset ID is set, material asset ID must be invalid
		MaterialTechniqueId	mMaterialTechniqueId;		///< Must always be valid
		AssetId				mMaterialBlueprintAssetId;	///< If material asset ID is set, material blueprint asset ID must be invalid
		MaterialProperties	mMaterialProperties;
		MaterialResourceId	mMaterialResourceId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
