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
namespace RERenderer
{
	class RenderableManager;
	class MaterialResourceManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MaterialResourceId;	///< POD material resource identifier
	typedef RECore::uint32 SkeletonResourceId;	///< POD skeleton resource identifier
	typedef RECore::uint32 MaterialTechniqueId;	///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Renderable
	*
	*  @note
	*    - Example: Abstract representation of a sub-mesh which is part of an mesh scene item
	*/
	class Renderable final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RenderQueue;		// Must be able to update cached material data
		friend class MaterialResource;	// Must be able to update cached material data


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		Renderable();
		Renderable(RenderableManager& renderableManager, const RERHI::RHIVertexArrayPtr& vertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, RECore::uint32 startIndexLocation, RECore::uint32 numberOfIndices, RECore::uint32 instanceCount = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER);
		Renderable(RenderableManager& renderableManager, const RERHI::RHIVertexArrayPtr& vertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, const RERHI::RHIIndirectBufferPtr& indirectBufferPtr, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER);
		Renderable(RenderableManager& renderableManager, const RERHI::RHIVertexArrayPtr& vertexArrayPtr, const RERHI::RHIVertexArrayPtr& positionOnlyVertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, RECore::uint32 startIndexLocation, RECore::uint32 numberOfIndices, RECore::uint32 instanceCount = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER);
		Renderable(RenderableManager& renderableManager, const RERHI::RHIVertexArrayPtr& vertexArrayPtr, const RERHI::RHIVertexArrayPtr& positionOnlyVertexArrayPtr, const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId, SkeletonResourceId skeletonResourceId, bool drawIndexed, const RERHI::RHIIndirectBufferPtr& indirectBufferPtr, RECore::uint32 indirectBufferOffset = 0, RECore::uint32 numberOfDraws = 1 RHI_RESOURCE_DEBUG_NAME_PARAMETER);

		inline ~Renderable()
		{
			unsetMaterialResourceIdInternal();
		}

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		#ifdef DEBUG
			/**
			*  @brief
			*    Return the renderable debug name
			*
			*  @return
			*    The renderable debug name, never a null pointer and at least an empty string
			*
			*  @note
			*    - If possible, the renderable debug name should use the following convention: "<filename>?[<attribute 0>][<attribute n>]" (for "?" see "RECore::IFileManager::INVALID_CHARACTER")
			*/
			[[nodiscard]] inline const char* getDebugName() const
			{
				return mDebugName;
			}

			inline void setDebugName(const char debugName[])
			{
				ASSERT(strlen(debugName) < 256, "Renderable debug name is not allowed to exceed 255 characters")
				strncpy(mDebugName, debugName, 256);
				mDebugName[255] = '\0';
			}
		#endif

