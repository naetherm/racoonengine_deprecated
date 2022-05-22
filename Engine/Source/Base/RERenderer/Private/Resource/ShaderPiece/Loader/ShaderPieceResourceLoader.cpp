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
#include "RERenderer/Resource/ShaderPiece/Loader/ShaderPieceResourceLoader.h"
#include "RERenderer/Resource/ShaderPiece/Loader/ShaderPieceFileFormat.h"
#include "RERenderer/Resource/ShaderPiece/ShaderPieceResource.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
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
	void ShaderPieceResourceLoader::initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mShaderPieceResource = static_cast<ShaderPieceResource*>(&resource);
	}

	bool ShaderPieceResourceLoader::onDeserialization(RECore::IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1ShaderPiece::FORMAT_TYPE, v1ShaderPiece::FORMAT_VERSION, file);
	}

	void ShaderPieceResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the shader piece header
		v1ShaderPiece::ShaderPieceHeader shaderPieceHeader;
		mMemoryFile.read(&shaderPieceHeader, sizeof(v1ShaderPiece::ShaderPieceHeader));

		// Sanity check
		RHI_ASSERT(shaderPieceHeader.numberOfShaderSourceCodeBytes > 0, "Invalid shader piece asset without any shader source code detected")

		// Allocate more temporary memory, if required
		if (mMaximumNumberOfShaderSourceCodeBytes < shaderPieceHeader.numberOfShaderSourceCodeBytes)
		{
			mMaximumNumberOfShaderSourceCodeBytes = shaderPieceHeader.numberOfShaderSourceCodeBytes;
			delete [] mShaderSourceCode;
			mShaderSourceCode = new char[mMaximumNumberOfShaderSourceCodeBytes];
		}

		// Read the shader piece ASCII source code
		mMemoryFile.read(mShaderSourceCode, shaderPieceHeader.numberOfShaderSourceCodeBytes);
		mShaderPieceResource->mShaderSourceCode.assign(mShaderSourceCode, mShaderSourceCode + shaderPieceHeader.numberOfShaderSourceCodeBytes);
	}

	bool ShaderPieceResourceLoader::onDispatch()
	{
		// TODO(naetherm) Cleanup: Get all influenced material blueprint resources
		if (getReload())
		{
			const ShaderPieceResourceId shaderPieceResourceId = mShaderPieceResource->getId();
			const ShaderBlueprintResourceManager& shaderBlueprintResourceManager = mRenderer.getShaderBlueprintResourceManager();
			typedef std::unordered_set<MaterialBlueprintResource*> MaterialBlueprintResourcePointers;
			MaterialBlueprintResourcePointers materialBlueprintResourcePointers;
			const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
			const RECore::uint32 numberOfElements = materialBlueprintResourceManager.getNumberOfResources();
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				MaterialBlueprintResource& materialBlueprintResource = materialBlueprintResourceManager.getByIndex(i);
				ShaderBlueprintResourceId shaderBlueprintResourceId = materialBlueprintResource.getComputeShaderBlueprintResourceId();
				if (RECore::isValid(shaderBlueprintResourceId))
				{
					// Compute pipeline state object (PSO)
					const ShaderBlueprintResource::IncludeShaderPieceResourceIds& includeShaderPieceResourceIds = shaderBlueprintResourceManager.getById(shaderBlueprintResourceId).getIncludeShaderPieceResourceIds();
					if (std::find(includeShaderPieceResourceIds.cbegin(), includeShaderPieceResourceIds.cend(), shaderPieceResourceId) != includeShaderPieceResourceIds.cend())
					{
						materialBlueprintResourcePointers.insert(&materialBlueprintResource);
					}
				}
				else
				{
					// Graphics pipeline state object (PSO)
					for (RECore::uint8 graphicsShaderType = 0; graphicsShaderType < NUMBER_OF_GRAPHICS_SHADER_TYPES; ++graphicsShaderType)
					{
						shaderBlueprintResourceId = materialBlueprintResource.getGraphicsShaderBlueprintResourceId(static_cast<GraphicsShaderType>(graphicsShaderType));
						if (RECore::isValid(shaderBlueprintResourceId))
						{
							const ShaderBlueprintResource::IncludeShaderPieceResourceIds& includeShaderPieceResourceIds = shaderBlueprintResourceManager.getById(shaderBlueprintResourceId).getIncludeShaderPieceResourceIds();
							if (std::find(includeShaderPieceResourceIds.cbegin(), includeShaderPieceResourceIds.cend(), shaderPieceResourceId) != includeShaderPieceResourceIds.cend())
							{
								materialBlueprintResourcePointers.insert(&materialBlueprintResource);
								break;
							}
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

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
