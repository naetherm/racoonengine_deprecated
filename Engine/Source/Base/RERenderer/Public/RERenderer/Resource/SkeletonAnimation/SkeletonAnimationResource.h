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
#include <RECore/Resource/IResource.h>


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
	class SkeletonAnimationResourceLoader;
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
	*    Rigid skeleton animation tracks resource
	*/
	class SkeletonAnimationResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SkeletonAnimationResourceLoader;
		friend RECore::PackedElementManager<SkeletonAnimationResource, SkeletonAnimationResourceId, 2048>;										// Type definition of template class
		friend RECore::ResourceManagerTemplate<SkeletonAnimationResource, SkeletonAnimationResourceLoader, SkeletonAnimationResourceId, 2048>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint8 getNumberOfChannels() const
		{
			return mNumberOfChannels;
		}

		[[nodiscard]] inline float getDurationInTicks() const
		{
			return mDurationInTicks;
		}

		[[nodiscard]] inline float getTicksPerSecond() const
		{
			return mTicksPerSecond;
		}

		[[nodiscard]] inline const std::vector<RECore::uint32>& getBoneIds() const
		{
			return mBoneIds;
		}

		[[nodiscard]] inline const std::vector<RECore::uint8>& getAclCompressedTracks() const
		{
			return mAclCompressedTracks;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline SkeletonAnimationResource() :
			mNumberOfChannels(0),
			mDurationInTicks(0.0f),
			mTicksPerSecond(0.0f)
		{
			// Nothing here
		}

		inline virtual ~SkeletonAnimationResource() override
		{
			// Sanity checks
			ASSERT(0 == mNumberOfChannels, "Invalid number of channels")
			ASSERT(0.0f == mDurationInTicks, "Invalid duration in ticks")
			ASSERT(0.0f == mTicksPerSecond, "Invalid ticks per second")
		}

		explicit SkeletonAnimationResource(const SkeletonAnimationResource&) = delete;
		SkeletonAnimationResource& operator=(const SkeletonAnimationResource&) = delete;

		inline void clearSkeletonAnimationData()
		{
			mNumberOfChannels = 0;
			mDurationInTicks  = 0.0f;
			mTicksPerSecond   = 0.0f;
		}

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(SkeletonAnimationResourceId skeletonAnimationResourceId)
		{
			// Sanity checks
			ASSERT(0 == mNumberOfChannels, "Invalid number of channels")
			ASSERT(0.0f == mDurationInTicks, "Invalid duration in ticks")
			ASSERT(0.0f == mTicksPerSecond, "Invalid ticks per second")

			// Call base implementation
			IResource::initializeElement(skeletonAnimationResourceId);
		}

		inline void deinitializeElement()
		{
			// Reset everything
			clearSkeletonAnimationData();

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint8				  mNumberOfChannels;	///< The number of bone animation channels; each channel affects a single bone
		float				  mDurationInTicks;		///< Duration of the animation in ticks
		float				  mTicksPerSecond;		///< Ticks per second; 0 if not specified in the imported file
		std::vector<RECore::uint32> mBoneIds;
		std::vector<RECore::uint8>  mAclCompressedTracks;	///< ACL ( https://github.com/nfrechette/acl ) compressed skeleton animation tracks


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
