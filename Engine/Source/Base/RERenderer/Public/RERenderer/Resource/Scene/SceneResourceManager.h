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
#include "RERenderer/Context.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer {
class ISceneFactory;

class SceneResource;

class IRenderer;

class SceneResourceLoader;
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
	typedef RECore::uint32 SceneResourceId;	///< POD scene resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class SceneResourceManager final : public RECore::ResourceManager<SceneResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline const ISceneFactory& getSceneFactory() const
		{
			// We know that this pointer is always valid
			RHI_ASSERT(nullptr != mSceneFactory, "Invalid scene factory")
			return *mSceneFactory;
		}

		void setSceneFactory(const ISceneFactory* sceneFactory);
		[[nodiscard]] SceneResource* getSceneResourceByAssetId(RECore::AssetId assetId) const;		// Considered to be inefficient, avoid method whenever possible
		[[nodiscard]] SceneResourceId getSceneResourceIdByAssetId(RECore::AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadSceneResourceByAssetId(RECore::AssetId assetId, SceneResourceId& sceneResourceId, RECore::IResourceListener* resourceListener = nullptr, bool reload = false, RECore::ResourceLoaderTypeId resourceLoaderTypeId = RECore::getInvalid<RECore::ResourceLoaderTypeId>());	// Asynchronous
		void destroySceneResource(SceneResourceId sceneResourceId);
		void setInvalidResourceId(SceneResourceId& sceneResourceId, RECore::IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual RECore::uint32 getNumberOfResources() const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByIndex(RECore::uint32 index) const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByResourceId(RECore::ResourceId resourceId) const override;
		[[nodiscard]] virtual RECore::IResource* tryGetResourceByResourceId(RECore::ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(RECore::AssetId assetId) override;

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
		explicit SceneResourceManager(IRenderer& renderer);
		virtual ~SceneResourceManager() override;
		explicit SceneResourceManager(const SceneResourceManager&) = delete;
		SceneResourceManager& operator=(const SceneResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&			 mRenderer;		///< Renderer instance, do not destroy the instance
		const ISceneFactory* mSceneFactory;	///< Scene factory, always valid, do not destroy the instance

		// Internal resource manager implementation
    RECore::ResourceManagerTemplate<SceneResource, SceneResourceLoader, SceneResourceId, 16>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
