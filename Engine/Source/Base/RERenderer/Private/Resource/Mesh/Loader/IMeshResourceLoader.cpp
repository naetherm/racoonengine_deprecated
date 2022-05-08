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
#include "RERenderer/Resource/Mesh/Loader/IMeshResourceLoader.h"
#include "RERenderer/Resource/Mesh/MeshResource.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void IMeshResourceLoader::initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mMeshResource = static_cast<MeshResource*>(&resource);
	}

	bool IMeshResourceLoader::isFullyLoaded()
	{
		// Fully loaded?
		const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
		const SubMeshes& subMeshes = mMeshResource->getSubMeshes();
		const uint32_t numberOfUsedSubMeshes = static_cast<uint32_t>(subMeshes.size());
		for (uint32_t i = 0; i < numberOfUsedSubMeshes; ++i)
		{
			if (RECore::IResource::LoadingState::LOADED != materialResourceManager.getResourceByResourceId(subMeshes[i].getMaterialResourceId()).getLoadingState())
			{
				// Not fully loaded
				return false;
			}
		}

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
