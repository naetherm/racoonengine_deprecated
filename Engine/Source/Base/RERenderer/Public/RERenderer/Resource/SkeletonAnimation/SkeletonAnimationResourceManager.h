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
namespace RERenderer {
class IRenderer;

class SkeletonAnimationResource;

class SkeletonAnimationController;

class SkeletonAnimationResourceLoader;
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
	typedef RECore::uint32 SkeletonAnimationResourceId;	///< POD skeleton animation resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Rigid skeleton animation resource manager
	*/
	class SkeletonAnimationResourceManager final : public RECore::ResourceManager<SkeletonAnimationResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;
		friend class SkeletonAnimationController;	// Registers/unregisters itself inside the skeleton animation resource manager


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] SkeletonAnimationResource* getSkeletonAnimationResourceByAssetId(RECore::AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void loadSkeletonAnimationResourceByAssetId(RECore::AssetId assetId, SkeletonAnimationResourceId& skeletonAnimationResourceId, RECore::IResourceListener* resourceListener = nullptr, bool reload = false, RECore::ResourceLoaderTypeId resourceLoaderTypeId = RECore::getInvalid<RECore::ResourceLoaderTypeId>());	// Asynchronous
		[[nodiscard]] SkeletonAnimationResourceId createSkeletonAnimationResourceByAssetId(RECore::AssetId assetId);	// Skeleton animation resource is not allowed to exist, yet
		void setInvalidResourceId(SkeletonAnimationResourceId& skeletonAnimationResourceId, RECore::IResourceListener& resourceListener) const;


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual RECore::uint32 getNumberOfResources() const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByIndex(RECore::uint32 index) const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByResourceId(RECore::ResourceId resourceId) const override;
		[[nodiscard]] virtual RECore::IResource* tryGetResourceByResourceId(RECore::ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(RECore::AssetId assetId) override;
		virtual void update() override;


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual RECore::IResourceLoader* createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit SkeletonAnimationResourceManager(IRenderer& renderer);
		virtual ~SkeletonAnimationResourceManager() override;
		explicit SkeletonAnimationResourceManager(const SkeletonAnimationResourceManager&) = delete;
		SkeletonAnimationResourceManager& operator=(const SkeletonAnimationResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<SkeletonAnimationController*> SkeletonAnimationControllers;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
    IRenderer& mRenderer;
		SkeletonAnimationControllers mSkeletonAnimationControllers;	///< Don't destroy the instanced, they are not owned here
    RECore::ResourceManagerTemplate<SkeletonAnimationResource, SkeletonAnimationResourceLoader, SkeletonAnimationResourceId, 2048>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
