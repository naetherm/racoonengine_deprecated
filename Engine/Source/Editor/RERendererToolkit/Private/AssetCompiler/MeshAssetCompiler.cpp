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
#include "RERendererToolkit/AssetCompiler/MeshAssetCompiler.h"
#include "RERendererToolkit/Helper/AssimpLogStream.h"
#include "RERendererToolkit/Helper/AssimpIOSystem.h"
#include "RERendererToolkit/Helper/AssimpHelper.h"
#include "RERendererToolkit/Helper/CacheManager.h"
#include "RERendererToolkit/Helper/StringHelper.h"
#include "RERendererToolkit/Helper/JsonHelper.h"
#include "RERendererToolkit/Context.h"

#include <RECore/Asset/AssetPackage.h>
#include <RECore/Math/Math.h>
#include <RECore/File/IFile.h>
#include <RECore/File/MemoryFile.h>
#include <RECore/File/IFileManager.h>
#include <RECore/File/FileSystemHelper.h>
#include <RERenderer/Resource/Mesh/MeshResource.h>
#include <RERenderer/Resource/Mesh/Loader/MeshFileFormat.h>
#include <RECore/Memory/Memory.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4061)	// warning C4061: enumerator 'FORCE_32BIT' in switch of enum 'aiMetadataType' is not explicitly handled by a case label
	PRAGMA_WARNING_DISABLE_MSVC(4619)	// warning C4619: #pragma warning: there is no warning number '4351'
	PRAGMA_WARNING_DISABLE_MSVC(5219)	// warning C5219: implicit conversion from 'int' to 'float', possible loss of data
	#include <assimp/scene.h>
	#include <assimp/Importer.hpp>
PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'initializing': conversion from 'unsigned int' to 'int', signed/unsigned mismatch
	#include <meshoptimizer.h>
PRAGMA_WARNING_POP

