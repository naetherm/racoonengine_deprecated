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
#include <RECore/Resource/IResource.h>
#include "RERenderer/Resource/Mesh/SubMesh.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <vector>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
template<class ELEMENT_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS>
class PackedElementManager;

template<class TYPE, class LOADER_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS>
class ResourceManagerTemplate;
}
namespace RERenderer {
class IMeshResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<SubMesh> SubMeshes;
	typedef RECore::uint32			 MeshResourceId;		///< POD mesh resource identifier
	typedef RECore::uint32			 SkeletonResourceId;	///< POD skeleton resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Mesh resource class
	*/
	class RERENDERER_API MeshResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend RECore::PackedElementManager<MeshResource, MeshResourceId, 4096>;							// Type definition of template class
		friend RECore::ResourceManagerTemplate<MeshResource, IMeshResourceLoader, MeshResourceId, 4096>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const RERHI::VertexAttributes VERTEX_ATTRIBUTES;			///< Default vertex attributes layout, whenever possible stick to this to be as compatible as possible to the rest
		static const RERHI::VertexAttributes SKINNED_VERTEX_ATTRIBUTES;	///< Default skinned vertex attributes layout, whenever possible stick to this to be as compatible as possible to the rest


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Bounding                                              ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const glm::vec3& getMinimumBoundingBoxPosition() const
		{
			return mMinimumBoundingBoxPosition;
		}

		[[nodiscard]] inline const glm::vec3& getMaximumBoundingBoxPosition() const
		{
			return mMaximumBoundingBoxPosition;
		}

		inline void setBoundingBoxPosition(const glm::vec3& minimumBoundingBoxPosition, const glm::vec3& maximumBoundingBoxPosition)
		{
			mMinimumBoundingBoxPosition = minimumBoundingBoxPosition;
			mMaximumBoundingBoxPosition = maximumBoundingBoxPosition;
		}

		[[nodiscard]] inline const glm::vec3& getBoundingSpherePosition() const
		{
			return mBoundingSpherePosition;
		}

		[[nodiscard]] inline float getBoundingSphereRadius() const
		{
			return mBoundingSphereRadius;
		}

		inline void setBoundingSpherePositionRadius(const glm::vec3& boundingSpherePosition, float boundingSphereRadius)
		{
			mBoundingSpherePosition = boundingSpherePosition;
			mBoundingSphereRadius = boundingSphereRadius;
		}

		//[-------------------------------------------------------]
		//[ Vertex and index data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline RECore::uint32 getNumberOfVertices() const
		{
			return mNumberOfVertices;
		}

		inline void setNumberOfVertices(RECore::uint32 numberOfVertices)
		{
			mNumberOfVertices = numberOfVertices;
		}

		[[nodiscard]] inline RECore::uint32 getNumberOfIndices() const
		{
			return mNumberOfIndices;
		}

