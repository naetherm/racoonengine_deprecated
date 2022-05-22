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
#include <RECore/Resource/IResource.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	#include <glm/fwd.hpp>
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
	class SkeletonResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 SkeletonResourceId;	///< POD skeleton resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Rigid skeleton resource
	*
	*  @note
	*    - Each skeleton must have at least one bone
	*    - Bone data is cache friendly depth-first rolled up, see "Molecular Musings" - "Adventures in data-oriented design � Part 2: Hierarchical data" - https://blog.molecular-matters.com/2013/02/22/adventures-in-data-oriented-design-part-2-hierarchical-data/
	*    - The complete skeleton data is sequential in memory
	*/
	class SkeletonResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MeshResourceLoader;
		friend class SkeletonResourceLoader;
		friend RECore::PackedElementManager<SkeletonResource, SkeletonResourceId, 2048>;							// Type definition of template class
		friend RECore::ResourceManagerTemplate<SkeletonResource, SkeletonResourceLoader, SkeletonResourceId, 2048>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// static constexpr RECore::uint32 NUMBER_OF_BONE_SPACE_DATA_BYTES = sizeof(glm::mat3x4);	// Linear blend skinning (LBS) using matrices; there's no runtime switch by intent since dual quaternion skinning (DQS) is the way to go, don't remove this reference comment
		static constexpr RECore::uint32 NUMBER_OF_BONE_SPACE_DATA_BYTES = 32;	// Dual quaternion skinning (DQS); "sizeof(glm::dualquat)" not used due to unavailable forward declaration inside "glm/fwd.hpp"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint8 getNumberOfBones() const
		{
			return mNumberOfBones;
		}

		[[nodiscard]] inline const RECore::uint8* getBoneParentIndices() const
		{
			return mBoneParentIndices;
		}

		[[nodiscard]] inline const RECore::uint32* getBoneIds() const
		{
			return mBoneIds;
		}

		[[nodiscard]] inline glm::mat4* getLocalBoneMatrices()
		{
			return mLocalBoneMatrices;
		}

		[[nodiscard]] inline const glm::mat4* getLocalBoneMatrices() const
		{
			return mLocalBoneMatrices;
		}

		[[nodiscard]] inline const glm::mat4* getBoneOffsetMatrices() const
		{
			return mBoneOffsetMatrices;
		}

		[[nodiscard]] inline const glm::mat4* getGlobalBoneMatrices() const
		{
			return mGlobalBoneMatrices;
		}

		[[nodiscard]] inline RECore::uint32 getTotalNumberOfBoneSpaceDataBytes() const
		{
			ASSERT(0 != mNumberOfBones, "Each skeleton must have at least one bone")
			return static_cast<RECore::uint32>(NUMBER_OF_BONE_SPACE_DATA_BYTES * mNumberOfBones);
		}

		[[nodiscard]] inline const RECore::uint8* getBoneSpaceData() const
		{
			return mBoneSpaceData;
		}

		[[nodiscard]] RECore::uint32 getBoneIndexByBoneId(RECore::uint32 boneId) const;	// Bone IDs = "RERenderer::StringId" on bone name, "RECore::getInvalid<RECore::uint32>()" if unknown bone ID
		void localToGlobalPose();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline SkeletonResource() :
			mNumberOfBones(0),
			mBoneParentIndices(nullptr),
			mBoneIds(nullptr),
			mLocalBoneMatrices(nullptr),
			mBoneOffsetMatrices(nullptr),
			mGlobalBoneMatrices(nullptr),
			mBoneSpaceData(nullptr)
		{
			// Nothing here
		}

		inline virtual ~SkeletonResource() override
		{
			// Sanity checks
			ASSERT(0 == mNumberOfBones, "Invalid number of bones")
			ASSERT(nullptr == mBoneParentIndices, "Invalid bone parent indices")
			ASSERT(nullptr == mBoneIds, "Invalid bone IDs")
			ASSERT(nullptr == mLocalBoneMatrices, "Invalid local bone matrices")
			ASSERT(nullptr == mBoneOffsetMatrices, "Invalid bone offset matrices")
			ASSERT(nullptr == mGlobalBoneMatrices, "Invalid global bone matrices")
			ASSERT(nullptr == mBoneSpaceData, "Invalid bone space data")
		}

		explicit SkeletonResource(const SkeletonResource&) = delete;
		SkeletonResource& operator=(const SkeletonResource&) = delete;

		inline void clearSkeletonData()
		{
			mNumberOfBones = 0;
			delete [] mBoneParentIndices;
			mBoneParentIndices = nullptr;
			// delete [] mBoneIds;				// The complete skeleton data is sequential in memory, so, deleting "mBoneParentIndices" is does it all
			mBoneIds = nullptr;
			// delete [] mLocalBoneMatrices;	// The complete skeleton data is sequential in memory, so, deleting "mBoneParentIndices" is does it all
			mLocalBoneMatrices = nullptr;
			// delete [] mBoneOffsetMatrices;	// The complete skeleton data is sequential in memory, so, deleting "mBoneParentIndices" is does it all
			mBoneOffsetMatrices = nullptr;
			// delete [] mGlobalBoneMatrices;	// The complete skeleton data is sequential in memory, so, deleting "mBoneParentIndices" is does it all
			mGlobalBoneMatrices = nullptr;
			// delete [] mBoneSpaceData;		// The complete skeleton data is sequential in memory, so, deleting "mBoneParentIndices" is does it all
			mBoneSpaceData = nullptr;
		}

		//[-------------------------------------------------------]
		//[ "RERenderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(SkeletonResourceId skeletonResourceId)
		{
			// Sanity checks
			ASSERT(0 == mNumberOfBones, "Invalid number of bones")
			ASSERT(nullptr == mBoneParentIndices, "Invalid bone parent indices")
			ASSERT(nullptr == mBoneIds, "Invalid bone IDs")
			ASSERT(nullptr == mLocalBoneMatrices, "Invalid local bone matrices")
			ASSERT(nullptr == mBoneOffsetMatrices, "Invalid bone offset matrices")
			ASSERT(nullptr == mGlobalBoneMatrices, "Invalid global bone matrices")
			ASSERT(nullptr == mBoneSpaceData, "Invalid bone space data")

			// Call base implementation
			IResource::initializeElement(skeletonResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			clearSkeletonData();

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint8	   mNumberOfBones;		///< Number of bones
		// Structure-of-arrays (SoA)
		RECore::uint8*   mBoneParentIndices;	///< Cache friendly depth-first rolled up bone parent indices, null pointer only in case of horrible error, free the memory if no longer required
		RECore::uint32*  mBoneIds;			///< Cache friendly depth-first rolled up bone IDs ("RERenderer::StringId" on bone name), null pointer only in case of horrible error, don't free the memory because it's owned by "mBoneParentIndices"
		glm::mat4* mLocalBoneMatrices;	///< Cache friendly depth-first rolled up local bone matrices, null pointer only in case of horrible error, don't free the memory because it's owned by "mBoneParentIndices"
		glm::mat4* mBoneOffsetMatrices;	///< Cache friendly depth-first rolled up bone offset matrices (object space to bone space), null pointer only in case of horrible error, don't free the memory because it's owned by "mBoneParentIndices"
		glm::mat4* mGlobalBoneMatrices;	///< Cache friendly depth-first rolled up global bone matrices, null pointer only in case of horrible error, don't free the memory because it's owned by "mBoneParentIndices"
		RECore::uint8*   mBoneSpaceData;		///< Cache friendly depth-first rolled up bone space data, null pointer only in case of horrible error, don't free the memory because it's owned by "mBoneParentIndices"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
