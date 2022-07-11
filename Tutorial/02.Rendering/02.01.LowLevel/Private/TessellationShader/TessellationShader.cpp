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
#include "TessellationShader/TessellationShader.h"
#include <RECore/Color/Color4.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void TessellationShader::onInitialization()
{
	// Get and check the RHI instance
	// -> Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> Tessellation control and tessellation evaluation shaders supported?
	RERHI::RHIDynamicRHIPtr rhi(getRhi());
	if (nullptr != rhi && rhi->getCapabilities().maximumUniformBufferSize > 0 && rhi->getCapabilities().maximumNumberOfPatchVertices > 0)
	{
		// Create the buffer manager
		mBufferManager = rhi->createBufferManager();

		{ // Create the root signature
			// Setup
			RERHI::RootSignatureBuilder rootSignatureBuilder;
			rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// Create the instance
			mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
		}

		// Vertex input layout
		static constexpr RERHI::VertexAttribute vertexAttributesLayout[] =
		{
			{ // Attribute 0
				// Data destination
				RERHI::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
				"Position",								// name[32] (char)
				"POSITION",								// semanticName[32] (char)
				0,										// semanticIndex (RECore::uint32)
				// Data source
				0,										// inputSlot (RECore::uint32)
				0,										// alignedByteOffset (RECore::uint32)
				sizeof(float) * 2,						// strideInBytes (RECore::uint32)
				0										// instancesPerElement (RECore::uint32)
			}
		};
		const RERHI::VertexAttributes vertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

		{ // Create vertex array object (VAO)
			// Create the vertex buffer object (VBO)
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
			static constexpr float VERTEX_POSITION[] =
			{					// Vertex ID	Triangle on screen
				 0.0f, 1.0f,	// 0				0
				 1.0f, 0.0f,	// 1			   .   .
				-0.5f, 0.0f		// 2			  2.......1
			};
			RERHI::RHIVertexBufferPtr vertexBuffer(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION));

			// Create vertex array object (VAO)
			// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
			// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
			// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
			//    reference of the used vertex buffer objects (VBO). If the reference counter of a
			//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
			const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
			mVertexArray = mBufferManager->createVertexArray(vertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers);
		}

		{
			// Create the graphics program
			RERHI::RHIGraphicsProgramPtr graphicsProgram;
			{
				// Get the shader source code (outsourced to keep an overview)
				const char* vertexShaderSourceCode = nullptr;
				const char* tessellationControlShaderSourceCode = nullptr;
				const char* tessellationEvaluationShaderSourceCode = nullptr;
				const char* fragmentShaderSourceCode = nullptr;
				#include "TessellationShader/TessellationShader_GLSL_450.h"	// For Vulkan
				#include "TessellationShader/TessellationShader_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
				#include "TessellationShader/TessellationShader_HLSL_D3D11_D3D12.h"
				#include "TessellationShader/TessellationShader_Null.h"

				// Create the graphics program
				RERHI::RHIShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
				graphicsProgram = shaderLanguage.createGraphicsProgram(
					*mRootSignature,
					vertexAttributes,
					shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
					shaderLanguage.createTessellationControlShaderFromSourceCode(tessellationControlShaderSourceCode),
					shaderLanguage.createTessellationEvaluationShaderFromSourceCode(tessellationEvaluationShaderSourceCode),
					shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
			}

			// Create the graphics pipeline state object (PSO)
			if (nullptr != graphicsProgram)
			{
				RERHI::GraphicsPipelineState graphicsPipelineState = RERHI::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass());
				graphicsPipelineState.primitiveTopology = RERHI::PrimitiveTopology::PATCH_LIST_3;	// Patch list with 3 vertices per patch (tessellation relevant topology type) - "RERHI::PrimitiveTopology::TriangleList" used for tessellation
				graphicsPipelineState.primitiveTopologyType = RERHI::PrimitiveTopologyType::PATCH;
				graphicsPipelineState.rasterizerState.fillMode = RERHI::FillMode::WIREFRAME;
				mGraphicsPipelineState = rhi->createGraphicsPipelineState(graphicsPipelineState);
			}
		}

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		fillCommandBuffer();
	}
}

void TessellationShader::onDeinitialization()
{
	// Release the used resources
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

void TessellationShader::onDraw(RERHI::RHICommandBuffer& commandBuffer)
{
	// Dispatch pre-recorded command buffer
	RERHI::Command::DispatchCommandBuffer::create(commandBuffer, &mCommandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void TessellationShader::fillCommandBuffer()
{
	// Sanity checks
	ASSERT(nullptr != getRhi(), "Invalid RHI instance")
	RHI_ASSERT(getRhi()->getContext(), mCommandBuffer.isEmpty(), "The command buffer is already filled")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mRootSignature, "Invalid root signature")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mVertexArray, "Invalid vertex array")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

	// Set the used graphics root signature
	RERHI::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	RERHI::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Input assembly (IA): Set the used vertex array
	RERHI::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on an array of vertices
	RERHI::Command::DrawGraphics::create(mCommandBuffer, 3);
}
