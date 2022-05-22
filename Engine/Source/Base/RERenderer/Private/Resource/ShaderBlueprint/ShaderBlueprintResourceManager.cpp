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
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include "RERenderer/Resource/ShaderBlueprint/Loader/ShaderBlueprintResourceLoader.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/IRenderer.h"


// Disable warnings
// TODO(naetherm) See "RERenderer::ShaderBlueprintResourceManager::ShaderBlueprintResourceManager()": How the heck should we avoid such a situation without using complicated solutions like a pointer to an instance? (= more individual allocations/deallocations)
PRAGMA_WARNING_DISABLE_MSVC(4355)	// warning C4355: 'this': used in base member initializer list


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void ShaderBlueprintResourceManager::loadShaderBlueprintResourceByAssetId(AssetId assetId, ShaderBlueprintResourceId& shaderBlueprintResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		mInternalResourceManager->loadResourceByAssetId(assetId, shaderBlueprintResourceId, resourceListener, reload, resourceLoaderTypeId);
	}

	void ShaderBlueprintResourceManager::setInvalidResourceId(ShaderBlueprintResourceId& shaderBlueprintResourceId, RECore::IResourceListener& resourceListener) const
	{
		ShaderBlueprintResource* shaderBlueprintResource = tryGetById(shaderBlueprintResourceId);
		if (nullptr != shaderBlueprintResource)
		{
			shaderBlueprintResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(shaderBlueprintResourceId);
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 ShaderBlueprintResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& ShaderBlueprintResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& ShaderBlueprintResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* ShaderBlueprintResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void ShaderBlueprintResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		return mInternalResourceManager->reloadResourceByAssetId(assetId);
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* ShaderBlueprintResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new ShaderBlueprintResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	ShaderBlueprintResourceManager::ShaderBlueprintResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mShaderCacheManager(*this)
	{
		mInternalResourceManager = new RECore::ResourceManagerTemplate<ShaderBlueprintResource, ShaderBlueprintResourceLoader, ShaderBlueprintResourceId, 128>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);

		// Gather RHI shader properties
		// -> Write the RHI name as well as the shader language name into the shader properties so shaders can perform RHI specific handling if required
		// -> We really need both, usually shader language name is sufficient, but if more fine granular information is required it's accessible
		RERHI::RHIDynamicRHI& rhi = mRenderer.getRhi();
		const RERHI::Capabilities& capabilities = rhi.getCapabilities();
		mRhiShaderProperties.setPropertyValue(static_cast<RECore::uint32>(rhi.getNameId()), 1);
		mRhiShaderProperties.setPropertyValue(STRING_ID("ZeroToOneClipZ"), capabilities.zeroToOneClipZ ? 1 : 0);
		mRhiShaderProperties.setPropertyValue(STRING_ID("UpperLeftOrigin"), capabilities.upperLeftOrigin ? 1 : 0);
		mRhiShaderProperties.setPropertyValue(RECore::StringId(rhi.getDefaultShaderLanguage().getShaderLanguageName()), 1);
	}

	ShaderBlueprintResourceManager::~ShaderBlueprintResourceManager()
	{
		delete mInternalResourceManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
