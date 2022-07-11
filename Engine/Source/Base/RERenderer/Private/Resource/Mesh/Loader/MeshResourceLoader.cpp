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
#include "RERenderer/Resource/Mesh/Loader/MeshResourceLoader.h"
#include "RERenderer/Resource/Mesh/Loader/MeshFileFormat.h"
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Mesh/MeshResource.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Skeleton/SkeletonResourceManager.h"
#include "RERenderer/Resource/Skeleton/SkeletonResource.h"
#include "RERenderer/IRenderer.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	bool MeshResourceLoader::onDeserialization(RECore::IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1Mesh::FORMAT_TYPE, v1Mesh::FORMAT_VERSION, file);
	}

	void MeshResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the mesh header
		v1Mesh::MeshHeader meshHeader;
		mMemoryFile.read(&meshHeader, sizeof(v1Mesh::MeshHeader));

		// Sanity checks
		RHI_ASSERT(0 != meshHeader.numberOfBytesPerVertex, "Invalid mesh with zero bytes per vertex")
		RHI_ASSERT(0 != meshHeader.numberOfVertices, "Invalid mesh which has no vertices")
		RHI_ASSERT(0 != meshHeader.numberOfVertexAttributes, "Invalid mesh which has no vertex attributes")
		RHI_ASSERT(0 != meshHeader.numberOfSubMeshes, "Invalid mesh which has no sub-meshes")
		RHI_ASSERT(0 != meshHeader.numberOfLods, "Invalid mesh which has no LODs: There must always be at least one LOD, namely the original none reduced version")

		// Set basic mesh resource data
		mMeshResource->setBoundingBoxPosition(meshHeader.minimumBoundingBoxPosition, meshHeader.maximumBoundingBoxPosition);
		mMeshResource->setBoundingSpherePositionRadius(meshHeader.boundingSpherePosition, meshHeader.boundingSphereRadius);
		mMeshResource->setNumberOfVertices(meshHeader.numberOfVertices);
		mMeshResource->setNumberOfIndices(meshHeader.numberOfIndices);
		mMeshResource->setNumberOfLods(meshHeader.numberOfLods);

		{ // Read in the vertex buffer
			// Allocate memory for the local vertex buffer data
			mNumberOfUsedVertexBufferDataBytes = meshHeader.numberOfBytesPerVertex * meshHeader.numberOfVertices;
			if (mNumberOfVertexBufferDataBytes < mNumberOfUsedVertexBufferDataBytes)
			{
				mNumberOfVertexBufferDataBytes = mNumberOfUsedVertexBufferDataBytes;
				delete [] mVertexBufferData;
				mVertexBufferData = new RECore::uint8[mNumberOfVertexBufferDataBytes];
			}

			// Read in the vertex buffer
			mMemoryFile.read(mVertexBufferData, mNumberOfUsedVertexBufferDataBytes);
		}

		// Read in the index buffer
		mIndexBufferFormat = meshHeader.indexBufferFormat;
		mNumberOfUsedIndexBufferDataBytes = RERHI::IndexBufferFormat::getNumberOfBytesPerElement(static_cast<RERHI::IndexBufferFormat::Enum>(mIndexBufferFormat)) * mMeshResource->getNumberOfIndices();
		if (mNumberOfUsedIndexBufferDataBytes > 0)
		{
			// Allocate memory for the local index buffer data
			if (mNumberOfIndexBufferDataBytes < mNumberOfUsedIndexBufferDataBytes)
			{
				mNumberOfIndexBufferDataBytes = mNumberOfUsedIndexBufferDataBytes;
				delete [] mIndexBufferData;
				mIndexBufferData = new RECore::uint8[mNumberOfIndexBufferDataBytes];
			}

			// Read in the index buffer
			mMemoryFile.read(mIndexBufferData, mNumberOfUsedIndexBufferDataBytes);
		}

		// Read in the position-only index buffer
		if (meshHeader.hasPositionOnlyIndices)
		{
			mNumberOfUsedPositionOnlyIndexBufferDataBytes = mNumberOfUsedIndexBufferDataBytes;
			if (mNumberOfUsedPositionOnlyIndexBufferDataBytes > 0)
			{
				// Allocate memory for the local position-only index buffer data
				if (mNumberOfPositionOnlyIndexBufferDataBytes < mNumberOfUsedPositionOnlyIndexBufferDataBytes)
				{
					mNumberOfPositionOnlyIndexBufferDataBytes = mNumberOfUsedPositionOnlyIndexBufferDataBytes;
					delete [] mPositionOnlyIndexBufferData;
					mPositionOnlyIndexBufferData = new RECore::uint8[mNumberOfPositionOnlyIndexBufferDataBytes];
				}

				// Read in the position-only index buffer
				mMemoryFile.read(mPositionOnlyIndexBufferData, mNumberOfUsedPositionOnlyIndexBufferDataBytes);
			}
		}
		else
		{
			mNumberOfUsedPositionOnlyIndexBufferDataBytes = 0;
		}

		// Read in the vertex attributes
		mNumberOfUsedVertexAttributes = meshHeader.numberOfVertexAttributes;
		if (mNumberOfVertexAttributes < mNumberOfUsedVertexAttributes)
		{
			mNumberOfVertexAttributes = mNumberOfUsedVertexAttributes;
			delete [] mVertexAttributes;
			mVertexAttributes = new RERHI::VertexAttribute[mNumberOfVertexAttributes];
		}
		mMemoryFile.read(mVertexAttributes, sizeof(RERHI::VertexAttribute) * mNumberOfUsedVertexAttributes);

		// Read in the sub-meshes
		mNumberOfUsedSubMeshes = meshHeader.numberOfSubMeshes;
		if (mNumberOfSubMeshes < mNumberOfUsedSubMeshes)
		{
			mNumberOfSubMeshes = mNumberOfUsedSubMeshes;
			delete [] mSubMeshes;
			mSubMeshes = new v1Mesh::SubMesh[mNumberOfSubMeshes];
		}
		mMemoryFile.read(mSubMeshes, sizeof(v1Mesh::SubMesh) * mNumberOfUsedSubMeshes);

		// Read in optional skeleton
		mNumberOfBones = meshHeader.numberOfBones;
		if (mNumberOfBones > 0)
		{
			// Read in the skeleton data in a single burst
			const RECore::uint32 numberOfSkeletonDataBytes = (sizeof(RECore::uint8) + sizeof(RECore::uint32) + sizeof(glm::mat4) * 2) * mNumberOfBones;
			mSkeletonData = new RECore::uint8[numberOfSkeletonDataBytes + (sizeof(glm::mat4) + SkeletonResource::NUMBER_OF_BONE_SPACE_DATA_BYTES) * mNumberOfBones];	// "RERenderer::SkeletonResource::mGlobalBoneMatrices" & "RERenderer::SkeletonResource::mBoneSpaceData" aren't serialized
			mMemoryFile.read(mSkeletonData, numberOfSkeletonDataBytes);
		}

		// Can we create the RHI resource asynchronous as well?
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			createVertexArrays();
		}
	}

	bool MeshResourceLoader::onDispatch()
	{
		// Create vertex array object (VAO)
		if (!mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			createVertexArrays();
		}
		mMeshResource->setVertexArray(mVertexArray, mPositionOnlyVertexArray);

		{ // Create sub-meshes
			MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
			SubMeshes& subMeshes = mMeshResource->getSubMeshes();
			subMeshes.resize(mNumberOfUsedSubMeshes);
			for (RECore::uint32 i = 0; i < mNumberOfUsedSubMeshes; ++i)
			{
				// Get source and destination sub-mesh references
				SubMesh& subMesh = subMeshes[i];
				const v1Mesh::SubMesh& v1SubMesh = mSubMeshes[i];

				// Setup sub-mesh
				MaterialResourceId materialResourceId = RECore::getInvalid<MaterialResourceId>();
				materialResourceManager.loadMaterialResourceByAssetId(v1SubMesh.materialAssetId, materialResourceId);
				subMesh.setMaterialResourceId(materialResourceId);
				subMesh.setStartIndexLocation(v1SubMesh.startIndexLocation);
				subMesh.setNumberOfIndices(v1SubMesh.numberOfIndices);

				// Sanity check
				RHI_ASSERT(RECore::isValid(subMesh.getMaterialResourceId()), "Invalid sub mesh material resource ID")
			}
		}

		// Optional skeleton
		if (mNumberOfBones > 0)
		{
			SkeletonResourceManager& skeletonResourceManager = mRenderer.getSkeletonResourceManager();

			// Get/create skeleton resource
			SkeletonResource* skeletonResource = nullptr;
			if (RECore::isValid(mMeshResource->getSkeletonResourceId()))
			{
				// Reuse existing skeleton resource
				skeletonResource = &skeletonResourceManager.getById(mMeshResource->getSkeletonResourceId());
				skeletonResource->clearSkeletonData();
			}
			else
			{
				// Create new skeleton resource
				const SkeletonResourceId skeletonResourceId = skeletonResourceManager.createSkeletonResourceByAssetId(getAsset().assetId);
				mMeshResource->setSkeletonResourceId(skeletonResourceId);
				skeletonResource = &skeletonResourceManager.getById(skeletonResourceId);
			}

			// Pass on the skeleton data to the skeleton resource
			skeletonResource->mNumberOfBones = mNumberOfBones;
			skeletonResource->mBoneParentIndices = mSkeletonData;
			mSkeletonData += sizeof(RECore::uint8) * mNumberOfBones;
			skeletonResource->mBoneIds = reinterpret_cast<RECore::uint32*>(mSkeletonData);
			mSkeletonData += sizeof(RECore::uint32) * mNumberOfBones;
			skeletonResource->mLocalBoneMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mBoneOffsetMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mGlobalBoneMatrices = reinterpret_cast<glm::mat4*>(mSkeletonData);
			mSkeletonData += sizeof(glm::mat4) * mNumberOfBones;
			skeletonResource->mBoneSpaceData = mSkeletonData;
			skeletonResource->localToGlobalPose();

			// Skeleton data has been passed on
			mSkeletonData = nullptr;
		}

		// Fully loaded?
		return isFullyLoaded();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MeshResourceLoader::MeshResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer) :
		IMeshResourceLoader(resourceManager, renderer),
		mBufferManager(renderer.getBufferManager()),
		mVertexArray(nullptr),
		mPositionOnlyVertexArray(nullptr),
		// Temporary vertex buffer
		mNumberOfVertexBufferDataBytes(0),
		mNumberOfUsedVertexBufferDataBytes(0),
		mVertexBufferData(nullptr),
		// Temporary index buffer
		mIndexBufferFormat(0),
		mNumberOfIndexBufferDataBytes(0),
		mNumberOfUsedIndexBufferDataBytes(0),
		mIndexBufferData(nullptr),
		// Temporary position-only index buffer
		mNumberOfPositionOnlyIndexBufferDataBytes(0),
		mNumberOfUsedPositionOnlyIndexBufferDataBytes(0),
		mPositionOnlyIndexBufferData(nullptr),
		// Temporary vertex attributes
		mNumberOfVertexAttributes(0),
		mNumberOfUsedVertexAttributes(0),
		mVertexAttributes(nullptr),
		// Temporary sub-meshes
		mNumberOfSubMeshes(0),
		mNumberOfUsedSubMeshes(0),
		mSubMeshes(nullptr),
		// Optional temporary skeleton
		mNumberOfBones(0),
		mSkeletonData(nullptr)
	{
		// Nothing here
	}

	MeshResourceLoader::~MeshResourceLoader()
	{
		delete [] mVertexBufferData;
		delete [] mIndexBufferData;
		delete [] mPositionOnlyIndexBufferData;
		delete [] mVertexAttributes;
		delete [] mSubMeshes;
		delete [] mSkeletonData;	// In case the mesh resource loaded was never dispatched
	}

	void MeshResourceLoader::createVertexArrays()
	{
		// Create the vertex buffer object (VBO)
		RERHI::RHIVertexBufferPtr vertexBuffer(mBufferManager.createVertexBuffer(mNumberOfUsedVertexBufferDataBytes, mVertexBufferData, 0, RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename)));

		// Create the index buffer object (IBO)
		RERHI::RHIIndexBufferPtr indexBuffer(mBufferManager.createIndexBuffer(mNumberOfUsedIndexBufferDataBytes, mIndexBufferData, 0, RERHI::BufferUsage::STATIC_DRAW, static_cast<RERHI::IndexBufferFormat::Enum>(mIndexBufferFormat) RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename)));

		// Create vertex array object (VAO)
		const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer, mRenderer.getMeshResourceManager().getDrawIdVertexBufferPtr() };
		const RERHI::VertexAttributes vertexAttributes(mNumberOfUsedVertexAttributes, mVertexAttributes);
		mVertexArray = mBufferManager.createVertexArray(vertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

		// Create the position-only vertex array object (VAO)
		if (mNumberOfUsedPositionOnlyIndexBufferDataBytes > 0)
		{
			// Create the index buffer object (IBO)
			indexBuffer = mBufferManager.createIndexBuffer(mNumberOfUsedPositionOnlyIndexBufferDataBytes, mPositionOnlyIndexBufferData, 0, RERHI::BufferUsage::STATIC_DRAW, static_cast<RERHI::IndexBufferFormat::Enum>(mIndexBufferFormat) RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

			// Create vertex array object (VAO)
			mPositionOnlyVertexArray = mBufferManager.createVertexArray(vertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));
		}
		else
		{
			mPositionOnlyVertexArray = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
