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
	class CompositorWorkspaceResourceLoader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 CompositorWorkspaceResourceId;	///< POD compositor workspace resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorWorkspaceResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorWorkspaceResourceLoader;
		friend RECore::PackedElementManager<CompositorWorkspaceResource, CompositorWorkspaceResourceId, 32>;										// Type definition of template class
		friend RECore::ResourceManagerTemplate<CompositorWorkspaceResource, CompositorWorkspaceResourceLoader, CompositorWorkspaceResourceId, 32>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<RECore::AssetId> CompositorNodeAssetIds;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline void reserveCompositorNodes(RECore::uint32 numberOfCompositorNodes)
		{
			mCompositorNodeAssetIds.reserve(numberOfCompositorNodes);
		}

		inline void addCompositorNode(RECore::AssetId compositorNodeAssetId)
		{
			mCompositorNodeAssetIds.push_back(compositorNodeAssetId);
		}

		[[nodiscard]] inline const CompositorNodeAssetIds& getCompositorNodeAssetIds() const
		{
			return mCompositorNodeAssetIds;
		}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline CompositorWorkspaceResource()
		{
			// Nothing here
		}

		inline virtual ~CompositorWorkspaceResource() override
		{
			// Sanity check
			ASSERT(mCompositorNodeAssetIds.empty(), "Invalid compositor node asset IDs")
		}

		explicit CompositorWorkspaceResource(const CompositorWorkspaceResource&) = delete;
		CompositorWorkspaceResource& operator=(const CompositorWorkspaceResource&) = delete;

		//[-------------------------------------------------------]
		//[ "Renderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(CompositorWorkspaceResourceId compositorWorkspaceResourceId)
		{
			// Sanity check
			ASSERT(mCompositorNodeAssetIds.empty(), "Invalid compositor node asset IDs")

			// Call base implementation
			IResource::initializeElement(compositorWorkspaceResourceId);
		}

		inline void deinitializeElement()
		{
			mCompositorNodeAssetIds.clear();

			// Call base implementation
			IResource::deinitializeElement();
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CompositorNodeAssetIds mCompositorNodeAssetIds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
