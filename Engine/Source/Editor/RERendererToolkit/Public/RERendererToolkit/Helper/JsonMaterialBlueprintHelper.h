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
#include "RERendererToolkit/AssetCompiler/IAssetCompiler.h"

#include <RERenderer/Resource/Material/MaterialProperties.h>
#include <RECore/Asset/Asset.h>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
class IFile;
}
namespace RERenderer
{
	class ShaderProperties;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::unordered_map<uint32_t, std::string> MaterialPropertyIdToName;				///< Key = "RERenderer::MaterialPropertyId"
	typedef std::unordered_map<uint32_t, uint32_t> SamplerBaseShaderRegisterNameToIndex;	///< Key = "RECore::StringId(<Base Shader Register Name>)", value = index of the material blueprint sampler state resource to use


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class JsonMaterialBlueprintHelper final
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void loadDocumentByFilename(const IAssetCompiler::Input& input, const std::string& virtualFilename, rapidjson::Document& rapidJsonDocument);
		static void optionalPrimitiveTopology(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::PrimitiveTopology& value);
		[[nodiscard]] static RERHI::PrimitiveTopologyType getPrimitiveTopologyTypeByPrimitiveTopology(RERHI::PrimitiveTopology primitiveTopology);
		static void optionalShaderVisibilityProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::ShaderVisibility& value);
		[[nodiscard]] static RERenderer::MaterialProperty::Usage mandatoryMaterialPropertyUsage(const rapidjson::Value& rapidJsonValue);
		[[nodiscard]] static RERenderer::MaterialProperty::ValueType mandatoryMaterialPropertyValueType(const rapidjson::Value& rapidJsonValue);
		static void getPropertiesByMaterialBlueprintAssetId(const IAssetCompiler::Input& input, RECore::AssetId materialBlueprintAssetId, RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector, MaterialPropertyIdToName* materialPropertyIdToName = nullptr);
		[[nodiscard]] static RERenderer::MaterialPropertyValue mandatoryMaterialPropertyValue(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValue, const char* propertyName, const RERenderer::MaterialProperty::ValueType valueType);
		static void readRootSignatureByResourceGroups(const rapidjson::Value& rapidJsonValueResourceGroups, RECore::IFile& file, bool isComputeMaterialBlueprint);
		static void readProperties(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValueProperties, RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector, RERenderer::ShaderProperties& visualImportanceOfShaderProperties, RERenderer::ShaderProperties& maximumIntegerValueOfShaderProperties, bool ignoreGlobalReferenceFallback, bool sort, bool referencesAllowed, MaterialPropertyIdToName* materialPropertyIdToName = nullptr);
		static void readComputePipelineStateObject(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValueComputePipelineState, RECore::IFile& file, const RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector);
		static void readGraphicsPipelineStateObject(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValueGraphicsPipelineState, RECore::IFile& file, const RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector);
		static void readUniformBuffersByResourceGroups(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValueResourceGroups, RECore::IFile& file);
		static void readTextureBuffersByResourceGroups(const rapidjson::Value& rapidJsonValueResourceGroups, RECore::IFile& file);
		static void readSamplerStatesByResourceGroups(const rapidjson::Value& rapidJsonValueResourceGroups, const RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector, RECore::IFile& file, SamplerBaseShaderRegisterNameToIndex& samplerBaseShaderRegisterNameToIndex);
		static void readTexturesByResourceGroups(const IAssetCompiler::Input& input, const RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector, const rapidjson::Value& rapidJsonValueResourceGroups, const SamplerBaseShaderRegisterNameToIndex& samplerBaseShaderRegisterNameToIndex, RECore::IFile& file);
		static void getDependencyFiles(const IAssetCompiler::Input& input, const std::string& virtualInputFilename, std::vector<std::string>& virtualDependencyFilenames);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		JsonMaterialBlueprintHelper(const JsonMaterialBlueprintHelper&) = delete;
		JsonMaterialBlueprintHelper& operator=(const JsonMaterialBlueprintHelper&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
