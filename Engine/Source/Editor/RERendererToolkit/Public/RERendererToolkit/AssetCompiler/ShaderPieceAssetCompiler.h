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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ShaderPieceAssetCompiler final : public IAssetCompiler
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr uint32_t CLASS_ID = STRING_ID("RendererToolkit::ShaderPieceAssetCompiler");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline ShaderPieceAssetCompiler()
		{
			// Nothing here
		}

		inline virtual ~ShaderPieceAssetCompiler() override
		{
			// Nothing here
		}


	//[-------------------------------------------------------]
	//[ Public virtual RERendererToolkit::IAssetCompiler methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual AssetCompilerClassId getAssetCompilerClassId() const override
		{
			return CLASS_ID;
		}

		[[nodiscard]] inline virtual std::string_view getOptionalUniqueAssetFilenameExtension() const override
		{
			return ".shader_piece";
		}

		[[nodiscard]] virtual std::string getVirtualOutputAssetFilename(const Input& input, const Configuration& configuration) const override;
		[[nodiscard]] virtual bool checkIfChanged(const Input& input, const Configuration& configuration) const override;
		virtual void compile(const Input& input, const Configuration& configuration) const override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
