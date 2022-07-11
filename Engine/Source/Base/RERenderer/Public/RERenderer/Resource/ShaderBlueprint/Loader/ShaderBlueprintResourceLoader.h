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
#include <RECore/Resource/IResourceLoader.h>
#include <RECore/File/MemoryFile.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class IRenderer;
	class ShaderBlueprintResource;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 ShaderBlueprintResourceId;	///< POD shader blueprint resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ShaderBlueprintResourceLoader final : public RECore::IResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend ResourceManagerTemplate<ShaderBlueprintResource, ShaderBlueprintResourceLoader, ShaderBlueprintResourceId, 128>;	// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("shader_blueprint");


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual RECore::ResourceLoaderTypeId getResourceLoaderTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource) override;

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
		[[nodiscard]] virtual bool isFullyLoaded() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	public:
		inline ShaderBlueprintResourceLoader(RECore::IResourceManager& resourceManager, IRenderer& renderer) :
			IResourceLoader(resourceManager),
			mRenderer(renderer),
			mShaderBlueprintResource(nullptr),
			mMaximumNumberOfIncludeShaderPieceAssetIds(0),
			mIncludeShaderPieceAssetIds(nullptr),
			mMaximumNumberOfShaderSourceCodeBytes(0),
			mShaderSourceCode(nullptr)
		{
			// Nothing here
		}

		inline virtual ~ShaderBlueprintResourceLoader() override
		{
			// Free temporary data
			delete [] mIncludeShaderPieceAssetIds;
			delete [] mShaderSourceCode;
		}

		explicit ShaderBlueprintResourceLoader(const ShaderBlueprintResourceLoader&) = delete;
		ShaderBlueprintResourceLoader& operator=(const ShaderBlueprintResourceLoader&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&				 mRenderer;					///< Renderer instance, do not destroy the instance
		ShaderBlueprintResource* mShaderBlueprintResource;	///< Destination resource

		// Temporary data
		RECore::MemoryFile mMemoryFile;
		RECore::uint32   mMaximumNumberOfIncludeShaderPieceAssetIds;
    RECore::AssetId*   mIncludeShaderPieceAssetIds;
		RECore::uint32   mMaximumNumberOfShaderSourceCodeBytes;
		char*	   mShaderSourceCode;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