		inline void setNumberOfIndices(RECore::uint32 numberOfIndices)
		{
			mNumberOfIndices = numberOfIndices;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getVertexArrayPtr() const
		{
			return mVertexArray;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getPositionOnlyVertexArrayPtr() const
		{
			return mPositionOnlyVertexArray;
		}

		inline void setVertexArray(RERHI::RHIVertexArray* vertexArray, RERHI::RHIVertexArray* positionOnlyVertexArray = nullptr)
		{
			mVertexArray = vertexArray;
			mPositionOnlyVertexArray = positionOnlyVertexArray;
		}

		//[-------------------------------------------------------]
		//[ Sub-meshes and LODs                                   ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline const SubMeshes& getSubMeshes() const
		{
			return mSubMeshes;
		}

		[[nodiscard]] inline SubMeshes& getSubMeshes()
		{
			return mSubMeshes;
		}

		[[nodiscard]] inline RECore::uint8 getNumberOfLods() const
		{
			return mNumberOfLods;
		}

		inline void setNumberOfLods(RECore::uint8 numberOfLods)
		{
			mNumberOfLods = numberOfLods;
		}

		//[-------------------------------------------------------]
		//[ Optional skeleton                                     ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline SkeletonResourceId getSkeletonResourceId() const
		{
			return mSkeletonResourceId;
		}

		inline void setSkeletonResourceId(SkeletonResourceId skeletonResourceId)
		{
			mSkeletonResourceId = skeletonResourceId;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline MeshResource() :
			// Bounding
			mMinimumBoundingBoxPosition(RECore::getInvalid<float>()),
			mMaximumBoundingBoxPosition(RECore::getInvalid<float>()),
			mBoundingSpherePosition(RECore::getInvalid<float>()),
			mBoundingSphereRadius(RECore::getInvalid<float>()),
			// Vertex and index data
			mNumberOfVertices(0),
			mNumberOfIndices(0),
			// Sub-meshes and LODs
			mNumberOfLods(0),
			// Optional skeleton
			mSkeletonResourceId(RECore::getInvalid<SkeletonResourceId>())
		{
			// Nothing here
		}

		inline virtual ~MeshResource() override
		{
			// Sanity checks
			ASSERT(RECore::isInvalid(mMinimumBoundingBoxPosition.x) && RECore::isInvalid(mMinimumBoundingBoxPosition.y) && RECore::isInvalid(mMinimumBoundingBoxPosition.z), "Invalid bounding box")
			ASSERT(RECore::isInvalid(mMaximumBoundingBoxPosition.x) && RECore::isInvalid(mMaximumBoundingBoxPosition.y) && RECore::isInvalid(mMaximumBoundingBoxPosition.z), "Invalid bounding box")
			ASSERT(RECore::isInvalid(mBoundingSpherePosition.x) && RECore::isInvalid(mBoundingSpherePosition.y) && RECore::isInvalid(mBoundingSpherePosition.z), "Invalid bounding sphere")
			ASSERT(RECore::isInvalid(mBoundingSphereRadius), "Invalid bounding sphere")
			ASSERT(0 == mNumberOfVertices, "Invalid number of vertices")
			ASSERT(0 == mNumberOfIndices, "Invalid number of indices")
			ASSERT(nullptr == mVertexArray.getPointer(), "Invalid vertex array")
			ASSERT(nullptr == mPositionOnlyVertexArray.getPointer(), "Invalid position only vertex array")
			ASSERT(mSubMeshes.empty(), "Invalid sub-meshes")
			ASSERT(RECore::isInvalid(mSkeletonResourceId), "Invalid skeleton resource ID")
		}

		explicit MeshResource(const MeshResource&) = delete;
		MeshResource& operator=(const MeshResource&) = delete;

		//[-------------------------------------------------------]
		//[ "RERenderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(MeshResourceId meshResourceId)
		{
			// Sanity checks
			ASSERT(RECore::isInvalid(mMinimumBoundingBoxPosition.x) && RECore::isInvalid(mMinimumBoundingBoxPosition.y) && RECore::isInvalid(mMinimumBoundingBoxPosition.z), "Invalid bounding box")
			ASSERT(RECore::isInvalid(mMaximumBoundingBoxPosition.x) && RECore::isInvalid(mMaximumBoundingBoxPosition.y) && RECore::isInvalid(mMaximumBoundingBoxPosition.z), "Invalid bounding box")
			ASSERT(RECore::isInvalid(mBoundingSpherePosition.x) && RECore::isInvalid(mBoundingSpherePosition.y) && RECore::isInvalid(mBoundingSpherePosition.z), "Invalid bounding sphere")
			ASSERT(RECore::isInvalid(mBoundingSphereRadius), "Invalid bounding sphere")
			ASSERT(0 == mNumberOfVertices, "Invalid number of vertices")
			ASSERT(0 == mNumberOfIndices, "Invalid number of indices")
			ASSERT(nullptr == mVertexArray.getPointer(), "Invalid vertex array")
			ASSERT(nullptr == mPositionOnlyVertexArray.getPointer(), "Invalid position only vertex array")
			ASSERT(mSubMeshes.empty(), "Invalid sub-meshes")
			ASSERT(RECore::isInvalid(mSkeletonResourceId), "Invalid skeleton resource ID")

			// Call base implementation
			IResource::initializeElement(meshResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			RECore::setInvalid(mMinimumBoundingBoxPosition.x);
			RECore::setInvalid(mMinimumBoundingBoxPosition.y);
			RECore::setInvalid(mMinimumBoundingBoxPosition.z);
			RECore::setInvalid(mMaximumBoundingBoxPosition.x);
			RECore::setInvalid(mMaximumBoundingBoxPosition.y);
			RECore::setInvalid(mMaximumBoundingBoxPosition.z);
			RECore::setInvalid(mBoundingSpherePosition.x);
			RECore::setInvalid(mBoundingSpherePosition.y);
			RECore::setInvalid(mBoundingSpherePosition.z);
			RECore::setInvalid(mBoundingSphereRadius);
			mNumberOfVertices = 0;
			mNumberOfIndices = 0;
			mVertexArray = nullptr;
			mPositionOnlyVertexArray = nullptr;
			mSubMeshes.clear();
			mNumberOfIndices = 0;
			RECore::setInvalid(mSkeletonResourceId);

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Bounding, the bounding sphere radius is enclosing the bounding box (don't use the inner bounding box radius)
		glm::vec3 mMinimumBoundingBoxPosition;
		glm::vec3 mMaximumBoundingBoxPosition;
		glm::vec3 mBoundingSpherePosition;
		float	  mBoundingSphereRadius;
		// Vertex and index data
		RECore::uint32			 mNumberOfVertices;			///< Number of vertices
		RECore::uint32			 mNumberOfIndices;			///< Number of indices
		RERHI::RHIVertexArrayPtr mVertexArray;				///< Vertex array object (VAO), can be a null pointer, directly containing also the index data of all LODs
		RERHI::RHIVertexArrayPtr mPositionOnlyVertexArray;	///< Optional position-only vertex array object (VAO) which can reduce the number of processed vertices up to half, can be a null pointer, can be used for position-only rendering (e.g. shadow map rendering) using the same vertex data that the original vertex array object (VAO) uses, directly containing also the index data of all LODs
		// Sub-meshes and LODs
		SubMeshes			 mSubMeshes;			///< Sub-meshes, directly containing also the sub-meshes of all LODs, each LOD has the same number of sub-meshes
		RECore::uint8				 mNumberOfLods;			///< Number of LODs, there's always at least one LOD, namely the original none reduced version
		// Optional skeleton
		SkeletonResourceId	 mSkeletonResourceId;	///< Resource ID of the used skeleton, can be invalid


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
