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
#include "RERenderer/Resource/ShaderBlueprint/GraphicsShaderType.h"
#include "RERenderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class ShaderBlueprintResource;
	class MaterialBlueprintResource;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MaterialBlueprintResourceId;		///< POD material blueprint resource identifier
	typedef RECore::uint32 GraphicsPipelineStateSignatureId;	///< Graphics pipeline state signature identifier, result of hashing the referenced shaders as well as other pipeline state properties
	typedef RECore::uint32 ShaderCombinationId;				///< Shader combination identifier, result of hashing the shader combination generating shader blueprint resource, shader properties and dynamic shader pieces


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Graphics pipeline state signature
	*
	*  @see
	*    - See "RERenderer::GraphicsPipelineStateCacheManager" for additional information
	*/
	class GraphicsPipelineStateSignature final
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] static ShaderCombinationId generateShaderCombinationId(const ShaderBlueprintResource& shaderBlueprintResource, const ShaderProperties& shaderProperties);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline GraphicsPipelineStateSignature() :
			mMaterialBlueprintResourceId(RECore::getInvalid<MaterialBlueprintResourceId>()),
			mSerializedGraphicsPipelineStateHash(RECore::getInvalid<RECore::uint32>()),
			mGraphicsPipelineStateSignatureId(RECore::getInvalid<GraphicsPipelineStateSignatureId>()),
			mShaderCombinationId{RECore::getInvalid<ShaderCombinationId>(), RECore::getInvalid<ShaderCombinationId>(), RECore::getInvalid<ShaderCombinationId>(), RECore::getInvalid<ShaderCombinationId>(), RECore::getInvalid<ShaderCombinationId>()}
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] serializedGraphicsPipelineStateHash
		*    FNV1a hash of "RERHI::SerializedGraphicsPipelineState"
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "RERenderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		inline GraphicsPipelineStateSignature(const MaterialBlueprintResource& materialBlueprintResource, RECore::uint32 serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties)
		{
			set(materialBlueprintResource, serializedGraphicsPipelineStateHash, shaderProperties);
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] graphicsPipelineStateSignature
		*    Graphics pipeline state signature to copy from
		*/
		explicit GraphicsPipelineStateSignature(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~GraphicsPipelineStateSignature()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy operator
		*/
		GraphicsPipelineStateSignature& operator=(const GraphicsPipelineStateSignature& graphicsPipelineStateSignature);

		/**
		*  @brief
		*    Setter
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] serializedGraphicsPipelineStateHash
		*    FNV1a hash of "RERHI::SerializedGraphicsPipelineState"
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "Renderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		void set(const MaterialBlueprintResource& materialBlueprintResource, RECore::uint32 serializedGraphicsPipelineStateHash, const ShaderProperties& shaderProperties);

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline MaterialBlueprintResourceId getMaterialBlueprintResourceId() const
		{
			return mMaterialBlueprintResourceId;
		}

		[[nodiscard]] inline RECore::uint32 getSerializedGraphicsPipelineStateHash() const
		{
			return mSerializedGraphicsPipelineStateHash;
		}

		[[nodiscard]] inline const ShaderProperties& getShaderProperties() const
		{
			return mShaderProperties;
		}

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline GraphicsPipelineStateSignatureId getGraphicsPipelineStateSignatureId() const
		{
			return mGraphicsPipelineStateSignatureId;
		}

		[[nodiscard]] inline ShaderCombinationId getShaderCombinationId(GraphicsShaderType graphicsShaderType) const
		{
			return mShaderCombinationId[static_cast<RECore::uint8>(graphicsShaderType)];
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		MaterialBlueprintResourceId	mMaterialBlueprintResourceId;
		RECore::uint32					mSerializedGraphicsPipelineStateHash;
		ShaderProperties			mShaderProperties;
		// Derived data
		GraphicsPipelineStateSignatureId mGraphicsPipelineStateSignatureId;
		ShaderCombinationId				 mShaderCombinationId[NUMBER_OF_GRAPHICS_SHADER_TYPES];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
