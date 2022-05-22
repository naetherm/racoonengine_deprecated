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
#include "RERenderer/Vr/OpenVR/Loader/OpenVRMeshResourceLoader.h"
#include "RERenderer/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.h"
#include "RERenderer/Resource/Mesh/MeshResource.h"
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include "RERenderer/Vr/OpenVR/VrManagerOpenVR.h"
#include <RECore/Math/Math.h>
#include "RERenderer/RendererImpl.h"
#include "RERenderer/Context.h"
#include <RECore/Log/Log.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(5204)	// warning C5204: 'crnlib::task_pool::executable_task': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
	#include <openvr/openvr.h>
PRAGMA_WARNING_POP

#include <mikktspace.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
	PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::_Func_impl_no_alloc<Concurrency::details::_Task_impl<_ReturnType>::_CancelAndRunContinuations::<lambda_0456396a71e3abd88ede77bdd2823d8e>,_Ret>': copy constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::_Func_impl_no_alloc<Concurrency::details::_Task_impl<_ReturnType>::_CancelAndRunContinuations::<lambda_0456396a71e3abd88ede77bdd2823d8e>,_Ret>': assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Func_impl_no_alloc<Concurrency::details::_Task_impl<_ReturnType>::_CancelAndRunContinuations::<lambda_0456396a71e3abd88ede77bdd2823d8e>,_Ret>': move constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Func_impl_no_alloc<Concurrency::details::_Task_impl<_ReturnType>::_CancelAndRunContinuations::<lambda_0456396a71e3abd88ede77bdd2823d8e>,_Ret>': move assignment operator was implicitly defined as deleted
	#include <chrono>
	#include <thread>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{

	namespace MikkTSpace
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr RECore::uint32 NUMBER_OF_VERTICES_PER_FACE = 3;


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		SMikkTSpaceContext g_MikkTSpaceContext;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] int getNumFaces(const SMikkTSpaceContext* pContext)
		{
			const RERenderer::OpenVRMeshResourceLoader* openVRMeshResourceLoader = static_cast<const RERenderer::OpenVRMeshResourceLoader*>(pContext->m_pUserData);
			return static_cast<int>(openVRMeshResourceLoader->getVrRenderModel()->unTriangleCount);
		}

		[[nodiscard]] int getNumVerticesOfFace(const SMikkTSpaceContext*, const int)
		{
			return NUMBER_OF_VERTICES_PER_FACE;
		}

		void getPosition(const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
		{
			const RERenderer::OpenVRMeshResourceLoader* openVRMeshResourceLoader = static_cast<const RERenderer::OpenVRMeshResourceLoader*>(pContext->m_pUserData);
			const vr::RenderModel_t* vrRenderModel = openVRMeshResourceLoader->getVrRenderModel();
			const vr::HmdVector3_t& position = vrRenderModel->rVertexData[vrRenderModel->rIndexData[iFace * NUMBER_OF_VERTICES_PER_FACE + iVert]].vPosition;
			fvPosOut[0] = position.v[0];
			fvPosOut[1] = position.v[1];
			fvPosOut[2] = position.v[2];
		}

		void getNormal(const SMikkTSpaceContext* pContext, float fvNormOut[], const int iFace, const int iVert)
		{
			const RERenderer::OpenVRMeshResourceLoader* openVRMeshResourceLoader = static_cast<const RERenderer::OpenVRMeshResourceLoader*>(pContext->m_pUserData);
			const vr::RenderModel_t* vrRenderModel = openVRMeshResourceLoader->getVrRenderModel();
			const vr::HmdVector3_t& normal = vrRenderModel->rVertexData[vrRenderModel->rIndexData[iFace * NUMBER_OF_VERTICES_PER_FACE + iVert]].vNormal;
			fvNormOut[0] = normal.v[0];
			fvNormOut[1] = normal.v[1];
			fvNormOut[2] = normal.v[2];
		}

		void getTexCoord(const SMikkTSpaceContext* pContext, float fvTexcOut[], const int iFace, const int iVert)
		{
			const RERenderer::OpenVRMeshResourceLoader* openVRMeshResourceLoader = static_cast<const RERenderer::OpenVRMeshResourceLoader*>(pContext->m_pUserData);
			const vr::RenderModel_t* vrRenderModel = openVRMeshResourceLoader->getVrRenderModel();
			const float* texCoord = vrRenderModel->rVertexData[vrRenderModel->rIndexData[iFace * NUMBER_OF_VERTICES_PER_FACE + iVert]].rfTextureCoord;
			fvTexcOut[0] = texCoord[0];
			fvTexcOut[1] = texCoord[1];
		}

		void setTSpace(const SMikkTSpaceContext* pContext, const float fvTangent[], const float fvBiTangent[], const float, const float, const tbool, const int iFace, const int iVert)
		{
			RERenderer::OpenVRMeshResourceLoader* openVRMeshResourceLoader = static_cast<RERenderer::OpenVRMeshResourceLoader*>(pContext->m_pUserData);
			const size_t index = openVRMeshResourceLoader->getVrRenderModel()->rIndexData[iFace * NUMBER_OF_VERTICES_PER_FACE + iVert];

			{ // Tangent
				glm::vec3& tangent = openVRMeshResourceLoader->getTangentsData()[index];
				tangent.x = fvTangent[0];
				tangent.y = fvTangent[1];
				tangent.z = fvTangent[2];
			}

			{ // Binormal
				glm::vec3& binormal = openVRMeshResourceLoader->getBinormalsData()[index];
				binormal.x = fvBiTangent[0];
				binormal.y = fvBiTangent[1];
				binormal.z = fvBiTangent[2];
			}
		}


	} // MikkTSpace

	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr RECore::uint32 NUMBER_OF_BYTES_PER_VERTEX = sizeof(float) * 3 + sizeof(float) * 2 + sizeof(short) * 4;


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		[[nodiscard]] RECore::AssetId setupRenderModelAlbedoTexture(const RERenderer::IRenderer& renderer, const vr::RenderModel_t& vrRenderModel)
		{
			// Check whether or not we need to generate the mesh asset right now
			RECore::AssetId assetId = RERenderer::VrManagerOpenVR::albedoTextureIdToAssetId(vrRenderModel.diffuseTextureId);
			RERenderer::TextureResourceId textureResourceId = RECore::getInvalid<RERenderer::TextureResourceId>();
			const bool rgbHardwareGammaCorrection = true;	// TODO(naetherm) It must be possible to set the property name from the outside: Ask the material blueprint whether or not hardware gamma correction should be used
			renderer.getTextureResourceManager().loadTextureResourceByAssetId(assetId, ASSET_ID("RacoonEngine/Texture/DynamicByCode/IdentityAlbedoMap2D"), textureResourceId, nullptr, rgbHardwareGammaCorrection, false, RERenderer::OpenVRTextureResourceLoader::TYPE_ID);

			// Done
			return assetId;
		}

		[[nodiscard]] RERenderer::MaterialResourceId setupRenderModelMaterial(const RERenderer::IRenderer& renderer, RERenderer::MaterialResourceId vrDeviceMaterialResourceId, vr::TextureID_t vrTextureId, RECore::AssetId albedoTextureAssetId)
		{
			// Get the texture name and convert it into an material asset ID
			const std::string materialName = "OpenVR_" + std::to_string(vrTextureId);
			RECore::AssetId materialAssetId = RECore::StringId(materialName.c_str());

			// Check whether or not we need to generate the material asset right now
			RERenderer::MaterialResourceManager& materialResourceManager = renderer.getMaterialResourceManager();
			RERenderer::MaterialResourceId materialResourceId = materialResourceManager.getMaterialResourceIdByAssetId(materialAssetId);
			if (RECore::isInvalid(materialResourceId))
			{
				// We need to generate the material asset right now
				materialResourceId = materialResourceManager.createMaterialResourceByCloning(vrDeviceMaterialResourceId, materialAssetId);
				if (RECore::isValid(materialResourceId))
				{
					RERenderer::MaterialResource* materialResource = materialResourceManager.tryGetById(materialResourceId);
					if (nullptr != materialResource)
					{
						// TODO(naetherm) It must be possible to set the property name from the outside
						materialResource->setPropertyById(STRING_ID("_argb_nxa"), RERenderer::MaterialPropertyValue::fromTextureAssetId(albedoTextureAssetId));
					}
				}
			}

			// Done
			return materialResourceId;
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
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void OpenVRMeshResourceLoader::onProcessing()
	{
		// Load the render model
		const std::string& renderModelName = getRenderModelName();
		vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();
		vr::EVRRenderModelError vrRenderModelError = vr::VRRenderModelError_Loading;
		while (vrRenderModelError == vr::VRRenderModelError_Loading)
		{
			vrRenderModelError = vrRenderModels->LoadRenderModel_Async(renderModelName.c_str(), &mVrRenderModel);
			if (vrRenderModelError == vr::VRRenderModelError_Loading)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1ms);
			}
		}
		if (vr::VRRenderModelError_None != vrRenderModelError)
		{
      RE_LOG(Critical, std::string("The renderer was unable to load OpenVR render model ") + renderModelName + ": " + vrRenderModels->GetRenderModelErrorNameFromEnum(vrRenderModelError))
			return;
		}

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
		MikkTSpace::g_MikkTSpaceContext.m_pUserData  = this;

		{ // Get the vertex buffer and index buffer data
			mMinimumBoundingBoxPosition = glm::vec3(std::numeric_limits<float>::max());
			mMaximumBoundingBoxPosition = glm::vec3(std::numeric_limits<float>::lowest());

			// Tell the mesh resource about the number of vertices and indices
			const RECore::uint32 numberOfVertices = mVrRenderModel->unVertexCount;
			const RECore::uint32 numberOfIndices = mVrRenderModel->unTriangleCount * 3;
			mMeshResource->setNumberOfVertices(numberOfVertices);
			mMeshResource->setNumberOfIndices(numberOfIndices);

			{ // Fill the vertex buffer data
				const RECore::uint32 numberOfBytes = numberOfVertices * ::detail::NUMBER_OF_BYTES_PER_VERTEX;
				mVertexBufferData.resize(numberOfBytes);
				RECore::uint8* currentVertexBufferData = mVertexBufferData.data();
				const vr::RenderModel_Vertex_t* currentVrRenderModelVertex = mVrRenderModel->rVertexData;
				mTangentsData.resize(mVrRenderModel->unVertexCount);
				mBinormalsData.resize(mVrRenderModel->unVertexCount);
				if (genTangSpaceDefault(&MikkTSpace::g_MikkTSpaceContext) == 0)
				{
					// TODO(naetherm) Error handling
					RHI_ASSERT(false, "MikkTSpace for semi-standard tangent space generation failed")
				}
				for (RECore::uint32 i = 0; i < numberOfVertices; ++i, ++currentVrRenderModelVertex)
				{
					const float* vrRenderModelVertexPosition = currentVrRenderModelVertex->vPosition.v;

					{ // Update minimum and maximum bounding box position
						const glm::vec3 glmVertex(vrRenderModelVertexPosition[0], vrRenderModelVertexPosition[1], -vrRenderModelVertexPosition[2]);
						mMinimumBoundingBoxPosition = glm::min(mMinimumBoundingBoxPosition, glmVertex);
						mMaximumBoundingBoxPosition = glm::max(mMaximumBoundingBoxPosition, glmVertex);
					}

					{ // 32 bit position
						float* position = reinterpret_cast<float*>(currentVertexBufferData);
						position[0] = vrRenderModelVertexPosition[0];
						position[1] = vrRenderModelVertexPosition[1];
						position[2] = -vrRenderModelVertexPosition[2];
						currentVertexBufferData += sizeof(float) * 3;
					}

					{ // 32 bit texture coordinate
						float* textureCoordinate = reinterpret_cast<float*>(currentVertexBufferData);
						textureCoordinate[0] = currentVrRenderModelVertex->rfTextureCoord[0];
						textureCoordinate[1] = currentVrRenderModelVertex->rfTextureCoord[1];
						currentVertexBufferData += sizeof(float) * 2;
					}

					{ // 16 bit QTangent
						// Get the mesh vertex normal, tangent and binormal
						const glm::vec3 normal(currentVrRenderModelVertex->vNormal.v[0], currentVrRenderModelVertex->vNormal.v[1], currentVrRenderModelVertex->vNormal.v[2]);
						const glm::vec3& tangent = mTangentsData[i];
						const glm::vec3& binormal = mBinormalsData[i];

						// Generate tangent frame rotation matrix
						glm::mat3 tangentFrame(
							tangent.x,  tangent.y,  tangent.z,
							binormal.x, binormal.y, binormal.z,
							normal.x,   normal.y,   normal.z
						);

						// Calculate tangent frame quaternion
						const glm::quat tangentFrameQuaternion = RECore::Math::calculateTangentFrameQuaternion(tangentFrame);

						// Set our vertex buffer 16 bit QTangent
						short* qTangent = reinterpret_cast<short*>(currentVertexBufferData);
						qTangent[0] = static_cast<short>(tangentFrameQuaternion.x * SHRT_MAX);
						qTangent[1] = static_cast<short>(tangentFrameQuaternion.y * SHRT_MAX);
						qTangent[2] = static_cast<short>(tangentFrameQuaternion.z * SHRT_MAX);
						qTangent[3] = static_cast<short>(tangentFrameQuaternion.w * SHRT_MAX);
						currentVertexBufferData += sizeof(short) * 4;
					}
				}
			}

			{ // Fill the index buffer data
			  // -> We need to flip the vertex winding so we don't need to modify rasterizer states
				mIndexBufferData.resize(numberOfIndices);
				RECore::uint16* currentIndexBufferData = mIndexBufferData.data();
				for (RECore::uint32 i = 0; i < mVrRenderModel->unTriangleCount; ++i, currentIndexBufferData += 3)
				{
					const RECore::uint32 offset = i * 3;
					currentIndexBufferData[0] = mVrRenderModel->rIndexData[offset + 2];
					currentIndexBufferData[1] = mVrRenderModel->rIndexData[offset + 1];
					currentIndexBufferData[2] = mVrRenderModel->rIndexData[offset + 0];
				}
			}
		}

		// Can we create the RHI resource asynchronous as well?
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			mVertexArray = createVertexArray();
		}
	}

	bool OpenVRMeshResourceLoader::onDispatch()
	{
		// Bounding
		// -> Calculate the bounding sphere radius enclosing the bounding box (don't use the inner bounding box radius)
		mMeshResource->setBoundingBoxPosition(mMinimumBoundingBoxPosition, mMaximumBoundingBoxPosition);
		mMeshResource->setBoundingSpherePositionRadius((mMinimumBoundingBoxPosition + mMaximumBoundingBoxPosition) * 0.5f, RECore::Math::calculateInnerBoundingSphereRadius(mMinimumBoundingBoxPosition, mMaximumBoundingBoxPosition));

		// Create vertex array object (VAO)
		mMeshResource->setVertexArray(mRenderer.getRhi().getCapabilities().nativeMultithreading ? mVertexArray : createVertexArray());

		{ // Create sub-meshes
			// Load the render model texture and setup the material asset
			// -> We don't care if loading of the albedo texture fails in here, isn't that important and the show must go on
			const AssetId albedoTextureAssetId = ::detail::setupRenderModelAlbedoTexture(mRenderer, *mVrRenderModel);
			const MaterialResourceId materialResourceId = ::detail::setupRenderModelMaterial(mRenderer, static_cast<const VrManagerOpenVR&>(mRenderer.getVrManager()).getVrDeviceMaterialResourceId(), mVrRenderModel->diffuseTextureId, albedoTextureAssetId);

			// Tell the mesh resource about the sub-mesh
			mMeshResource->getSubMeshes().push_back(SubMesh(materialResourceId, 0, mMeshResource->getNumberOfIndices()));
		}

		// Free the render model
		if (nullptr != mVrRenderModel)
		{
			vr::VRRenderModels()->FreeRenderModel(mVrRenderModel);
			mVrRenderModel = nullptr;
		}

		// Fully loaded?
		return true;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	RERHI::RHIVertexArray* OpenVRMeshResourceLoader::createVertexArray() const
	{
		RERHI::RHIBufferManager& bufferManager = mRenderer.getBufferManager();

		// Create the vertex buffer
		RERHI::RHIVertexBufferPtr vertexBuffer(bufferManager.createVertexBuffer(static_cast<RECore::uint32>(mVertexBufferData.size()), mVertexBufferData.data(), 0, RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME(getRenderModelName().c_str())));

		// Create the index buffer
		RERHI::RHIIndexBufferPtr indexBuffer(bufferManager.createIndexBuffer(static_cast<RECore::uint32>(mIndexBufferData.size() * sizeof(RECore::uint16)), mIndexBufferData.data(), 0, RERHI::BufferUsage::STATIC_DRAW, RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME(getRenderModelName().c_str())));

		// Create vertex array object (VAO)
		const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer, mRenderer.getMeshResourceManager().getDrawIdVertexBufferPtr() };
		return bufferManager.createVertexArray(MeshResource::VERTEX_ATTRIBUTES, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, indexBuffer RHI_RESOURCE_DEBUG_NAME(getRenderModelName().c_str()));
	}

	const std::string& OpenVRMeshResourceLoader::getRenderModelName() const
	{
		// OpenVR render model names can get awful long due to absolute path information, so, we need to store them inside a separate list and tell the asset just about the render model name index
		const VrManagerOpenVR::RenderModelNames& renderModelNames = static_cast<const VrManagerOpenVR&>(mRenderer.getVrManager()).getRenderModelNames();
		const RECore::uint32 renderModelNameIndex = static_cast<RECore::uint32>(std::atoi(getAsset().virtualFilename));
		RHI_ASSERT(renderModelNameIndex < static_cast<RECore::uint32>(renderModelNames.size()), "Invalid model name index")
		return renderModelNames[renderModelNameIndex];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
