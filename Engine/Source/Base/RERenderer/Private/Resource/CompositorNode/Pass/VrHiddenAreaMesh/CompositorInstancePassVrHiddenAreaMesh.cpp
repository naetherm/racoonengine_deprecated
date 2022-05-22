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
#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/CompositorInstancePassVrHiddenAreaMesh.h"
#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/CompositorResourcePassVrHiddenAreaMesh.h"
#include "RERenderer/Resource/CompositorNode/CompositorNodeInstance.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h"
#include "RERenderer/Core/IProfiler.h"
#ifdef RENDERER_OPENVR
	#include "RERenderer/Vr/OpenVR/VrManagerOpenVR.h"
#endif
#include "RERenderer/IRenderer.h"
#include "RERenderer/Context.h"
#include <RECore/Log/Log.h>


#ifdef RENDERER_OPENVR
	//[-------------------------------------------------------]
	//[ Anonymous detail namespace                            ]
	//[-------------------------------------------------------]
	namespace
	{
		namespace detail
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			class Mesh : public RECore::RefCount<Mesh>
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				explicit Mesh(const RERenderer::IRenderer& renderer) :
					mNumberOfTriangles(0)
				{
					RERHI::RHIDynamicRHI& rhi = renderer.getRhi();

					{ // Create the root signature
						// Setup
						RERHI::RootSignatureBuilder rootSignatureBuilder;
						rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

						// Create the instance
						mRootSignature = rhi.createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"));
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

					{ // Create vertex array and merge both meshes into a single mesh since we're using single pass stereo rendering via instancing as described in "High Performance Stereo Rendering For VR", Timothy Wilson, San Diego, Virtual Reality Meetup
						RERHI::RHIBufferManager& bufferManager = renderer.getBufferManager();
						vr::IVRSystem* vrSystem = static_cast<RERenderer::VrManagerOpenVR&>(renderer.getVrManager()).getVrSystem();

						// Get the combined number of vertex buffer bytes and triangles
						RECore::uint32 numberOfBytes = 0;
						for (int vrEyeIndex = 0; vrEyeIndex < 2; ++vrEyeIndex)
						{
							const vr::HiddenAreaMesh_t vrHiddenAreaMesh = vrSystem->GetHiddenAreaMesh(static_cast<vr::EVREye>(vrEyeIndex));
							mNumberOfTriangles += vrHiddenAreaMesh.unTriangleCount;
							numberOfBytes += sizeof(vr::HmdVector2_t) * 3 * vrHiddenAreaMesh.unTriangleCount;
						}

						// Allocate temporary vertex buffer memory, if necessary
						// -> For HTC Vive there are tiny 1248 bytes which can be easily put onto the C-runtime-stack to avoid a memory allocation
						static constexpr RECore::uint32 STACK_NUMBER_OF_BYTES = 1248;
						RECore::uint8 stackMemory[STACK_NUMBER_OF_BYTES];
						RECore::uint8* temporaryMemory = (numberOfBytes <= STACK_NUMBER_OF_BYTES) ? stackMemory : new RECore::uint8[numberOfBytes];

						{ // Fill temporary vertex buffer memory
							float* currentTemporaryMemory = reinterpret_cast<float*>(temporaryMemory);
							for (int vrEyeIndex = 0; vrEyeIndex < 2; ++vrEyeIndex)
							{
								const vr::HiddenAreaMesh_t vrHiddenAreaMesh = vrSystem->GetHiddenAreaMesh(static_cast<vr::EVREye>(vrEyeIndex));
								const float offset = (0 == vrEyeIndex) ? 0.0f : 0.5f;
								const vr::HmdVector2_t* currentSourceVertexData = vrHiddenAreaMesh.pVertexData;
								const vr::HmdVector2_t* sourceVertexDataEnd = currentSourceVertexData + 3 * vrHiddenAreaMesh.unTriangleCount;
								for (; currentSourceVertexData < sourceVertexDataEnd; ++currentSourceVertexData, currentTemporaryMemory += 2)
								{
									currentTemporaryMemory[0] = currentSourceVertexData->v[0] * 0.5f + offset;
									currentTemporaryMemory[1] = currentSourceVertexData->v[1];
								}
							}
						}

						// Create the vertex buffer object (VBO)
						RERHI::RHIVertexBufferPtr vertexBuffer(bufferManager.createVertexBuffer(numberOfBytes, temporaryMemory, 0, RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh")));

						{ // Create vertex array object (VAO)
							const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
							mVertexArrayPtr = bufferManager.createVertexArray(vertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"));
						}

						// Free allocated temporary vertex buffer memory, if necessary
						if (temporaryMemory != stackMemory)
						{
							delete [] temporaryMemory;
						}
					}

					{
						// Create the graphics program
						RERHI::RHIGraphicsProgramPtr graphicsProgram;
						{
							// Get the shader source code (outsourced to keep an overview)
							const char* vertexShaderSourceCode = nullptr;
							const char* fragmentShaderSourceCode = nullptr;
							#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/Shader/VrHiddenAreaMesh_GLSL_410.h"
							#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/Shader/VrHiddenAreaMesh_GLSL_ES3.h"
							#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/Shader/VrHiddenAreaMesh_HLSL_D3D9.h"
							#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/Shader/VrHiddenAreaMesh_HLSL_D3D10_D3D11_D3D12.h"
							#include "RERenderer/Resource/CompositorNode/Pass/VrHiddenAreaMesh/Shader/VrHiddenAreaMesh_Null.h"

							// Create the graphics program
							RERHI::RHIShaderLanguage& shaderLanguage = rhi.getDefaultShaderLanguage();
							graphicsProgram = shaderLanguage.createGraphicsProgram(
								*mRootSignature,
								vertexAttributes,
								shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh")),
								shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"))
								RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"));
						}

						// Create the graphics pipeline state object (PSO)
						if (nullptr != graphicsProgram)
						{
							// TODO(naetherm) Render pass related update, the render pass in here is currently just a dummy so the debug compositor works
							RERHI::RHIRenderPass* renderPass = rhi.createRenderPass(1, &rhi.getCapabilities().preferredSwapChainColorTextureFormat, rhi.getCapabilities().preferredSwapChainDepthStencilTextureFormat, 1 RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"));

							RERHI::GraphicsPipelineState graphicsPipelineState = RERHI::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, *renderPass);
							graphicsPipelineState.rasterizerState.cullMode = RERHI::CullMode::NONE;
							mGraphicsPipelineState = rhi.createGraphicsPipelineState(graphicsPipelineState RHI_RESOURCE_DEBUG_NAME("Compositor instance pass VR hidden area mesh"));
						}
					}
				}

				~Mesh()
				{
					mRootSignature->Release();
					mVertexArrayPtr->Release();
					mGraphicsPipelineState->Release();
				}

				void onFillCommandBuffer(RERHI::RHICommandBuffer& commandBuffer)
				{
					// Set the used graphics root signature
					RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

					// Set the used graphics pipeline state object (PSO)
					RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

					// Setup input assembly (IA): Set the used vertex array
					RERHI::Command::SetGraphicsVertexArray::create(commandBuffer, mVertexArrayPtr);

					// Render the specified geometric primitive, based on an array of vertices
					RERHI::Command::DrawGraphics::create(commandBuffer, mNumberOfTriangles * 3);
				}


			//[-------------------------------------------------------]
			//[ Protected virtual RERHI::RefCount methods               ]
			//[-------------------------------------------------------]
			protected:
				virtual void selfDestruct() override
				{
					delete this;
				}


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				explicit Mesh(const Mesh&) = delete;
				Mesh& operator=(const Mesh&) = delete;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				RERHI::RHIRootSignaturePtr			mRootSignature;
				RERHI::RHIVertexArrayPtr			mVertexArrayPtr;
				RECore::uint32						mNumberOfTriangles;
				RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;	// TODO(naetherm) As soon as we support stencil in here, instance might need different graphics pipeline states


			};
			typedef RECore::SmartRefCount<Mesh> MeshPtr;


			//[-------------------------------------------------------]
			//[ Global variables                                      ]
			//[-------------------------------------------------------]
			static MeshPtr g_MeshPtr;


	//[-------------------------------------------------------]
	//[ Anonymous detail namespace                            ]
	//[-------------------------------------------------------]
		} // detail
	}
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::ICompositorInstancePass methods ]
	//[-------------------------------------------------------]
	void CompositorInstancePassVrHiddenAreaMesh::onFillCommandBuffer([[maybe_unused]] const RERHI::RHIRenderTarget* renderTarget, [[maybe_unused]] const CompositorContextData& compositorContextData, [[maybe_unused]] RERHI::RHICommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(nullptr != renderTarget, "The VR hidden area mesh compositor instance pass needs a valid render target")

		#ifdef RENDERER_OPENVR
			if (nullptr != ::detail::g_MeshPtr)
			{
				// Combined scoped profiler CPU and GPU sample as well as renderer debug event command
				RENDERER_SCOPED_PROFILER_EVENT_DYNAMIC(getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer().getContext(), commandBuffer, getCompositorResourcePass().getDebugName())

				// Fill command buffer
				compositorContextData.resetCurrentlyBoundMaterialBlueprintResource();
				::detail::g_MeshPtr->onFillCommandBuffer(commandBuffer);
			}
		#else
			RHI_ASSERT(false, "OpenVR support is disabled")
		#endif
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	CompositorInstancePassVrHiddenAreaMesh::CompositorInstancePassVrHiddenAreaMesh(const CompositorResourcePassVrHiddenAreaMesh& compositorResourcePassVrHiddenAreaMesh, const CompositorNodeInstance& compositorNodeInstance) :
		ICompositorInstancePass(compositorResourcePassVrHiddenAreaMesh, compositorNodeInstance)
	{
		#ifdef RENDERER_OPENVR
			// Add reference to vertex array object (VAO) shared between all compositor instance pass VR hidden area mesh instances
			if (nullptr == ::detail::g_MeshPtr)
			{
				const IRenderer& renderer = getCompositorNodeInstance().getCompositorWorkspaceInstance().getRenderer();
				IVrManager& vrManager = renderer.getVrManager();
				if (vrManager.isRunning() && vrManager.getVrManagerTypeId() == RERenderer::VrManagerOpenVR::TYPE_ID &&
					0 != static_cast<RERenderer::VrManagerOpenVR&>(vrManager).getVrSystem()->GetHiddenAreaMesh(vr::EVREye::Eye_Left).unTriangleCount)
				{
					::detail::g_MeshPtr = new ::detail::Mesh(renderer);
				}
			}
			if (nullptr != ::detail::g_MeshPtr)
			{
				::detail::g_MeshPtr->AddReference();
			}
		#endif
	}

	CompositorInstancePassVrHiddenAreaMesh::~CompositorInstancePassVrHiddenAreaMesh()
	{
		#ifdef RENDERER_OPENVR
			// Release reference to vertex array object (VAO) shared between all compositor instance pass VR hidden area mesh instances
			if (nullptr != ::detail::g_MeshPtr && 1 == ::detail::g_MeshPtr->Release())	// +1 for reference to global shared pointer
			{
				::detail::g_MeshPtr = nullptr;
			}
		#endif
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