		//[-------------------------------------------------------]
		//[ Data                                                  ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline RenderableManager& getRenderableManager() const
		{
			return mRenderableManager;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getVertexArrayPtr() const
		{
			return mVertexArrayPtr;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getPositionOnlyVertexArrayPtr() const
		{
			return mPositionOnlyVertexArrayPtr;
		}

		[[nodiscard]] inline const RERHI::RHIVertexArrayPtr& getPositionOnlyVertexArrayPtrWithFallback() const
		{
			// In case an alpha map is used, usage of the position-only vertex array object (VAO) can result in visual artifacts since also a texture coordinate is needed for sampling
			return (!mUseAlphaMap && mPositionOnlyVertexArrayPtr != nullptr) ? mPositionOnlyVertexArrayPtr : mVertexArrayPtr;
		}

		inline void setVertexArrayPtr(const RERHI::RHIVertexArrayPtr& vertexArrayPtr, const RERHI::RHIVertexArrayPtr& positionOnlyVertexArrayPtr = RERHI::RHIVertexArrayPtr())
		{
			mVertexArrayPtr = vertexArrayPtr;
			mPositionOnlyVertexArrayPtr = positionOnlyVertexArrayPtr;
		}

		[[nodiscard]] inline const RERHI::RHIIndirectBufferPtr& getIndirectBufferPtr() const
		{
			return mIndirectBufferPtr;
		}

		inline void setIndirectBufferPtr(const RERHI::RHIIndirectBufferPtr& indirectBufferPtr)
		{
			mIndirectBufferPtr = indirectBufferPtr;
		}

		[[nodiscard]] inline bool getDrawIndexed() const
		{
			return mDrawIndexed;
		}

		inline void setDrawIndexed(bool drawIndexed)
		{
			mDrawIndexed = drawIndexed;
		}

		[[nodiscard]] inline RECore::uint32 getStartIndexLocation() const
		{
			return mStartIndexLocation;
		}

		inline void setStartIndexLocation(RECore::uint32 startIndexLocation)
		{
			mStartIndexLocation = startIndexLocation;
		}

		[[nodiscard]] inline RECore::uint32 getIndirectBufferOffset() const
		{
			return mIndirectBufferOffset;
		}

		inline void setIndirectBufferOffset(RECore::uint32 indirectBufferOffset)
		{
			mIndirectBufferOffset = indirectBufferOffset;
		}

		[[nodiscard]] inline RECore::uint32 getNumberOfIndices() const
		{
			return mNumberOfIndices;
		}

		inline void setNumberOfIndices(RECore::uint32 numberOfIndices)
		{
			mNumberOfIndices = numberOfIndices;
		}

		[[nodiscard]] inline RECore::uint32 getInstanceCount() const
		{
			return mInstanceCount;
		}

		inline void setInstanceCount(RECore::uint32 instanceCount)
		{
			mInstanceCount = instanceCount;
		}

		[[nodiscard]] inline RECore::uint32 getNumberOfDraws() const
		{
			return mNumberOfDraws;
		}

		inline void setNumberOfDraws(RECore::uint32 numberOfDraws)
		{
			mNumberOfDraws = numberOfDraws;
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}

		void setMaterialResourceId(const MaterialResourceManager& materialResourceManager, MaterialResourceId materialResourceId);

		inline void unsetMaterialResourceId()
		{
			unsetMaterialResourceIdInternal();
		}

		[[nodiscard]] inline SkeletonResourceId getSkeletonResourceId() const
		{
			return mSkeletonResourceId;
		}

		inline void setSkeletonResourceId(SkeletonResourceId skeletonResourceId)
		{
			mSkeletonResourceId = skeletonResourceId;
		}

		//[-------------------------------------------------------]
		//[ Cached material data                                  ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline RECore::uint8 getRenderQueueIndex() const
		{
			return mRenderQueueIndex;
		}

		[[nodiscard]] inline bool getCastShadows() const
		{
			return mCastShadows;
		}

		[[nodiscard]] inline bool getUseAlphaMap() const
		{
			return mUseAlphaMap;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Renderable& operator=(const Renderable&) = delete;
		void unsetMaterialResourceIdInternal();


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct PipelineStateCache
		{
			MaterialTechniqueId	   materialTechniqueId;
			RECore::uint32			   generationCounter;	// Most simple solution to detect e.g. shader combination changes which make the pipeline state cache invalid
			RERHI::RHIPipelineStatePtr pipelineStatePtr;
			inline PipelineStateCache(MaterialTechniqueId _materialTechniqueId, RECore::uint32 _generationCounter, const RERHI::RHIGraphicsPipelineStatePtr& graphicsPipelineStatePtr) :
				materialTechniqueId(_materialTechniqueId),
				generationCounter(_generationCounter),
				pipelineStatePtr(graphicsPipelineStatePtr)
			{};
			inline PipelineStateCache(MaterialTechniqueId _materialTechniqueId, RECore::uint32 _generationCounter, const RERHI::RHIComputePipelineStatePtr& computePipelineStatePtr) :
				materialTechniqueId(_materialTechniqueId),
				generationCounter(_generationCounter),
				pipelineStatePtr(computePipelineStatePtr)
			{};
		};
		typedef std::vector<PipelineStateCache> PipelineStateCaches;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Debug
		#ifdef DEBUG
			char						mDebugName[256];				///< Debug name for easier renderable identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
		#endif
		// Data
		RenderableManager&				mRenderableManager;
		RERHI::RHIVertexArrayPtr			mVertexArrayPtr;				///< Optional vertex array object (VAO), can be a null pointer
		RERHI::RHIVertexArrayPtr			mPositionOnlyVertexArrayPtr;	///< Optional position-only vertex array object (VAO) which can reduce the number of processed vertices up to half, can be a null pointer, can be used for position-only rendering (e.g. shadow map rendering) using the same vertex data that the original vertex array object (VAO) uses
		RERHI::RHIIndirectBufferPtr			mIndirectBufferPtr;				///< Optional indirect buffer, can be a null pointer
		union
		{
			RECore::uint32					mStartIndexLocation;			///< Used in case there's no indirect buffer
			RECore::uint32					mIndirectBufferOffset;			///< Used in case there's an indirect buffer
		};
		RECore::uint32						mNumberOfIndices;				///< If there's an indirect buffer set, this value is unused
		union
		{
			RECore::uint32					mInstanceCount;					///< Used in case there's no indirect buffer
			RECore::uint32					mNumberOfDraws;					///< Used in case there's an indirect buffer
		};
		MaterialResourceId				mMaterialResourceId;
		SkeletonResourceId				mSkeletonResourceId;
		bool							mDrawIndexed;					///< Placed at this location due to padding
		// Cached material data
		RECore::uint8							mRenderQueueIndex;
		bool							mCastShadows;
		bool							mUseAlphaMap;					///< Relevant e.g. in combination with cast shadows in which the optional position-only vertex array object (VAO) can't be used in case an alpha mask is used
		PipelineStateCaches				mPipelineStateCaches;
		// Internal data
		const MaterialResourceManager*	mMaterialResourceManager;
		int								mMaterialResourceAttachmentIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
