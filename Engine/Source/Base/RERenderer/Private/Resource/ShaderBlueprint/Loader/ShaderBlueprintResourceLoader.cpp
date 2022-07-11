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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/Resource/ShaderBlueprint/Loader/ShaderBlueprintResourceLoader.h"
#include "RERenderer/Resource/ShaderBlueprint/Loader/ShaderBlueprintFileFormat.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include "RERenderer/Resource/ShaderPiece/ShaderPieceResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/IRenderer.h"

#include <unordered_set>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void ShaderBlueprintResourceLoader::initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mShaderBlueprintResource = static_cast<ShaderBlueprintResource*>(&resource);
	}

	bool ShaderBlueprintResourceLoader::onDeserialization(RECore::IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1ShaderBlueprint::FORMAT_TYPE, v1ShaderBlueprint::FORMAT_VERSION, file);
	}

	void ShaderBlueprintResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the shader blueprint header
		v1ShaderBlueprint::ShaderBlueprintHeader shaderBlueprintHeader;
		mMemoryFile.read(&shaderBlueprintHeader, sizeof(v1ShaderBlueprint::ShaderBlueprintHeader));

		// Sanity check
		RHI_ASSERT(shaderBlueprintHeader.numberOfShaderSourceCodeBytes > 0, "Invalid shader blueprint asset without any shader source code detected")

		// Read the asset IDs of the shader pieces to include
		if (shaderBlueprintHeader.numberOfIncludeShaderPieceAssetIds > 0)
		{
			// Allocate more temporary memory, if required
			if (mMaximumNumberOfIncludeShaderPieceAssetIds < shaderBlueprintHeader.numberOfIncludeShaderPieceAssetIds)
			{
				mMaximumNumberOfIncludeShaderPieceAssetIds = shaderBlueprintHeader.numberOfIncludeShaderPieceAssetIds;
				delete [] mIncludeShaderPieceAssetIds;
				mIncludeShaderPieceAssetIds = new AssetId[mMaximumNumberOfIncludeShaderPieceAssetIds];
			}

			// Read the asset IDs of the shader pieces to include
			mMemoryFile.read(mIncludeShaderPieceAssetIds, sizeof(AssetId) * shaderBlueprintHeader.numberOfIncludeShaderPieceAssetIds);
			mShaderBlueprintResource->mIncludeShaderPieceResourceIds.resize(shaderBlueprintHeader.numberOfIncludeShaderPieceAssetIds);
		}
		else
		{
			mShaderBlueprintResource->mIncludeShaderPieceResourceIds.clear();
		}

		{ // Read the referenced shader properties
			ShaderProperties::SortedPropertyVector& sortedPropertyVector = mShaderBlueprintResource->mReferencedShaderProperties.getSortedPropertyVector();
			if (shaderBlueprintHeader.numberOfReferencedShaderProperties > 0)
			{
				sortedPropertyVector.resize(shaderBlueprintHeader.numberOfReferencedShaderProperties);
				mMemoryFile.read(sortedPropertyVector.data(), sizeof(ShaderProperties::Property) * shaderBlueprintHeader.numberOfReferencedShaderProperties);
			}
			else
			{
				sortedPropertyVector.clear();
			}
		}

		{ // Read the shader blueprint ASCII source code
			// Allocate more temporary memory, if required
			if (mMaximumNumberOfShaderSourceCodeBytes < shaderBlueprintHeader.numberOfShaderSourceCodeBytes)
			{
				mMaximumNumberOfShaderSourceCodeBytes = shaderBlueprintHeader.numberOfShaderSourceCodeBytes;
				delete [] mShaderSourceCode;
				mShaderSourceCode = new char[mMaximumNumberOfShaderSourceCodeBytes];
			}

			// Read the shader blueprint ASCII source code
			mMemoryFile.read(mShaderSourceCode, shaderBlueprintHeader.numberOfShaderSourceCodeBytes);
			mShaderBlueprintResource->mShaderSourceCode.assign(mShaderSourceCode, mShaderSourceCode + shaderBlueprintHeader.numberOfShaderSourceCodeBytes);
		}
	}

	bool ShaderBlueprintResourceLoader::onDispatch()
	{
		{ // Read the shader piece resources to include
			ShaderPieceResourceManager& shaderPieceResourceManager = mRenderer.getShaderPieceResourceManager();
			ShaderBlueprintResource::IncludeShaderPieceResourceIds& includeShaderPieceResourceIds = mShaderBlueprintResource->mIncludeShaderPieceResourceIds;
			const size_t numberOfShaderPieceResources = includeShaderPieceResourceIds.size();
			const RECore::AssetId* includeShaderPieceAssetIds = mIncludeShaderPieceAssetIds;
			for (size_t i = 0; i < numberOfShaderPieceResources; ++i, ++includeShaderPieceAssetIds)
			{
				shaderPieceResourceManager.loadShaderPieceResourceByAssetId(*includeShaderPieceAssetIds, includeShaderPieceResourceIds[i]);
			}
		}

		// TODO(naetherm) Cleanup: Get all influenced material blueprint resources
		if (getReload())
		{
			const ShaderBlueprintResourceId shaderBlueprintResourceId = mShaderBlueprintResource->getId();
			typedef std::unordered_set<MaterialBlueprintResource*> MaterialBlueprintResourcePointers;
			MaterialBlueprintResourcePointers materialBlueprintResourcePointers;
			const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
			const RECore::uint32 numberOfElements = materialBlueprintResourceManager.getNumberOfResources();
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				MaterialBlueprintResource& materialBlueprintResource = materialBlueprintResourceManager.getByIndex(i);
				const ShaderBlueprintResourceId computeShaderBlueprintResourceId = materialBlueprintResource.getComputeShaderBlueprintResourceId();
				if (RECore::isValid(computeShaderBlueprintResourceId))
				{
					// Compute pipeline state object (PSO)
					if (shaderBlueprintResourceId == computeShaderBlueprintResourceId)
					{
						materialBlueprintResourcePointers.insert(&materialBlueprintResource);
					}
				}
				else
				{
					// Graphics pipeline state object (PSO)
					for (RECore::uint8 graphicsShaderType = 0; graphicsShaderType < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++graphicsShaderType)
					{
						if (materialBlueprintResource.getGraphicsShaderBlueprintResourceId(static_cast<GraphicsShaderType>(graphicsShaderType)) == shaderBlueprintResourceId)
						{
							materialBlueprintResourcePointers.insert(&materialBlueprintResource);
							break;
						}
					}
				}
			}
			for (MaterialBlueprintResource* materialBlueprintResource : materialBlueprintResourcePointers)
			{
				materialBlueprintResource->getGraphicsPipelineStateCacheManager().clearCache();
				materialBlueprintResource->getGraphicsPipelineStateCacheManager().getGraphicsProgramCacheManager().clearCache();
				materialBlueprintResource->getComputePipelineStateCacheManager().clearCache();
				++const_cast<MaterialProperties&>(materialBlueprintResource->getMaterialProperties()).mShaderCombinationGenerationCounter;
			}

			// TODO(naetherm) Do only clear the influenced shader cache entries
			mRenderer.getShaderBlueprintResourceManager().getShaderCacheManager().clearCache();
		}

		// Fully loaded?
		return isFullyLoaded();
	}

	bool ShaderBlueprintResourceLoader::isFullyLoaded()
	{
		const ShaderPieceResourceManager& shaderPieceResourceManager = mRenderer.getShaderPieceResourceManager();
		for (ShaderPieceResourceId shaderPieceResourceId : mShaderBlueprintResource->mIncludeShaderPieceResourceIds)
		{
			if (RECore::isValid(shaderPieceResourceId) && RECore::IResource::LoadingState::LOADED != shaderPieceResourceManager.getResourceByResourceId(shaderPieceResourceId).getLoadingState())
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
