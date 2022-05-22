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
#include "RERenderer/Core/Platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	// Rigid skeleton animation tracks file format content:
	// - File format header
	// - Skeleton animation header
	// - Bone IDs
	// - ACL ( https://github.com/nfrechette/acl ) compressed skeleton animation tracks
	namespace v1SkeletonAnimation
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr RECore::uint32 FORMAT_TYPE	 = STRING_ID("SkeletonAnimation");
		static constexpr RECore::uint32 FORMAT_VERSION = 3;

		#pragma pack(push)
		#pragma pack(1)
			struct SkeletonAnimationHeader final
			{
				RECore::uint8  numberOfChannels;			///< The number of bone animation channels; each channel affects a single bone
				float	 durationInTicks;			///< Duration of the animation in ticks
				float	 ticksPerSecond;			///< Ticks per second; 0 if not specified in the imported file
				RECore::uint32 aclCompressedTracksSize;	///< ACL compressed tracks size in bytes
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1SkeletonAnimation
} // RECore
