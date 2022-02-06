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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Mesh/MeshResource.h"
#include "RERenderer/Resource/Mesh/Loader/MeshResourceLoader.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#ifdef RENDERER_OPENVR
	#include "RERenderer/Vr/OpenVR/Loader/OpenVRMeshResourceLoader.h"
#endif
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		// Vertex input layout
		static constexpr RERHI::VertexAttribute DrawIdVertexAttributesLayout[] =
		{
			{ // Attribute 0, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
				// Data destination
				RERHI::VertexAttributeFormat::UINT_1,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"drawId",							// name[32] (char)
				"DRAWID",							// semanticName[32] (char)
				0,									// semanticIndex (uint32_t)
				// Data source
				0,									// inputSlot (uint32_t)
				0,									// alignedByteOffset (uint32_t)
				sizeof(uint32_t),					// strideInBytes (uint32_t)
				1									// instancesPerElement (uint32_t)
			}
		};


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
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const RERHI::VertexAttributes MeshResourceManager::DRAW_ID_VERTEX_ATTRIBUTES(static_cast<uint32_t>(GLM_COUNTOF(::detail::DrawIdVertexAttributesLayout)), ::detail::DrawIdVertexAttributesLayout);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MeshResource* MeshResourceManager::getMeshResourceByAssetId(AssetId assetId) const
	{
		return mInternalResourceManager->getResourceByAssetId(assetId);
	}

	void MeshResourceManager::loadMeshResourceByAssetId(AssetId assetId, MeshResourceId& meshResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		// Choose default resource loader type ID, if necessary
		if (RECore::isInvalid(resourceLoaderTypeId))
		{
			resourceLoaderTypeId = MeshResourceLoader::TYPE_ID;
		}

		// Load
		mInternalResourceManager->loadResourceByAssetId(assetId, meshResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	MeshResourceId MeshResourceManager::createEmptyMeshResourceByAssetId(AssetId assetId)
	{
		MeshResource& meshResource = mInternalResourceManager->createEmptyResourceByAssetId(assetId);
		setResourceLoadingState(meshResource, RECore::IResource::LoadingState::LOADED);
		return meshResource.getId();
	}

	void MeshResourceManager::setInvalidResourceId(MeshResourceId& meshResourceId, RECore::IResourceListener& resourceListener) const
	{
		MeshResource* meshResource = tryGetById(meshResourceId);
		if (nullptr != meshResource)
		{
			meshResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(meshResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	uint32_t MeshResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& MeshResourceManager::getResourceByIndex(uint32_t index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& MeshResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* MeshResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void MeshResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}

	void MeshResourceManager::update()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* MeshResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		switch (resourceLoaderTypeId)
		{
			case MeshResourceLoader::TYPE_ID:
				return new MeshResourceLoader(*this, mRenderer);

			#ifdef RENDERER_OPENVR
				case OpenVRMeshResourceLoader::TYPE_ID:
					return new OpenVRMeshResourceLoader(*this, mRenderer);
			#endif

			default:
				// TODO(naetherm) Error handling
				ASSERT(false, "Invalid resource loader type ID")
				return nullptr;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MeshResourceManager::MeshResourceManager(IRenderer& renderer) :
    mRenderer(renderer), mNumberOfTopMeshLodsToRemove(0)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);

		// Create the draw ID vertex buffer, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
		uint32_t drawIds[4096];
		for (uint32_t i = 0; i < 4096; ++i)
		{
			drawIds[i] = i;
		}
		RERHI::RHIBufferManager& bufferManager = renderer.getBufferManager();
		mDrawIdVertexBufferPtr = bufferManager.createVertexBuffer(sizeof(drawIds), drawIds, 0, RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Draw ID"));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mDrawIdVertexBufferPtr };
		mDrawIdVertexArrayPtr = bufferManager.createVertexArray(DRAW_ID_VERTEX_ATTRIBUTES, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Draw ID"));
	}

	MeshResourceManager::~MeshResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