#include <mikktspace.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: '=': conversion from 'int' to 'rapidjson::internal::BigInteger::Type', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'rapidjson::GenericMember<Encoding,Allocator>': copy constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '__GNUC__' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	PRAGMA_WARNING_DISABLE_MSVC(4774)	// warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
	PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
	#include <rapidjson/document.h>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{

	namespace MikkTSpace
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		SMikkTSpaceContext g_MikkTSpaceContext;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] int getNumFaces(const SMikkTSpaceContext* pContext)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			return static_cast<int>(assimpMesh->mNumFaces);
		}

		[[nodiscard]] int getNumVerticesOfFace(const SMikkTSpaceContext* pContext, const int iFace)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			return static_cast<int>(assimpMesh->mFaces[iFace].mNumIndices);
		}

		void getPosition(const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			const aiVector3D& assimpVertex = assimpMesh->mVertices[assimpMesh->mFaces[iFace].mIndices[iVert]];
			fvPosOut[0] = assimpVertex.x;
			fvPosOut[1] = assimpVertex.y;
			fvPosOut[2] = assimpVertex.z;
		}

		void getNormal(const SMikkTSpaceContext* pContext, float fvNormOut[], const int iFace, const int iVert)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			const aiVector3D& assimpNormal = assimpMesh->mNormals[assimpMesh->mFaces[iFace].mIndices[iVert]];
			fvNormOut[0] = assimpNormal.x;
			fvNormOut[1] = assimpNormal.y;
			fvNormOut[2] = assimpNormal.z;
		}

		void getTexCoord(const SMikkTSpaceContext* pContext, float fvTexcOut[], const int iFace, const int iVert)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			const aiVector3D& assimpTexCoord = assimpMesh->mTextureCoords[0][assimpMesh->mFaces[iFace].mIndices[iVert]];
			fvTexcOut[0] = assimpTexCoord.x;
			fvTexcOut[1] = assimpTexCoord.y;
		}

		void setTSpace(const SMikkTSpaceContext* pContext, const float fvTangent[], const float fvBiTangent[], const float, const float, const tbool, const int iFace, const int iVert)
		{
			const aiMesh* assimpMesh = static_cast<const aiMesh*>(pContext->m_pUserData);
			const unsigned int index = assimpMesh->mFaces[iFace].mIndices[iVert];

			{ // Tangent
				aiVector3D& assimpTangent = assimpMesh->mTangents[index];
				assimpTangent.x = fvTangent[0];
				assimpTangent.y = fvTangent[1];
				assimpTangent.z = fvTangent[2];
			}

			{ // Binormal
				aiVector3D& assimpBinormal = assimpMesh->mBitangents[index];
				assimpBinormal.x = fvBiTangent[0];
				assimpBinormal.y = fvBiTangent[1];
				assimpBinormal.z = fvBiTangent[2];
			}
		}


	} // MikkTSpace

	namespace meshoptimizer
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		//RECore::IAllocator* allocator = nullptr;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void* allocate(size_t numberOfBytes) noexcept
		{
			//ASSERT(nullptr != allocator, "Invalid allocator")
			return RECore::Memory::reallocate(nullptr, 0, numberOfBytes, 1);
		}

		void deallocate(void* data) noexcept
		{
			//ASSERT(nullptr != allocator, "Invalid allocator")
      RECore::Memory::reallocate(data, 0, 0, 1);
		}


	} // meshoptimizer

	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr uint8_t NUMBER_OF_BYTES_PER_VERTEX = 28;										///< Number of bytes per vertex (3 float position, 2 float texture coordinate, 4 short QTangent)
		static constexpr uint8_t NUMBER_OF_BYTES_PER_SKINNED_VERTEX = NUMBER_OF_BYTES_PER_VERTEX + 8;	///< Number of bytes per skinned vertex (+4 byte bone indices, +4 byte bone weights)
		typedef std::vector<RERenderer::v1Mesh::SubMesh> SubMeshes;
		typedef std::unordered_map<std::string, RECore::AssetId> MaterialNameToAssetId;


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class Skeleton final
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			uint8_t		 numberOfBones;			///< Number of bones
			// Structure-of-arrays (SoA)
			uint8_t*	 boneParentIndices;		///< Cache friendly depth-first rolled up bone parent indices, null pointer only in case of horrible error, free the memory if no longer required
			uint32_t*	 boneIds;				///< Cache friendly depth-first rolled up bone IDs ("RECore::StringId" on bone name), null pointer only in case of horrible error, don't free the memory because it's owned by "boneParentIndices"
			aiMatrix4x4* localBoneMatrices;		///< Cache friendly depth-first rolled up local bone matrices, null pointer only in case of horrible error, don't free the memory because it's owned by "boneParentIndices"
			aiMatrix4x4* boneOffsetMatrices;	///< Cache friendly depth-first rolled up bone offset matrices (object space to bone space), null pointer only in case of horrible error, free the memory if no longer required


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			Skeleton(uint8_t _numberOfBones, const aiNode& assimpNode) :
				numberOfBones(_numberOfBones),
				boneParentIndices(nullptr),
				boneIds(nullptr),
				localBoneMatrices(nullptr),
				boneOffsetMatrices(nullptr)
			{
				if (numberOfBones > 0)
				{
					// To be able to serialize the data in a single burst we need to have the skeleton data sequential in memory
					uint8_t* skeletonData = new uint8_t[getNumberOfSkeletonDataBytes()];
					boneParentIndices = skeletonData;
					skeletonData += sizeof(uint8_t) * numberOfBones;
					boneIds = reinterpret_cast<uint32_t*>(skeletonData);
					skeletonData += sizeof(uint32_t) * numberOfBones;
					localBoneMatrices = reinterpret_cast<aiMatrix4x4*>(skeletonData);
					skeletonData += sizeof(aiMatrix4x4) * numberOfBones;
					boneOffsetMatrices = reinterpret_cast<aiMatrix4x4*>(skeletonData);

					// OGRE: The scene root node has no name
					if (0 == assimpNode.mName.length)
					{
						if (1 != assimpNode.mNumChildren)
						{
							throw std::runtime_error("There can be only single root bone");
						}
						fillSkeletonRecursive(*assimpNode.mChildren[0], 0, 0);

						// Some Assimp importers like the OGRE one compensate coordinate system differences by setting a root node transform, so we need to take this into account
						localBoneMatrices[0] = assimpNode.mTransformation * localBoneMatrices[0];
					}

					// FBX: The scene root node name is "RootNode"
					else if (assimpNode.mName == aiString("RootNode"))
					{
						// TODO(naetherm) Skeleton support is under construction
						NOP;
					}

					// MD5: The MD5 bones hierarchy is stored inside an Assimp node names "<MD5_Hierarchy>"
					else if (assimpNode.mName == aiString("<MD5_Root>"))
					{
						for (uint32_t i = 0; i < assimpNode.mNumChildren; ++i)
						{
							const aiNode* assimpChildNode = assimpNode.mChildren[i];
							if (assimpChildNode->mName == aiString("<MD5_Hierarchy>"))
							{
								if (1 != assimpChildNode->mNumChildren)
								{
									throw std::runtime_error("\"<MD5_Hierarchy>\" can only have a single root bone");
								}
								fillSkeletonRecursive(*assimpChildNode->mChildren[0], 0, 0);

								// Some Assimp importers like the MD5 one compensate coordinate system differences by setting a root node transform, so we need to take this into account
								localBoneMatrices[0] = assimpNode.mTransformation * localBoneMatrices[0];
								break;
							}
						}
					}
				}
			}

			inline ~Skeleton()
			{
				delete [] getSkeletonData();
			}

			[[nodiscard]] inline uint32_t getNumberOfSkeletonDataBytes() const
			{
				return (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(aiMatrix4x4) * 2) * numberOfBones;
			}

			[[nodiscard]] inline const uint8_t* getSkeletonData() const
			{
				return boneParentIndices;
			}

			[[nodiscard]] uint32_t getBoneIndexByBoneId(uint32_t boneId) const
			{
				// TODO(naetherm) Maybe it makes sense to store the bone IDs in some order to speed up the following
				for (uint8_t boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
				{
					if (boneIds[boneIndex] == boneId)
					{
						return boneIndex;
					}
				}
				return RECore::getInvalid<uint32_t>();
			}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			uint8_t fillSkeletonRecursive(const aiNode& assimpNode, uint8_t parentBoneIndex, uint8_t currentBoneIndex)
			{
				// Sanity check
				const uint32_t boneId = RECore::StringId::calculateFNV(assimpNode.mName.C_Str());
				if (RECore::isValid(getBoneIndexByBoneId(boneId)))
				{
					throw std::runtime_error(std::string("Assimp bone name \"") + assimpNode.mName.C_Str() + "\" is not unique");
				}

				// Gather bone data
				boneParentIndices[currentBoneIndex] = parentBoneIndex;
				boneIds[currentBoneIndex] = boneId;
				localBoneMatrices[currentBoneIndex] = assimpNode.mTransformation;
				parentBoneIndex = currentBoneIndex;
				++currentBoneIndex;

				// Loop through the child bones
				for (uint32_t i = 0; i < assimpNode.mNumChildren; ++i)
				{
					currentBoneIndex = fillSkeletonRecursive(*assimpNode.mChildren[i], parentBoneIndex, currentBoneIndex);
				}

				// Done
				return currentBoneIndex;
			}


		};


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the total number of vertices and indices by using a given Assimp node
		*
		*  @param[in] input
		*    Asset compiler input
		*  @param[in] materialNameToAssetId
		*    Material name to asset ID mapping information
		*  @param[in] assimpScene
		*    Assimp scene
		*  @param[in] assimpNode
		*    Assimp node to gather the data from
		*  @param[out] numberOfVertices
		*    Receives the number of vertices
		*  @param[out] numberOfIndices
		*    Receives the number of indices
		*  @param[out] subMeshes
		*    Receives the sub-meshes data
		*/
		void getNumberOfVerticesAndIndicesRecursive(const RERendererToolkit::IAssetCompiler::Input& input, const MaterialNameToAssetId& materialNameToAssetId, const aiScene& assimpScene, const aiNode& assimpNode, uint32_t& numberOfVertices, uint32_t& numberOfIndices, SubMeshes& subMeshes)
		{
			// Loop through all meshes this node is using
			for (uint32_t i = 0; i < assimpNode.mNumMeshes; ++i)
			{
				// Get the used mesh
				const aiMesh& assimpMesh = *assimpScene.mMeshes[assimpNode.mMeshes[i]];

				// Update the number of vertices
				numberOfVertices += assimpMesh.mNumVertices;

				// Loop through all mesh faces and update the number of indices
				const uint32_t previousNumberOfIndices = numberOfIndices;
				for (uint32_t j = 0; j < assimpMesh.mNumFaces; ++j)
				{
					numberOfIndices += assimpMesh.mFaces[j].mNumIndices;
				}

				{ // Add sub-mesh
					// Get the source material asset ID
					RECore::AssetId materialAssetId = RECore::getInvalid<RECore::AssetId>();
					aiString materialName;
					const aiMaterial* assimpMaterial = assimpScene.mMaterials[assimpMesh.mMaterialIndex];
					{
						assimpMaterial->Get(AI_MATKEY_NAME, materialName);
            printf("Found Materialname: %s\n", materialName.C_Str());
						MaterialNameToAssetId::const_iterator iterator = materialNameToAssetId.find(materialName.C_Str());
						if (materialNameToAssetId.cend() != iterator)
						{
							materialAssetId = iterator->second;
						}
						else
						{
							if (!RERendererToolkit::StringHelper::isSourceAssetIdAsString(materialName.C_Str()))
							{
								// If we're in luck, the diffuse texture 0 stores the material name
								assimpMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), materialName);
							}
							if (materialName.length > 0 && nullptr == strstr(materialName.C_Str(), AI_DEFAULT_MATERIAL_NAME))
							{
								iterator = materialNameToAssetId.find(materialName.C_Str());
								if (materialNameToAssetId.cend() != iterator)
								{
									materialAssetId = iterator->second;
								}
								else
								{
									materialAssetId = RERendererToolkit::StringHelper::getAssetIdByString(materialName.C_Str(), input);
								}
							}
						}
					}

					// Add sub-mesh
					if (RECore::isValid(materialAssetId))
					{
						RERenderer::v1Mesh::SubMesh subMesh;
						subMesh.materialAssetId	   = materialAssetId;
						subMesh.startIndexLocation = previousNumberOfIndices;
						subMesh.numberOfIndices	   = numberOfIndices - previousNumberOfIndices;
						subMeshes.push_back(subMesh);
					}
					else
					{
						assimpMaterial->Get(AI_MATKEY_NAME, materialName);
						throw std::runtime_error("Failed to determine the material asset ID for material \"" + std::string(materialName.C_Str()) + '\"');
					}
				}
			}

			// Loop through all child nodes recursively
			for (uint32_t i = 0; i < assimpNode.mNumChildren; ++i)
			{
				getNumberOfVerticesAndIndicesRecursive(input, materialNameToAssetId, assimpScene, *assimpNode.mChildren[i], numberOfVertices, numberOfIndices, subMeshes);
			}
		}

		/**
		*  @brief
		*    Fill the mesh data recursively
		*
		*  @param[in]  assimpScene
		*    Assimp scene
		*  @param[in]  assimpNode
		*    Assimp node to gather the data from
		*  @param[in]  skeleton
		*    Skeleton instance
		*  @param[in]  numberOfBytesPerVertex
		*    Number of bytes per vertex
		*  @param[in]  vertexBuffer
		*    Vertex buffer to fill
		*  @param[in]  indexBuffer
		*    Index buffer to fill
		*  @param[in]  assimpTransformation
		*    Current absolute Assimp transformation matrix (local to global space)
		*  @param[out] minimumBoundingBoxPosition
		*    Receives the minimum bounding box position
		*  @param[out] maximumBoundingBoxPosition
		*    Receives the maximum bounding box position
		*  @param[out] numberOfVertices
		*    Receives the number of processed vertices
		*  @param[out] numberOfIndices
		*    Receives the number of processed indices
		*/
		void fillMeshRecursive(const aiScene& assimpScene, const aiNode& assimpNode, const Skeleton& skeleton, uint8_t numberOfBytesPerVertex, uint8_t* vertexBuffer, uint32_t* indexBuffer, const aiMatrix4x4& assimpTransformation,
							   glm::vec3& minimumBoundingBoxPosition, glm::vec3& maximumBoundingBoxPosition, uint32_t& numberOfVertices, uint32_t& numberOfIndices)
		{
			// Get the absolute transformation matrix of this Assimp node
			const aiMatrix4x4 currentAssimpTransformation = assimpTransformation * assimpNode.mTransformation;
			const aiMatrix3x3 currentAssimpNormalTransformation = aiMatrix3x3(currentAssimpTransformation);

			// Loop through all meshes this node is using
			for (uint32_t i = 0; i < assimpNode.mNumMeshes; ++i)
			{
				// Get the used mesh
				aiMesh& assimpMesh = *assimpScene.mMeshes[assimpNode.mMeshes[i]];

				// Use "MikkTSpace" by Morten S. Mikkelsen ( http://mmikkelsen3d.blogspot.com/ ) for semi-standard tangent space generation and overwrite what Assimp calculated (see http://www.mikktspace.com/ for background information)
				if (0 != assimpMesh.mNumUVComponents[0] && nullptr != assimpMesh.mTangents && nullptr != assimpMesh.mBitangents)
				{
					MikkTSpace::g_MikkTSpaceContext.m_pUserData = reinterpret_cast<void*>(&assimpMesh);
					if (genTangSpaceDefault(&MikkTSpace::g_MikkTSpaceContext) == 0)
					{
						throw std::runtime_error("MikkTSpace for semi-standard tangent space generation failed");
					}
				}

				// Get the start vertex inside the our vertex buffer
				const uint32_t starVertex = numberOfVertices;

				{ // Loop through the Assimp mesh vertices
					uint8_t* RESTRICT currentVertexBuffer = vertexBuffer + numberOfVertices * numberOfBytesPerVertex;
					for (uint32_t j = 0; j < assimpMesh.mNumVertices; ++j)
					{
						uint8_t* RESTRICT currentVertex = currentVertexBuffer + j * numberOfBytesPerVertex;

						{ // 32 bit position
							// Get the Assimp mesh vertex position
							aiVector3D assimpVertex = assimpMesh.mVertices[j];

							// Transform the Assimp mesh vertex position into global space
							assimpVertex *= currentAssimpTransformation;

							// Set our vertex buffer position
							float* RESTRICT currentVertexBufferFloat = reinterpret_cast<float*>(currentVertex);
							*currentVertexBufferFloat = assimpVertex.x;
							++currentVertexBufferFloat;
							*currentVertexBufferFloat = assimpVertex.y;
							++currentVertexBufferFloat;
							*currentVertexBufferFloat = assimpVertex.z;
							currentVertex += sizeof(float) * 3;

							{ // Update minimum and maximum bounding box position
								const glm::vec3 glmVertex(assimpVertex.x, assimpVertex.y, assimpVertex.z);
								minimumBoundingBoxPosition = glm::min(minimumBoundingBoxPosition, glmVertex);
								maximumBoundingBoxPosition = glm::max(maximumBoundingBoxPosition, glmVertex);
							}
						}

						// 32 bit texture coordinate
						if (0 != assimpMesh.mNumUVComponents[0])
						{
							// Get the Assimp mesh vertex texture coordinate
							aiVector3D assimpTexCoord = assimpMesh.mTextureCoords[0][j];

							// Set our vertex buffer 32 bit texture coordinate
							float* RESTRICT currentVertexBufferFloat = reinterpret_cast<float*>(currentVertex);
							*currentVertexBufferFloat = assimpTexCoord.x;
							++currentVertexBufferFloat;
							*currentVertexBufferFloat = assimpTexCoord.y;
							currentVertex += sizeof(float) * 2;
						}
						else
						{
							// Set our vertex buffer 32 bit texture coordinate
							float* RESTRICT currentVertexBufferFloat = reinterpret_cast<float*>(currentVertex);
							*currentVertexBufferFloat = 0.0f;
							++currentVertexBufferFloat;
							*currentVertexBufferFloat = 0.0f;
							currentVertex += sizeof(float) * 2;
						}

						{ // 16 bit QTangent
						  // - QTangent basing on http://dev.theomader.com/qtangents/ "QTangents" which is basing on
						  //   http://www.crytek.com/cryengine/presentations/spherical-skinning-with-dual-quaternions-and-qtangents "Spherical Skinning with Dual-Quaternions and QTangents"
							// Get the Assimp mesh vertex tangent, binormal and normal
							aiVector3D tangent(1.0f, 0.0f, 0.0f);
							aiVector3D binormal(0.0f, 1.0f, 0.0f);
							aiVector3D normal = assimpMesh.mNormals[j];
							if (0 != assimpMesh.mNumUVComponents[0] && nullptr != assimpMesh.mTangents && nullptr != assimpMesh.mBitangents)
							{
								tangent = assimpMesh.mTangents[j];
								binormal = assimpMesh.mBitangents[j];
							}

							// Transform the Assimp mesh vertex data into global space and re-normalize since the transform might contain scale
							tangent *= currentAssimpNormalTransformation;
							binormal *= currentAssimpNormalTransformation;
							normal *= currentAssimpNormalTransformation;
							tangent.Normalize();
							binormal.Normalize();
							normal.Normalize();

							// Generate tangent frame rotation matrix
							glm::mat3 tangentFrame(
								tangent.x,  tangent.y,  tangent.z,
								binormal.x, binormal.y, binormal.z,
								normal.x,   normal.y,   normal.z
							);

							// Calculate tangent frame quaternion
							const glm::quat tangentFrameQuaternion = RECore::Math::calculateTangentFrameQuaternion(tangentFrame);

							// Set our vertex buffer 16 bit QTangent
							short* RESTRICT currentVertexBufferShort = reinterpret_cast<short*>(currentVertex);
							*currentVertexBufferShort = static_cast<short>(tangentFrameQuaternion.x * SHRT_MAX);
							++currentVertexBufferShort;
							*currentVertexBufferShort = static_cast<short>(tangentFrameQuaternion.y * SHRT_MAX);
							++currentVertexBufferShort;
							*currentVertexBufferShort = static_cast<short>(tangentFrameQuaternion.z * SHRT_MAX);
							++currentVertexBufferShort;
							*currentVertexBufferShort = static_cast<short>(tangentFrameQuaternion.w * SHRT_MAX);
							currentVertex += sizeof(short) * 4;
						}
					}
				}

				// Process the Assimp bones, if there are any to start with
				if (assimpMesh.mNumBones > 0 && skeleton.numberOfBones > 0)
				{
					std::vector<uint8_t> numberOfWeightsPerVertex;
					numberOfWeightsPerVertex.resize(assimpMesh.mNumVertices);
					memset(numberOfWeightsPerVertex.data(), 0, sizeof(uint8_t) * assimpMesh.mNumVertices);

					// Loop through the Assimp bones
					uint8_t* RESTRICT currentVertexBuffer = vertexBuffer + numberOfVertices * numberOfBytesPerVertex;
					for (unsigned int bone = 0; bone < assimpMesh.mNumBones; ++bone)
					{
						const aiBone* assimpBone = assimpMesh.mBones[bone];
						const uint32_t boneIndex = skeleton.getBoneIndexByBoneId(RECore::StringId::calculateFNV(assimpBone->mName.C_Str()));
						if (RECore::isInvalid(boneIndex))
						{
							throw std::runtime_error(std::string("Invalid Assimp bone name \"") + assimpBone->mName.C_Str() + '\"');
						}
						skeleton.boneOffsetMatrices[boneIndex] = assimpBone->mOffsetMatrix;

						// Loop through the Assimp bone weights
						for (unsigned int weight = 0; weight < assimpBone->mNumWeights; ++weight)
						{
							const aiVertexWeight& assimpVertexWeight = assimpBone->mWeights[weight];

							// Does this vertex still have a free weight slot?
							const uint8_t numberOfVertexWeights = numberOfWeightsPerVertex[assimpVertexWeight.mVertexId];
							if (numberOfVertexWeights < 4)
							{
								uint8_t* RESTRICT currentVertex = currentVertexBuffer + assimpVertexWeight.mVertexId * numberOfBytesPerVertex;

								// Skip 32 bit position, 32 bit texture coordinate and 16 bit QTangent
								currentVertex += sizeof(float) * 3 + sizeof(float) * 2 + sizeof(short) * 4;

								{ // 8 bit bone indices
									uint8_t* boneIndices = currentVertex;
									boneIndices[numberOfVertexWeights] = static_cast<uint8_t>(boneIndex);
									currentVertex += sizeof(uint8_t) * 4;
								}

								{ // 8 bit bone weights
									uint8_t* boneWeights = reinterpret_cast<uint8_t*>(currentVertex);
									boneWeights[numberOfVertexWeights] = static_cast<uint8_t>(assimpVertexWeight.mWeight * 255);
								}

								// Update the number of vertex weights
								++numberOfWeightsPerVertex[assimpVertexWeight.mVertexId];
							}
						}
					}
				}

				// Update the number of processed vertices
				numberOfVertices += assimpMesh.mNumVertices;

				// Loop through all Assimp mesh faces
				uint32_t* currentIndexBuffer = indexBuffer + numberOfIndices;
				for (uint32_t j = 0; j < assimpMesh.mNumFaces; ++j)
				{
					// Get the Assimp face
					const aiFace& assimpFace = assimpMesh.mFaces[j];

					// Loop through all indices of the Assimp face and set our indices
					for (uint32_t assimpIndex = 0; assimpIndex < assimpFace.mNumIndices; ++assimpIndex, ++currentIndexBuffer)
					{
						//					  Assimp mesh vertex index			 Where the Assimp mesh starts within the our vertex buffer
						*currentIndexBuffer = assimpFace.mIndices[assimpIndex] + starVertex;
					}

					// Update the number if processed indices
					numberOfIndices += assimpFace.mNumIndices;
				}
			}

			// Loop through all child nodes recursively
			for (uint32_t assimpChild = 0; assimpChild < assimpNode.mNumChildren; ++assimpChild)
			{
				fillMeshRecursive(assimpScene, *assimpNode.mChildren[assimpChild], skeleton, numberOfBytesPerVertex, vertexBuffer, indexBuffer, currentAssimpTransformation, minimumBoundingBoxPosition, maximumBoundingBoxPosition, numberOfVertices, numberOfIndices);
			}
		}

		/**
		*  @brief
		*    Write index buffer data into a given file
		*
		*  @param[in] indexBufferFormat
		*    Index buffer format
		*  @param[in] indexBufferData
		*    Index buffer data to write into the given file
		*  @param[in,out] temporaryShortIndexBufferData
		*    Temporary short index buffer data to be able to reuse allocated memory
		*  @param[out] file
		*    File to write into
		*/
		void writeIndexBufferData(RERHI::IndexBufferFormat::Enum indexBufferFormat, const std::vector<uint32_t>& indexBufferData, std::vector<uint16_t>& temporaryShortIndexBufferData, RECore::IFile& file)
		{
			const size_t numberOfIndices = indexBufferData.size();
			if (RERHI::IndexBufferFormat::UNSIGNED_INT == indexBufferFormat)
			{
				// Dump the 32-bit indices we have in memory
				file.write(indexBufferData.data(), sizeof(uint32_t) * numberOfIndices);
			}
			else
			{
				// Convert the 32-bit indices we have in memory to 16-bit indices before writing them into a given file
				temporaryShortIndexBufferData.resize(numberOfIndices);
				uint16_t* RESTRICT shortIndexBufferData = temporaryShortIndexBufferData.data();
				for (size_t i = 0; i < numberOfIndices; ++i)
				{
					shortIndexBufferData[i] = static_cast<uint16_t>(indexBufferData[i]);
				}
				file.write(shortIndexBufferData, sizeof(uint16_t) * numberOfIndices);
			}
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{


	//[-------------------------------------------------------]
	//[ Public virtual RERendererToolkit::IAssetCompiler methods ]
	//[-------------------------------------------------------]
	std::string MeshAssetCompiler::getVirtualOutputAssetFilename(const Input& input, const Configuration&) const
	{
		return input.virtualAssetOutputDirectory + '/' + std_filesystem::path(input.virtualAssetFilename).stem().generic_string() + ".mesh";
	}

	bool MeshAssetCompiler::checkIfChanged(const Input& input, const Configuration& configuration) const
	{
		const std::string virtualInputFilename = input.virtualAssetInputDirectory + '/' + JsonHelper::getAssetInputFileByRapidJsonDocument(configuration.rapidJsonDocumentAsset);
		return input.cacheManager.checkIfFileIsModified(configuration.rhiTarget, input.virtualAssetFilename, {virtualInputFilename}, getVirtualOutputAssetFilename(input, configuration), RERenderer::v1Mesh::FORMAT_VERSION);
	}

	void MeshAssetCompiler::compile(const Input& input, const Configuration& configuration) const
	{
		// Get relevant data
		const rapidjson::Value& rapidJsonValueMeshAssetCompiler = configuration.rapidJsonDocumentAsset["Asset"]["Compiler"];
		const std::string virtualInputFilename = input.virtualAssetInputDirectory + '/' + JsonHelper::getAssetInputFileByRapidJsonValue(rapidJsonValueMeshAssetCompiler);
		const std::string virtualOutputAssetFilename = getVirtualOutputAssetFilename(input, configuration);

		// Ask the cache manager whether or not we need to compile the source file (e.g. source changed or target not there)
		CacheManager::CacheEntries cacheEntries;
		if (input.cacheManager.needsToBeCompiled(configuration.rhiTarget, input.virtualAssetFilename, virtualInputFilename, virtualOutputAssetFilename, RERenderer::v1Mesh::FORMAT_VERSION, cacheEntries))
		{
			 RECore::MemoryFile memoryFile(0, 42 * 1024);

			// Setup "MikkTSpace" by Morten S. Mikkelsen ( http://mmikkelsen3d.blogspot.com/ ) for semi-standard tangent space generation (see http://www.mikktspace.com/ for background information)
			SMikkTSpaceInterface mikkTSpaceInterface;
			mikkTSpaceInterface.m_getNumFaces		   = MikkTSpace::getNumFaces;
			mikkTSpaceInterface.m_getNumVerticesOfFace = MikkTSpace::getNumVerticesOfFace;
			mikkTSpaceInterface.m_getPosition		   = MikkTSpace::getPosition;
			mikkTSpaceInterface.m_getNormal			   = MikkTSpace::getNormal;
			mikkTSpaceInterface.m_getTexCoord		   = MikkTSpace::getTexCoord;
			mikkTSpaceInterface.m_setTSpaceBasic	   = nullptr;
			mikkTSpaceInterface.m_setTSpace			   = MikkTSpace::setTSpace;
			MikkTSpace::g_MikkTSpaceContext.m_pInterface = &mikkTSpaceInterface;
			MikkTSpace::g_MikkTSpaceContext.m_pUserData  = nullptr;

			// Create an instance of the Assimp importer class
			AssimpLogStream assimpLogStream;
			Assimp::Importer assimpImporter;
			assimpImporter.SetIOHandler(new AssimpIOSystem(input.context.getFileManager()));
			// assimpImporter.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);	// We're using the "aiProcess_LimitBoneWeights"-flag, 4 is already the default value (don't delete this reminder comment)

			// Load the given mesh
			// -> We're using "MikkTSpace" by Morten S. Mikkelsen ( http://mmikkelsen3d.blogspot.com/ ) for semi-standard tangent space generation (see http://www.mikktspace.com/ for background information)
			// -> "aiProcess_CalcTangentSpace" from Assimp is still used to allocate internal memory and enable Assimp to perform work regarding e.g. shared vertices
			assimpImporter.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);	// Tell ASSIMP that we don't support lines nor points
			const aiScene* assimpScene = assimpImporter.ReadFile(virtualInputFilename.c_str(), AssimpHelper::getAssimpFlagsByRapidJsonValue(rapidJsonValueMeshAssetCompiler, "ImportFlags"));
			if (nullptr != assimpScene && nullptr != assimpScene->mRootNode)
			{
				// Get the optional material name to asset ID mapping information
				::detail::MaterialNameToAssetId materialNameToAssetId;
				if (rapidJsonValueMeshAssetCompiler.HasMember("MaterialNameToAssetId"))
				{
					const rapidjson::Value& rapidJsonValueMaterialNameToAssetId = rapidJsonValueMeshAssetCompiler["MaterialNameToAssetId"];
					for (rapidjson::Value::ConstMemberIterator rapidJsonMemberIterator = rapidJsonValueMaterialNameToAssetId.MemberBegin(); rapidJsonMemberIterator != rapidJsonValueMaterialNameToAssetId.MemberEnd(); ++rapidJsonMemberIterator)
					{
						if (materialNameToAssetId.find(rapidJsonMemberIterator->name.GetString()) == materialNameToAssetId.cend())
						{
							const std::string assetIdAsString = rapidJsonMemberIterator->value.GetString();
							if (assetIdAsString.empty())
							{
								throw std::runtime_error("Mesh asset material name to asset ID mapping entry \"" + std::string(rapidJsonMemberIterator->name.GetString()) + "\" has no material asset ID assigned");
							}
							materialNameToAssetId.emplace(rapidJsonMemberIterator->name.GetString(), StringHelper::getAssetIdByString(assetIdAsString, input));
						}
						else
						{
							throw std::runtime_error("Duplicate entry inside the mesh asset material name to asset ID mapping: \"" + std::string(rapidJsonMemberIterator->name.GetString()) + '\"');
						}
					}
				}

				// Get the number of bones and skeleton
				const uint32_t numberOfBones = AssimpHelper::getNumberOfBones(*assimpScene->mRootNode);
				if (numberOfBones > 255)
				{
					throw std::runtime_error("Maximum number of supported bones is 255");
				}
				::detail::Skeleton skeleton(static_cast<uint8_t>(numberOfBones), *assimpScene->mRootNode);

				// Get the total number of vertices and indices by using the Assimp root node
				uint32_t numberOfVertices = 0;
				uint32_t numberOfIndices = 0;
				::detail::SubMeshes subMeshes;
				::detail::getNumberOfVerticesAndIndicesRecursive(input, materialNameToAssetId, *assimpScene, *assimpScene->mRootNode, numberOfVertices, numberOfIndices, subMeshes);

				if (0 == numberOfVertices)
				{
					throw std::runtime_error("The mesh has no vertices");
				}
				if (subMeshes.empty())
				{
					throw std::runtime_error("The mesh has no sub-meshes");
				}
				if (subMeshes.size() > std::numeric_limits<uint16_t>::max())
				{
					throw std::runtime_error("The maximum number of supported sub-meshes is " + std::to_string(std::numeric_limits<uint16_t>::max()));
				}
				const RERHI::IndexBufferFormat::Enum indexBufferFormat = (numberOfVertices > std::numeric_limits<uint16_t>::max()) ? RERHI::IndexBufferFormat::UNSIGNED_INT : RERHI::IndexBufferFormat::UNSIGNED_SHORT;

				// Is there an optional skeleton?
				const RERHI::VertexAttributes& vertexAttributes = (numberOfBones > 0) ? RERenderer::MeshResource::SKINNED_VERTEX_ATTRIBUTES : RERenderer::MeshResource::VERTEX_ATTRIBUTES;
				const uint8_t numberOfBytesPerVertex = (numberOfBones > 0) ? ::detail::NUMBER_OF_BYTES_PER_SKINNED_VERTEX : ::detail::NUMBER_OF_BYTES_PER_VERTEX;

				// Allocate memory for the local vertex and index buffer data
				// -> Do also initialize the vertex buffer data with zero to handle not filled vertex bone weights
				uint8_t* vertexBufferData = new uint8_t[numberOfBytesPerVertex * numberOfVertices];
				memset(vertexBufferData, 0, numberOfBytesPerVertex * numberOfVertices);
				std::vector<uint32_t> indexBufferData(numberOfIndices);
				std::vector<uint16_t> temporaryShortIndexBufferData;	// Defined here to be able to reuse allocated memory

				// Fill the mesh data recursively
				glm::vec3 minimumBoundingBoxPosition(std::numeric_limits<float>::max());
				glm::vec3 maximumBoundingBoxPosition(std::numeric_limits<float>::lowest());
				{
					uint32_t numberOfFilledVertices = 0;
					uint32_t numberOfFilledIndices  = 0;
					::detail::fillMeshRecursive(*assimpScene, *assimpScene->mRootNode, skeleton, numberOfBytesPerVertex, vertexBufferData, &indexBufferData[0], aiMatrix4x4(), minimumBoundingBoxPosition, maximumBoundingBoxPosition, numberOfFilledVertices, numberOfFilledIndices);
					if (numberOfVertices != numberOfFilledVertices || numberOfIndices != numberOfFilledIndices)
					{
						throw std::runtime_error("Error while recursively filling the mesh data");
					}
				}

				// Paranoid sub-mesh and number of indices sanity check
				#ifdef DEBUG
				{
					uint32_t currentNumberOfIndices = 0;
					for (const RERenderer::v1Mesh::SubMesh& subMesh : subMeshes)
					{
						currentNumberOfIndices += subMesh.numberOfIndices;
					}
					if (currentNumberOfIndices != numberOfIndices)
					{
						throw std::runtime_error("Sub-mesh and number of indices mismatch");
					}
				}
				#endif

				// "meshoptimizer", in-place is supported internally so we don't need to create own vertex and index buffer copies
				std::vector<uint32_t> positionOnlyIndexBufferData;
				uint32_t numberOfLods = 5;	// There's always at least one LOD, namely the original none reduced version
				{
					// "meshoptimizer" configuration
					static constexpr float OVERDRAW_THRESHOLD = 1.01f;	// Allow up to 1% worse ACMR to get more reordering opportunities for overdraw
					{ // Number of LODs
						JsonHelper::optionalIntegerProperty(rapidJsonValueMeshAssetCompiler, "NumberOfLods", numberOfLods);
						if (0 == numberOfLods)
						{
							throw std::runtime_error("There must always be at least one LOD, namely the original none reduced version");
						}
						if (255 < numberOfLods)
						{
							throw std::runtime_error("The maximum number of supported LODs is 255");
						}
					}

					// Set "meshoptimizer" allocator
					//meshoptimizer::allocator = nullptr; //&input.context.getAllocator();
					meshopt_setAllocator(&meshoptimizer::allocate, &meshoptimizer::deallocate);

					// Handle multiple ranges for multiple draw calls
					const uint32_t numberOfSubMeshes = static_cast<uint32_t>(subMeshes.size());
					std::vector<std::vector<uint32_t>>					lodIndexOffsets(numberOfLods);		// LODs first, sub-meshes second
					std::vector<std::vector<uint32_t>>					lodNumberOfIndices(numberOfLods);	// LODs first, sub-meshes second
					std::vector<std::vector<std::vector<uint32_t>>>		lodIndices(numberOfLods);			// LODs first, sub-meshes second
					std::vector<std::vector<RERenderer::v1Mesh::SubMesh>> subMeshLods(numberOfLods);			// LODs first, sub-meshes second, we don't need index 0, but it's easier to just keep it
					for (uint32_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
					{
						lodIndexOffsets[lodIndex].resize(numberOfSubMeshes);
						lodNumberOfIndices[lodIndex].resize(numberOfSubMeshes);
						lodIndices[lodIndex].resize(numberOfSubMeshes);
						subMeshLods[lodIndex].reserve(numberOfSubMeshes);
					}

					// Step one: Create the LOD indices
					for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
					{
						const RERenderer::v1Mesh::SubMesh& subMesh = subMeshes[subMeshIndex];
						uint32_t* subMeshIndexBufferData = &indexBufferData[subMesh.startIndexLocation];
						const uint32_t subMeshNumberOfIndices = subMesh.numberOfIndices;

						// Mesh level of detail (LOD) via "meshoptimizer"
						if (numberOfLods > 1)
						{
							// Generate the LOD levels, with each subsequent LOD using 70% triangles, note that each LOD uses the same (shared) vertex buffer
							lodIndices[0][subMeshIndex].resize(subMeshNumberOfIndices);
							memcpy(lodIndices[0][subMeshIndex].data(), subMeshIndexBufferData, sizeof(uint32_t) * subMeshNumberOfIndices);
							for (size_t lodIndex = 1; lodIndex < numberOfLods; ++lodIndex)
							{
								std::vector<unsigned int>& currentLodIndices = lodIndices[lodIndex][subMeshIndex];

								const float threshold = powf(0.7f, static_cast<float>(lodIndex));
								size_t targetNumberOfIndices = static_cast<size_t>(static_cast<float>(subMeshNumberOfIndices) * threshold) / 3 * 3;
								static constexpr float TARGET_ERROR = 1e-2f;
								float LOD_ERROR = 0.f;

								// We can simplify all the way from base level or from the last result simplifying from the base level sometimes produces better results, but simplifying from last level is faster
								const std::vector<unsigned int>& source = lodIndices[lodIndex - 1][subMeshIndex];
								if (source.size() < targetNumberOfIndices)
								{
									targetNumberOfIndices = source.size();
								}
								currentLodIndices.resize(source.size());
                size_t iii = meshopt_simplifySloppy(currentLodIndices.data(), source.data(), source.size(), reinterpret_cast<const float*>(vertexBufferData), numberOfVertices, numberOfBytesPerVertex, targetNumberOfIndices, TARGET_ERROR);
								currentLodIndices.resize(iii);
							}

							// Optimize each individual LOD for vertex cache & overdraw
							for (size_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
							{
								std::vector<unsigned int>& currentLodIndices = lodIndices[lodIndex][subMeshIndex];
								meshopt_optimizeVertexCache(currentLodIndices.data(), currentLodIndices.data(), currentLodIndices.size(), numberOfVertices);
								meshopt_optimizeOverdraw(currentLodIndices.data(), currentLodIndices.data(), currentLodIndices.size(), reinterpret_cast<const float*>(vertexBufferData), numberOfVertices, numberOfBytesPerVertex, OVERDRAW_THRESHOLD);
							}
						}
						else
						{
							lodIndexOffsets[0][subMeshIndex] = 0;
							lodNumberOfIndices[0][subMeshIndex] = subMeshNumberOfIndices;

							// Vertex cache optimization should go first as it provides starting order for overdraw
							meshopt_optimizeVertexCache(subMeshIndexBufferData, subMeshIndexBufferData, subMeshNumberOfIndices, numberOfVertices);

							// Reorder indices for overdraw, balancing overdraw and vertex cache efficiency
							meshopt_optimizeOverdraw(subMeshIndexBufferData, subMeshIndexBufferData, subMeshNumberOfIndices, reinterpret_cast<const float*>(vertexBufferData), numberOfVertices, numberOfBytesPerVertex, OVERDRAW_THRESHOLD);
						}
					}

					// Step two: Concatenate all LODs into one index buffer
					if (numberOfLods > 1)
					{
						// Get offsets and number of indices
						// Note: The order of concatenation is important - since we optimize the entire index buffer for vertex fetch, putting coarse LODs first makes sure that the
						// vertex range referenced by them is as small as possible. Some GPUs process the entire range referenced by the index buffer region so doing this optimizes
						// the vertex transform cost for coarse LODs. This order also produces much better vertex fetch cache coherency for coarse LODs (since they're essentially
						// optimized first). Somewhat surprisingly, the vertex fetch cache coherency for fine LODs doesn't seem to suffer that much.
						uint32_t totalNumberOfIndices = 0;
						for (int currentLodIndex = static_cast<int>(numberOfLods) - 1; currentLodIndex >= 0; --currentLodIndex)
						{
							for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
							{
								const uint32_t lodIndex = static_cast<uint32_t>(currentLodIndex);
								lodIndexOffsets[lodIndex][subMeshIndex] = totalNumberOfIndices;
								lodNumberOfIndices[lodIndex][subMeshIndex] = static_cast<uint32_t>(lodIndices[lodIndex][subMeshIndex].size());
								totalNumberOfIndices += static_cast<uint32_t>(lodIndices[lodIndex][subMeshIndex].size());
							}
						}

						// Concatenate all LODs into one index buffer
						indexBufferData.resize(totalNumberOfIndices);
						for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
						{
							// Adjust sub-meshes of the first LOD
							RERenderer::v1Mesh::SubMesh& subMesh = subMeshes[subMeshIndex];
							subMesh.startIndexLocation = lodIndexOffsets[0][subMeshIndex];
							subMesh.numberOfIndices	   = lodNumberOfIndices[0][subMeshIndex];

							// Concatenate all LODs into one index buffer
							for (uint32_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
							{
								memcpy(&indexBufferData[lodIndexOffsets[lodIndex][subMeshIndex]], lodIndices[lodIndex][subMeshIndex].data(), lodIndices[lodIndex][subMeshIndex].size() * sizeof(uint32_t));
							}

							// Add sub-meshes for the LODs, we don't modify our sub-mesh array while iterating over it
							for (uint32_t lodIndex = 1; lodIndex < numberOfLods; ++lodIndex)
							{
								RERenderer::v1Mesh::SubMesh subMeshLod;
								subMeshLod.materialAssetId	   = subMesh.materialAssetId;	// TODO(naetherm) Add support for changing the material asset ID of LOD sub-meshes for material LOD
								subMeshLod.startIndexLocation  = lodIndexOffsets[lodIndex][subMeshIndex];
								subMeshLod.numberOfIndices	   = lodNumberOfIndices[lodIndex][subMeshIndex];
								subMeshLods[lodIndex].push_back(subMeshLod);
							}
						}
						numberOfIndices = static_cast<uint32_t>(indexBufferData.size());
					}

					// Step three: Add sub-meshes for the LODs, order is important here: <LOD 0, sub mesh 0>, <LOD 0, sub mesh n>, <LOD n, sub mesh 0>, <LOD n, sub mesh n>
					for (uint32_t lodIndex = 1; lodIndex < numberOfLods; ++lodIndex)
					{
						for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
						{
							subMeshes.push_back(subMeshLods[lodIndex][subMeshIndex]);
						}
					}

					// Step four: Vertex fetch optimization should go last as it depends on the final index order, note that the order of LODs above affects vertex fetch results
					meshopt_optimizeVertexFetch(vertexBufferData, indexBufferData.data(), numberOfIndices, vertexBufferData, numberOfVertices, numberOfBytesPerVertex);

					// Step five: Optional position-only index buffer (can reduce the number of processed vertices up to half)
					// -> This index buffer can be used for position-only rendering (e.g. shadow map rendering) using the same vertex data that the original index buffer uses
					// -> In case a mesh is only used together with materials using an alpha map for semi-transparent rendering, one can disable the optional position-only index buffer to save some memory (automatic detection not possible since the material might be changed during runtime)
					{
						bool hasPositionOnlyIndexBuffer = true;
						JsonHelper::optionalBooleanProperty(rapidJsonValueMeshAssetCompiler, "HasPositionOnlyIndexBuffer", hasPositionOnlyIndexBuffer);
						if (hasPositionOnlyIndexBuffer)
						{
							positionOnlyIndexBufferData.resize(numberOfIndices);
							if (numberOfBones > 0)
							{
								// For "meshopt_generateShadowIndexBuffer()" to work correctly, we also need to take the bone indices and bone weights into account. Those attributes don't directly
								// follow the vertex position and this vertex layout has several benefits at other places, so we don't change the vertex layout so "meshopt_generateShadowIndexBuffer()"
								// can work in an efficient direct way but create a temporary vertex buffer instead.

								// Number of bytes per position-only skinned vertex (3 float position, 4 byte bone indices, 4 byte bone weights) = 20 bytes
								static const uint32_t NUMBER_OF_BYTES_PER_VERTEX = 20;
								uint8_t* temporaryVertexBufferData = new uint8_t[NUMBER_OF_BYTES_PER_VERTEX * numberOfVertices];
								{
									uint8_t* RESTRICT currentTemporaryVertexBufferData = temporaryVertexBufferData;
									const uint8_t* RESTRICT currentVertexBufferData = vertexBufferData;
									for (uint32_t vertexIndex = 0; vertexIndex < numberOfVertices; ++vertexIndex)
									{
										{ // Copy over 3 float position
											float* RESTRICT currentTemporaryVertexBufferDataFloat = reinterpret_cast<float*>(currentTemporaryVertexBufferData);
											const float* currentVertexBufferDataFloat = reinterpret_cast<const float*>(currentVertexBufferData);
											currentTemporaryVertexBufferDataFloat[0] = currentVertexBufferDataFloat[0];
											currentTemporaryVertexBufferDataFloat[1] = currentVertexBufferDataFloat[1];
											currentTemporaryVertexBufferDataFloat[2] = currentVertexBufferDataFloat[2];
										}

										// Skip 32 bit position, 32 bit texture coordinate and 16 bit QTangent
										currentTemporaryVertexBufferData += sizeof(float) * 3;
										currentVertexBufferData += sizeof(float) * 3 + sizeof(float) * 2 + sizeof(short) * 4;

										// Copy over 4 byte bone indices and 4 byte bone weights
										for (uint32_t j = 0; j < 8; ++j)
										{
											currentTemporaryVertexBufferData[j] = currentVertexBufferData[j];
										}
										currentTemporaryVertexBufferData += sizeof(uint8_t) * 8;
										currentVertexBufferData += sizeof(uint8_t) * 8;
									}
								}
								for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
								{
									for (uint32_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
									{
										const uint32_t lodIndexOffset = lodIndexOffsets[lodIndex][subMeshIndex];
										meshopt_generateShadowIndexBuffer(&positionOnlyIndexBufferData[lodIndexOffset], &indexBufferData[lodIndexOffset], lodNumberOfIndices[lodIndex][subMeshIndex], temporaryVertexBufferData, numberOfVertices, NUMBER_OF_BYTES_PER_VERTEX, NUMBER_OF_BYTES_PER_VERTEX);
									}
								}
								delete [] temporaryVertexBufferData;
							}
							else
							{
								// Number of bytes per position-only vertex (3 float position) = 12 bytes
								for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
								{
									for (uint32_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
									{
										const uint32_t lodIndexOffset = lodIndexOffsets[lodIndex][subMeshIndex];
										meshopt_generateShadowIndexBuffer(&positionOnlyIndexBufferData[lodIndexOffset], &indexBufferData[lodIndexOffset], lodNumberOfIndices[lodIndex][subMeshIndex], vertexBufferData, numberOfVertices, 12, numberOfBytesPerVertex);
									}
								}
							}

							// While you can't optimize the vertex data after shadow index buffer was constructed, you can and should optimize the shadow index buffer for vertex cache this is valuable even if the original indices array was optimized for vertex cache
							for (uint32_t subMeshIndex = 0; subMeshIndex < numberOfSubMeshes; ++subMeshIndex)
							{
								for (uint32_t lodIndex = 0; lodIndex < numberOfLods; ++lodIndex)
								{
									const uint32_t lodIndexOffset = lodIndexOffsets[lodIndex][subMeshIndex];
									meshopt_optimizeVertexCache(&positionOnlyIndexBufferData[lodIndexOffset], &positionOnlyIndexBufferData[lodIndexOffset], lodNumberOfIndices[lodIndex][subMeshIndex], numberOfVertices);
								}
							}
						}

						/*
						{ // Don't remove this comment: For testing or verbose logging how many vertices the position-only index buffer saves one can do the following:
							std::unordered_set<uint32_t> vertexIndices;
							for (uint32_t vertexIndex : indexBufferData)
							{
								vertexIndices.insert(vertexIndex);
							}
							const size_t numberOfOriginalIndexedVertices = vertexIndices.size();
							vertexIndices.clear();
							for (uint32_t vertexIndex : positionOnlyIndexBufferData)
							{
								vertexIndices.insert(vertexIndex);
							}
							const size_t numberOfPositionOnlyIndexedVertices = vertexIndices.size();
							NOP;
						}
						*/
					}

					// "meshoptimizer" vertex/index buffer compression isn't used by intent. As of 22 December 2019 there's no 16-bit index support when using index compression and
					// a quick'n'dirty test showed that when using vertex compression the resulting file got actually bigger instead of smaller (we're using LZ4 compressed files).
				}

				{ // Write down the mesh header
					RERenderer::v1Mesh::MeshHeader meshHeader;

					// Bounding
					// -> Calculate the bounding sphere radius enclosing the bounding box (don't use the inner bounding box radius)
					meshHeader.minimumBoundingBoxPosition = minimumBoundingBoxPosition;
					meshHeader.maximumBoundingBoxPosition = maximumBoundingBoxPosition;
					meshHeader.boundingSpherePosition	  = (minimumBoundingBoxPosition + maximumBoundingBoxPosition) * 0.5f;
					meshHeader.boundingSphereRadius		  = RECore::Math::calculateInnerBoundingSphereRadius(minimumBoundingBoxPosition, maximumBoundingBoxPosition);

					// Vertex and index data
					meshHeader.numberOfBytesPerVertex	= numberOfBytesPerVertex;
					meshHeader.numberOfVertices			= numberOfVertices;
					meshHeader.indexBufferFormat		= static_cast<uint8_t>(indexBufferFormat);
					meshHeader.numberOfIndices			= numberOfIndices;
					meshHeader.numberOfVertexAttributes = static_cast<uint8_t>(vertexAttributes.numberOfAttributes);
					meshHeader.hasPositionOnlyIndices	= !positionOnlyIndexBufferData.empty();

					// Sub-meshes and LODs
					meshHeader.numberOfSubMeshes = static_cast<uint16_t>(subMeshes.size());
					meshHeader.numberOfLods = static_cast<uint8_t>(numberOfLods);

					// Optional skeleton
					meshHeader.numberOfBones = skeleton.numberOfBones;

					// Write down
					memoryFile.write(&meshHeader, sizeof(RERenderer::v1Mesh::MeshHeader));
				}

				// Write down the vertex and index buffer (directly containing also the index data of all LODs)
				memoryFile.write(vertexBufferData, numberOfBytesPerVertex * numberOfVertices);
				if (numberOfIndices > 0)
				{
					::detail::writeIndexBufferData(indexBufferFormat, indexBufferData, temporaryShortIndexBufferData, memoryFile);
				}

				// Write down the optional position-only index buffer (directly containing also the index data of all LODs)
				if (!positionOnlyIndexBufferData.empty())
				{
					::detail::writeIndexBufferData(indexBufferFormat, positionOnlyIndexBufferData, temporaryShortIndexBufferData, memoryFile);
				}

				// Destroy local vertex and input buffer data
				delete [] vertexBufferData;
				indexBufferData.clear();

				// Write down the vertex array attributes
				memoryFile.write(vertexAttributes.attributes, sizeof(RERHI::VertexAttribute) * vertexAttributes.numberOfAttributes);

				// Write down the sub-meshes
				memoryFile.write(subMeshes.data(), sizeof(RERenderer::v1Mesh::SubMesh) * subMeshes.size());

				// Write down the optional skeleton
				if (skeleton.numberOfBones > 0)
				{
					const aiMatrix4x4& assimpRootTransformation = assimpScene->mRootNode->mTransformation.Inverse();
					for (uint8_t boneIndex = 0; boneIndex < skeleton.numberOfBones; ++boneIndex)
					{
						// Some Assimp importers like the MD5 one compensate coordinate system differences by setting a root node transform, so we need to take this into account
						skeleton.boneOffsetMatrices[boneIndex] = skeleton.boneOffsetMatrices[boneIndex] * assimpRootTransformation;

						// Take care of row/column major flipping
						skeleton.localBoneMatrices[boneIndex].Transpose();
						skeleton.boneOffsetMatrices[boneIndex].Transpose();
					}
					memoryFile.write(skeleton.getSkeletonData(), skeleton.getNumberOfSkeletonDataBytes());
				}
			}
			else
			{
				throw std::runtime_error("Assimp failed to load in the given mesh \"" + virtualInputFilename + "\": " + assimpLogStream.getLastErrorMessage());
			}

			// Write LZ4 compressed output
			if (!memoryFile.writeLz4CompressedDataByVirtualFilename(RERenderer::v1Mesh::FORMAT_TYPE, RERenderer::v1Mesh::FORMAT_VERSION, input.context.getFileManager(), virtualOutputAssetFilename.c_str()))
			{
				throw std::runtime_error("Failed to write LZ4 compressed output file \"" + virtualOutputAssetFilename + '\"');
			}

			// Store new cache entries or update existing ones
			input.cacheManager.storeOrUpdateCacheEntries(cacheEntries);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
