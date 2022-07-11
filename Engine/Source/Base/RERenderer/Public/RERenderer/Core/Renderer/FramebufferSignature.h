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
#include <RECore/String/StringId.h>
#include <RECore/Utility/GetInvalid.h>

#include <cstring>	// For "memcpy()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;					///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::uint32 FramebufferSignatureId;	///< Framebuffer signature identifier, result of hashing framebuffer properties


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	struct FramebufferSignatureAttachment final
	{
		AssetId  textureAssetId;
		RECore::uint32 mipmapIndex;
		RECore::uint32 layerIndex;	///< "slice" in Direct3D terminology, depending on the texture type it's a 2D texture array layer, 3D texture slice or cube map face
		inline FramebufferSignatureAttachment() :
			textureAssetId(RECore::getInvalid<AssetId>()),
			mipmapIndex(0),
			layerIndex(0)
		{
			// Nothing here
		}
		inline FramebufferSignatureAttachment(const FramebufferSignatureAttachment& framebufferSignatureAttachment) :
			textureAssetId(framebufferSignatureAttachment.textureAssetId),
			mipmapIndex(framebufferSignatureAttachment.mipmapIndex),
			layerIndex(framebufferSignatureAttachment.layerIndex)
		{
			// Nothing here
		}
		inline FramebufferSignatureAttachment(AssetId _textureAssetId, RECore::uint32 _mipmapIndex = 0, RECore::uint32 _layerIndex = 0) :
			textureAssetId(_textureAssetId),
			mipmapIndex(_mipmapIndex),
			layerIndex(_layerIndex)
		{
			// Nothing here
		}
	};

	/**
	*  @brief
	*    Framebuffer signature
	*/
	class FramebufferSignature final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline FramebufferSignature() :
			mNumberOfColorAttachments(0),
			mFramebufferSignatureId(RECore::getInvalid<FramebufferSignatureId>())
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] numberOfColorAttachments
		*    Number of color attachments [0, 8]
		*  @param[in] colorFramebufferSignatureAttachments
		*    Color framebuffer signature attachment
		*  @param[in] depthStencilFramebufferSignatureAttachment
		*    Depth stencil framebuffer signature attachment
		*/
		FramebufferSignature(RECore::uint8 numberOfColorAttachments, const FramebufferSignatureAttachment colorFramebufferSignatureAttachments[8], const FramebufferSignatureAttachment& depthStencilFramebufferSignatureAttachment);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] framebufferSignature
		*    Framebuffer signature to copy from
		*/
		inline explicit FramebufferSignature(const FramebufferSignature& framebufferSignature) :
			mNumberOfColorAttachments(framebufferSignature.mNumberOfColorAttachments),
			mColorFramebufferSignatureAttachments{framebufferSignature.mColorFramebufferSignatureAttachments[0], framebufferSignature.mColorFramebufferSignatureAttachments[1], framebufferSignature.mColorFramebufferSignatureAttachments[2], framebufferSignature.mColorFramebufferSignatureAttachments[3], framebufferSignature.mColorFramebufferSignatureAttachments[4], framebufferSignature.mColorFramebufferSignatureAttachments[5], framebufferSignature.mColorFramebufferSignatureAttachments[6], framebufferSignature.mColorFramebufferSignatureAttachments[7]},
			mDepthStencilFramebufferSignatureAttachment(framebufferSignature.mDepthStencilFramebufferSignatureAttachment),
			mFramebufferSignatureId(framebufferSignature.mFramebufferSignatureId)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline ~FramebufferSignature()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy operator
		*/
		inline FramebufferSignature& operator=(const FramebufferSignature& framebufferSignature)
		{
			mNumberOfColorAttachments = framebufferSignature.mNumberOfColorAttachments;
			memcpy(mColorFramebufferSignatureAttachments, framebufferSignature.mColorFramebufferSignatureAttachments, sizeof(FramebufferSignatureAttachment) * 8);
			mDepthStencilFramebufferSignatureAttachment = framebufferSignature.mDepthStencilFramebufferSignatureAttachment;
			mFramebufferSignatureId = framebufferSignature.mFramebufferSignatureId;

			// Done
			return *this;
		}

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline RECore::uint8 getNumberOfColorAttachments() const
		{
			return mNumberOfColorAttachments;
		}

		[[nodiscard]] inline const FramebufferSignatureAttachment& getColorFramebufferSignatureAttachment(RECore::uint8 index) const
		{
			ASSERT(index < mNumberOfColorAttachments, "Invalid index")
			return mColorFramebufferSignatureAttachments[index];
		}

		[[nodiscard]] inline const FramebufferSignatureAttachment& getDepthStencilFramebufferSignatureAttachment() const
		{
			return mDepthStencilFramebufferSignatureAttachment;
		}

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline FramebufferSignatureId getFramebufferSignatureId() const
		{
			return mFramebufferSignatureId;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		RECore::uint8						   mNumberOfColorAttachments;
		FramebufferSignatureAttachment mColorFramebufferSignatureAttachments[8];
		FramebufferSignatureAttachment mDepthStencilFramebufferSignatureAttachment;
		// Derived data
		FramebufferSignatureId mFramebufferSignatureId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
