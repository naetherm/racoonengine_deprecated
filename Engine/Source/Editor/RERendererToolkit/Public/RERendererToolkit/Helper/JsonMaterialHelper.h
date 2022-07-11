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
#include "RERendererToolkit/AssetCompiler/IAssetCompiler.h"

#include <RERenderer/Resource/Material/MaterialProperties.h>
#include <RERenderer/Resource/Material/Loader/MaterialFileFormat.h>
#include <RECore/Asset/Asset.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class JsonMaterialHelper final
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void optionalFillModeProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::FillMode& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalCullModeProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::CullMode& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalConservativeRasterizationModeProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::ConservativeRasterizationMode& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalDepthWriteMaskProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::DepthWriteMask& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalStencilOpProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::StencilOp& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalBlendProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::Blend& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalBlendOpProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::BlendOp& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalFilterProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::FilterMode& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalTextureAddressModeProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::TextureAddressMode& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void optionalComparisonFuncProperty(const rapidjson::Value& rapidJsonValue, const char* propertyName, RERHI::ComparisonFunc& value, const RERenderer::MaterialProperties::SortedPropertyVector* sortedMaterialPropertyVector = nullptr);
		static void readMaterialPropertyValues(const IAssetCompiler::Input& input, const rapidjson::Value& rapidJsonValueProperties, RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector);
		static void getTechniquesAndPropertiesByMaterialAssetId(const IAssetCompiler::Input& input, rapidjson::Document& rapidJsonDocument, std::vector<RERenderer::v1Material::Technique>& techniques, RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector);
		static void getPropertiesByMaterialAssetId(const IAssetCompiler::Input& input, RECore::AssetId materialAssetId, RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector, std::vector<RERenderer::v1Material::Technique>* techniques = nullptr);
		static void getDependencyFiles(const IAssetCompiler::Input& input, const std::string& virtualInputFilename, std::vector<std::string>& virtualDependencyFilenames);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		JsonMaterialHelper(const JsonMaterialHelper&) = delete;
		JsonMaterialHelper& operator=(const JsonMaterialHelper&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
