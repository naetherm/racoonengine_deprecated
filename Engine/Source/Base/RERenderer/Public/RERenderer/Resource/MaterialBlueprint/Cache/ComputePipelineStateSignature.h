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
#include "RERenderer/Resource/ShaderBlueprint/Cache/ShaderProperties.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
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
	typedef RECore::uint32 ComputePipelineStateSignatureId;	///< Compute pipeline state signature identifier, result of hashing the referenced shaders as well as other pipeline state properties
	typedef RECore::uint32 ShaderCombinationId;				///< Shader combination identifier, result of hashing the shader combination generating shader blueprint resource, shader properties and dynamic shader pieces


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compute pipeline state signature
	*
	*  @see
	*    - See "RERenderer::ComputePipelineStateCacheManager" for additional information
	*/
	class ComputePipelineStateSignature final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ComputePipelineStateSignature() :
			mMaterialBlueprintResourceId(RECore::getInvalid<MaterialBlueprintResourceId>()),
			mComputePipelineStateSignatureId(RECore::getInvalid<ComputePipelineStateSignatureId>()),
			mShaderCombinationId(RECore::getInvalid<ShaderCombinationId>())
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "RERenderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		inline ComputePipelineStateSignature(const MaterialBlueprintResource& materialBlueprintResource, const ShaderProperties& shaderProperties)
		{
			set(materialBlueprintResource, shaderProperties);
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] computePipelineStateSignature
		*    Compute pipeline state signature to copy from
		*/
		explicit ComputePipelineStateSignature(const ComputePipelineStateSignature& computePipelineStateSignature);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ComputePipelineStateSignature()
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy operator
		*/
		ComputePipelineStateSignature& operator=(const ComputePipelineStateSignature& computePipelineStateSignature);

		/**
		*  @brief
		*    Setter
		*
		*  @param[in] materialBlueprintResource
		*    Material blueprint resource to use
		*  @param[in] shaderProperties
		*    Shader properties to use, you should ensure that this shader properties are already optimized by using e.g. "RERenderer::MaterialBlueprintResource::optimizeShaderProperties()"
		*/
		void set(const MaterialBlueprintResource& materialBlueprintResource, const ShaderProperties& shaderProperties);

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline MaterialBlueprintResourceId getMaterialBlueprintResourceId() const
		{
			return mMaterialBlueprintResourceId;
		}

		[[nodiscard]] inline const ShaderProperties& getShaderProperties() const
		{
			return mShaderProperties;
		}

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline ComputePipelineStateSignatureId getComputePipelineStateSignatureId() const
		{
			return mComputePipelineStateSignatureId;
		}

		[[nodiscard]] inline ShaderCombinationId getShaderCombinationId() const
		{
			return mShaderCombinationId;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		MaterialBlueprintResourceId	mMaterialBlueprintResourceId;
		ShaderProperties			mShaderProperties;
		// Derived data
		ComputePipelineStateSignatureId	mComputePipelineStateSignatureId;
		ShaderCombinationId				mShaderCombinationId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
