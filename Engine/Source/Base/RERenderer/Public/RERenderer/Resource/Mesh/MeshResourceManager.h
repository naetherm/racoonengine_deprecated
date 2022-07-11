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
#include "RERenderer/RERenderer.h"
#include <RECore/Resource/ResourceManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer {
class MeshResource;

class IRenderer;

class IMeshResourceLoader;
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
	typedef RECore::uint32 MeshResourceId;	///< POD mesh resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class RERENDERER_API MeshResourceManager final : public RECore::ResourceManager<MeshResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const RERHI::VertexAttributes DRAW_ID_VERTEX_ATTRIBUTES;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint8 getNumberOfTopMeshLodsToRemove() const
		{
			return mNumberOfTopMeshLodsToRemove;
		}

		inline void setNumberOfTopMeshLodsToRemove(RECore::uint8 numberOfTopMeshLodsToRemove)
		{
			mNumberOfTopMeshLodsToRemove = numberOfTopMeshLodsToRemove;
		}

		[[nodiscard]] MeshResource* getMeshResourceByAssetId(RECore::AssetId assetId) const;	// Considered to be inefficient, avoid method whenever possible
		void RERENDERER_API loadMeshResourceByAssetId(RECore::AssetId assetId, MeshResourceId& meshResourceId, RECore::IResourceListener* resourceListener = nullptr, bool reload = false, RECore::ResourceLoaderTypeId resourceLoaderTypeId = RECore::getInvalid<RECore::ResourceLoaderTypeId>());	// Asynchronous
		[[nodiscard]] MeshResourceId createEmptyMeshResourceByAssetId(RECore::AssetId assetId);	// Mesh resource is not allowed to exist, yet, prefer asynchronous mesh resource loading over this method
		void setInvalidResourceId(MeshResourceId& textureResourceId, RECore::IResourceListener& resourceListener) const;

		[[nodiscard]] inline const RERHI::RHIVertexBufferPtr& getDrawIdVertexBufferPtr() const
		{
			return mDrawIdVertexBufferPtr;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getDrawIdVertexArrayPtr() const
		{
			return mDrawIdVertexArrayPtr;
		}


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
		explicit MeshResourceManager(IRenderer& renderer);
		virtual ~MeshResourceManager() override;
		explicit MeshResourceManager(const MeshResourceManager&) = delete;
		MeshResourceManager& operator=(const MeshResourceManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
    IRenderer& mRenderer;
		RECore::uint8				  mNumberOfTopMeshLodsToRemove;	///< The number of top mesh LODs to remove, only has an impact while rendering and not on loading (amount of needed memory is not influenced)
    RECore::ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>* mInternalResourceManager;
		RERHI::RHIVertexBufferPtr mDrawIdVertexBufferPtr;		///< Draw ID vertex buffer, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
		RERHI::RHIVertexArrayPtr  mDrawIdVertexArrayPtr;		///< Draw ID vertex array, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
