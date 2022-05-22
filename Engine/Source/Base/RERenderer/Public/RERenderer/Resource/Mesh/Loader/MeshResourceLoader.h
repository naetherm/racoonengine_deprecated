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
#include "RERenderer/Resource/Mesh/Loader/IMeshResourceLoader.h"
#include <RECore/File/MemoryFile.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERHI
{
	class RHIVertexArray;
	class IBufferManager;
	struct VertexAttribute;
}
namespace RERenderer
{
	namespace v1Mesh
	{
		struct SubMesh;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MeshResourceId;	///< POD mesh resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MeshResourceLoader final : public IMeshResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MeshResourceManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("mesh");


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual RECore::ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] inline virtual bool hasDeserialization() const override
		{
			return true;
		}

		[[nodiscard]] virtual bool onDeserialization(RECore::IFile& file) override;

		[[nodiscard]] inline virtual bool hasProcessing() const override
		{
			return true;
		}

		virtual void onProcessing() override;
		[[nodiscard]] virtual bool onDispatch() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		MeshResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer);
		virtual ~MeshResourceLoader() override;
		explicit MeshResourceLoader(const MeshResourceLoader&) = delete;
		MeshResourceLoader& operator=(const MeshResourceLoader&) = delete;
		void createVertexArrays();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RERHI::RHIBufferManager& mBufferManager;	///< Buffer manager instance, do not destroy the instance

		// Temporary data
		RERHI::RHIVertexArray* mVertexArray;			///< In case the used RHI implementation supports native multithreading we also create the RHI resource asynchronous, but the final resource pointer reassignment must still happen synchronous
		RERHI::RHIVertexArray* mPositionOnlyVertexArray;
		RECore::MemoryFile		   mMemoryFile;

		// Temporary vertex buffer
		RECore::uint32 mNumberOfVertexBufferDataBytes;
		RECore::uint32 mNumberOfUsedVertexBufferDataBytes;
		RECore::uint8* mVertexBufferData;

		// Temporary index buffer
		RECore::uint8  mIndexBufferFormat;	// "RERHI::IndexBufferFormat", don't want to include the header in here
		RECore::uint32 mNumberOfIndexBufferDataBytes;
		RECore::uint32 mNumberOfUsedIndexBufferDataBytes;
		RECore::uint8* mIndexBufferData;

		// Temporary position-only index buffer
		RECore::uint32 mNumberOfPositionOnlyIndexBufferDataBytes;
		RECore::uint32 mNumberOfUsedPositionOnlyIndexBufferDataBytes;
		RECore::uint8* mPositionOnlyIndexBufferData;

		// Temporary vertex attributes
		RECore::uint32			  mNumberOfVertexAttributes;
		RECore::uint32			  mNumberOfUsedVertexAttributes;
		RERHI::VertexAttribute* mVertexAttributes;

		// Temporary sub-meshes
		RECore::uint32		 mNumberOfSubMeshes;
		RECore::uint32		 mNumberOfUsedSubMeshes;
		v1Mesh::SubMesh* mSubMeshes;

		// Optional temporary skeleton
		RECore::uint8  mNumberOfBones;
		RECore::uint8* mSkeletonData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
