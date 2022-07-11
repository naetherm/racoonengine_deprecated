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
#include "RERenderer/Resource/Scene/Item/Volume/VolumeSceneItem.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/Resource/Scene/SceneNode.h"
#include "RERenderer/IRenderer.h"
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Material/MaterialTechnique.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResource.h"


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
		static RERHI::RHIVertexArrayPtr VolumeVertexArrayPtr;	///< Vertex array object (VAO), can be a null pointer, shared between all volume instances


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] RERHI::RHIVertexArray* createVolumeVertexArray(const RERenderer::IRenderer& renderer)
		{
			// Vertex input layout
			static constexpr RERHI::VertexAttribute vertexAttributesLayout[] =
			{
				{ // Attribute 0
					// Data destination
					RERHI::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
					"Position",								// name[32] (char)
					"POSITION",								// semanticName[32] (char)
					0,										// semanticIndex (RECore::uint32)
					// Data source
					0,										// inputSlot (RECore::uint32)
					0,										// alignedByteOffset (RECore::uint32)
					sizeof(float) * 3,						// strideInBytes (RECore::uint32)
					0										// instancesPerElement (RECore::uint32)
				},
				{ // Attribute 1, see "17/11/2012 Surviving without gl_DrawID" - https://www.g-truc.net/post-0518.html
					// Data destination
					RERHI::VertexAttributeFormat::UINT_1,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
					"drawId",								// name[32] (char)
					"DRAWID",								// semanticName[32] (char)
					0,										// semanticIndex (RECore::uint32)
					// Data source
					1,										// inputSlot (RECore::uint32)
					0,										// alignedByteOffset (RECore::uint32)
					sizeof(RECore::uint32),						// strideInBytes (RECore::uint32)
					1										// instancesPerElement (RECore::uint32)
				}
			};
			const RERHI::VertexAttributes vertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

			// Our cube is constructed like this
			/*
					3+------+2  y
					/|     /|   |
				   / |    / |   |
				  / 0+---/--+1  *---x
				7+------+6 /   /
				 | /    | /   z
				 |/     |/
				4+------+5
			*/

			// Create the vertex buffer object (VBO)
			// -> Object space vertex positions
			static constexpr float VERTEX_POSITION[] =
			{
				-0.5f, -0.5f, -0.5f,	// 0
				 0.5f, -0.5f, -0.5f,	// 1
				 0.5f,  0.5f, -0.5f,	// 2
				-0.5f,  0.5f, -0.5f,	// 3
				-0.5f, -0.5f,  0.5f,	// 4
				 0.5f, -0.5f,  0.5f,	// 5
				 0.5f,  0.5f,  0.5f,	// 6
				-0.5f,  0.5f,  0.5f,	// 7
			};
			RERHI::RHIBufferManager& bufferManager = renderer.getBufferManager();
			RERHI::RHIVertexBufferPtr vertexBuffer(bufferManager.createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, 0, RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Volume")));

			// Create the index buffer object (IBO)
			static constexpr RECore::uint16 INDICES[] =
			{
				// Back		Triangle
				2, 3, 0,	// 0
				0, 1, 2,	// 1
				// Front
				7, 6, 5,	// 0
				5, 4, 7,	// 1
				// Left
				3, 7, 4,	// 0
				4, 0, 3,	// 1
				// Right
				6, 2, 1,	// 0
				1, 5, 6,	// 1
				// Top
				3, 2, 6,	// 0
				6, 7, 3,	// 1
				// Bottom
				0, 4, 5,	// 0
				5, 1, 0		// 1
			};
			RERHI::RHIIndexBufferPtr indexBuffer(bufferManager.createIndexBuffer(sizeof(INDICES), INDICES, 0, RERHI::BufferUsage::STATIC_DRAW, RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME("Volume")));

			// Create vertex array object (VAO)
			const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer, renderer.getMeshResourceManager().getDrawIdVertexBufferPtr() };
			return bufferManager.createVertexArray(vertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME("Volume"));
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
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	const RenderableManager* VolumeSceneItem::getRenderableManager() const
	{
		// Sanity check
		RHI_ASSERT(mRenderableManager.getTransform().scale.x == mRenderableManager.getTransform().scale.y && mRenderableManager.getTransform().scale.y == mRenderableManager.getTransform().scale.z, "Only uniform scale is supported to keep things simple")

		// Call the base implementation
		return MaterialSceneItem::getRenderableManager();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	void VolumeSceneItem::onMaterialResourceCreated()
	{
		// Setup renderable manager
		#ifdef DEBUG
			const char* debugName = "Volume";
			mRenderableManager.setDebugName(debugName);
		#endif
		mRenderableManager.getRenderables().emplace_back(mRenderableManager, ::detail::VolumeVertexArrayPtr, getSceneResource().getRenderer().getMaterialResourceManager(), getMaterialResourceId(), RECore::getInvalid<SkeletonResourceId>(), true, 0, 36, 1 RHI_RESOURCE_DEBUG_NAME(debugName));
		mRenderableManager.updateCachedRenderablesData();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	VolumeSceneItem::VolumeSceneItem(SceneResource& sceneResource) :
		MaterialSceneItem(sceneResource)
	{
		// Add reference to vertex array object (VAO) shared between all volume instances
		if (nullptr == ::detail::VolumeVertexArrayPtr)
		{
			::detail::VolumeVertexArrayPtr = ::detail::createVolumeVertexArray(getSceneResource().getRenderer());
			RHI_ASSERT(nullptr != ::detail::VolumeVertexArrayPtr, "Invalid volume vertex array")
		}
		::detail::VolumeVertexArrayPtr->AddReference();
	}

	VolumeSceneItem::~VolumeSceneItem()
	{
		if (RECore::isValid(getMaterialResourceId()))
		{
			// Clear the renderable manager right now so we have no more references to the shared vertex array
			mRenderableManager.getRenderables().clear();
		}

		// Release reference to vertex array object (VAO) shared between all volume instances
		if (nullptr != ::detail::VolumeVertexArrayPtr && 1 == ::detail::VolumeVertexArrayPtr->Release())	// +1 for reference to global shared pointer
		{
			::detail::VolumeVertexArrayPtr = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
