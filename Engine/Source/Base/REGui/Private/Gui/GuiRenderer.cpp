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
#include "REGui/Gui/GuiRenderer.h"
#include <RECore/Log/Log.h>
#include "REGui/Application/GuiContext.h"
#include "REGui/Gui/Keys.h"
#include "REGui/Gui/Gui.h"
#include "REGui/Gui/Resource/GuiRendererResources.h"
#include "REGui/Gui/Resource/GuiTextureManager.h"
#include "REGui/Widget/Window/MainWindow.h"
#include <RECore/Asset/Asset.h>
#include <RECore/Log/Log.h>
#include <RECore/Memory/Memory.h>
#include <RERenderer/Resource/Texture/TextureResourceManager.h>
#if defined(LINUX)
#include "REGui/Backend/Linux/GuiLinux.h"
#include <RERHI/Linux/X11Context.h>
#endif
#include <RERenderer/RendererImpl.h>
#include <imgui.h>
#include <imgui_internal.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
#include <glm/detail/setup.hpp>	// For "GLM_COUNTOF()"
PRAGMA_WARNING_POP

#if defined(LINUX)
#include <sys/time.h>
#endif


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
namespace detail
{


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
static const RECore::AssetId IMGUI_GLYPH_MAP_2D(ASSET_ID("RacoonEngine/Texture/DynamicByCode/ImGuiGlyphMap2D"));

// Vertex input layout
static constexpr RERHI::VertexAttribute VertexAttributesLayout[] =
  {
    { // Attribute 0
      // Data destination
      RERHI::VertexAttributeFormat::FLOAT_4,		// vertexAttributeFormat (RERHI::VertexAttributeFormat)
      "Position",									// name[32] (char)
      "POSITION",									// semanticName[32] (char)
      0,											// semanticIndex (RECore::uint32)
      // Data source
      0,											// inputSlot (RECore::uint32)
      0,											// alignedByteOffset (RECore::uint32)
      sizeof(float) * 4 + sizeof(RECore::uint8) * 4,	// strideInBytes (RECore::uint32)
      0											// instancesPerElement (RECore::uint32)
    },
    { // Attribute 1
      // Data destination
      RERHI::VertexAttributeFormat::R8G8B8A8_UNORM,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
      "Color",									// name[32] (char)
      "COLOR",									// semanticName[32] (char)
      0,											// semanticIndex (RECore::uint32)
      // Data source
      0,											// inputSlot (RECore::uint32)
      sizeof(float) * 4,							// alignedByteOffset (RECore::uint32)
      sizeof(float) * 4 + sizeof(RECore::uint8) * 4,	// strideInBytes (RECore::uint32)
      0											// instancesPerElement (RECore::uint32)
    }
  };
const RERHI::VertexAttributes VertexAttributes(static_cast<RECore::uint32>(GLM_COUNTOF(VertexAttributesLayout)), VertexAttributesLayout);


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
[[nodiscard]] void* AllocFunc(size_t sz, void* user_data)
{
  return RECore::Memory::reallocate(nullptr, 0, sz, 1);
}

void FreeFunc(void* ptr, void* user_data)
{
  RECore::Memory::reallocate(ptr, 0, 0, 1);
}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiRenderer::GuiRenderer(Gui *gui)
: mGui(gui)
, mRhiContext(nullptr)
, mRendererContext(nullptr)
, mRenderer(nullptr)
, mGuiRendererResources(nullptr)
, mGuiTextureManager(nullptr)
, mObjectSpaceToClipSpaceMatrixUniformHandle(NULL_HANDLE)
, mNumberOfAllocatedVertices(0)
, mNumberOfAllocatedIndices(0)
, mTime(0) {

}

GuiRenderer::~GuiRenderer() {
  delete mGuiRendererResources;
  delete mGuiTextureManager;
  delete mRenderer;
  delete mRendererContext;
  delete mRhiContext;
}


void GuiRenderer::startup(RECore::handle windowHandle) {
  // Initialize everything else from importance
#if defined(LINUX)
  this->mRhiContext = new RERHI::X11Context(
    reinterpret_cast<REGui::GuiLinux *>(mGui->getImpl())->getDisplay(),
    windowHandle);
#endif

  typedef RERHI::RHIDynamicRHI *(*RHI_INSTANCER)(const RERHI::RHIContext &);
  RHI_INSTANCER _Creator = reinterpret_cast<RHI_INSTANCER>(mGui->getGuiContext().getRhiLibrary().getSymbol("createRhiInstance"));
  if (_Creator) {
    this->mRhi = _Creator(*this->mRhiContext);

    if (this->mRhi) {
      this->mRhi->AddReference();
      RE_LOG(Info, "Successfully created the RHI backend of " + mGui->getGuiContext().getRhiLibraryName())
    } else {
      // Error
      RE_LOG(Error, "Creation of dynamic RHI failed.")
    }
  } else {
    // Error
    RE_LOG(Error, "Unable to find the symbol 'createRHIInstance'")
  }

  // Create renderer instance
  {
    mRendererContext = new RERenderer::Context(*mRhi, mGui->getCoreContext());
    mRenderer = new RERenderer::RendererImpl(*mRendererContext);
    mRenderer->loadPipelineStateObjectCache();
    RE_LOG(Info,"Created renderer context")
  }

  // Create and initialize ImGui
  {
    initializeKeyMap();
  }

  // TODO(naetherm): Create default font texture
  {
    unsigned char* pixels = nullptr;
    int width = 0;
    int height = 0;
    //this->embraceTheDarkness();
    //ImGui::GetIO().Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload texture to RHI
    mDefaultFontTexture = mRenderer->getTextureManager().createTexture2D(static_cast<RECore::uint32>(width), static_cast<RECore::uint32>(height), RERHI::TextureFormat::R8G8B8A8, pixels, RERHI::TextureFlag::GENERATE_MIPMAPS | RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME("Debug 2D GUI glyph texture atlas"));

    // Tell the texture resource manager about our glyph texture so it can be referenced inside e.g. compositor nodes
    mRenderer->getTextureResourceManager().createTextureResourceByAssetId(::detail::IMGUI_GLYPH_MAP_2D, *mDefaultFontTexture);
  }

  // Initialize additional resource handling
  createFixedBuildInRhiConfigurationResources();
  {
    // Create gui texture manager
    mGuiTextureManager = new GuiTextureManager(this);

    // Create gui resource manager
    mGuiRendererResources = new GuiRendererResources(this);
  }
}

void GuiRenderer::update() {
  mRenderer->update();
}



RERHI::RHIContext* GuiRenderer::getRhiContext() const {
  return mRhiContext;
}

RERHI::RHIDynamicRHI* GuiRenderer::getRhi() const {
  return mRhi;
}

RERenderer::Context* GuiRenderer::getRendererContext() const {
  return mRendererContext;
}

RERenderer::IRenderer* GuiRenderer::getRenderer() const {
  return mRenderer;
}

GuiRendererResources* GuiRenderer::getGuiRendererResources() const {
  return mGuiRendererResources;
}

GuiTextureManager* GuiRenderer::getGuiTextureManager() const {
  return mGuiTextureManager;
}


void GuiRenderer::createFixedBuildInRhiConfigurationResources() {
  RERHI::RHIDynamicRHI* rhi = getRhi();

  { // Create the root signature instance
    // Create the root signature
    RERHI::DescriptorRangeBuilder ranges[4];
    ranges[0].initialize(RERHI::ResourceType::UNIFORM_BUFFER, 0, "UniformBlockDynamicVs", RERHI::ShaderVisibility::VERTEX);
    ranges[1].initialize(RERHI::ResourceType::TEXTURE_2D,		0, "GlyphMap",				RERHI::ShaderVisibility::FRAGMENT);
    ranges[2].initializeSampler(0, RERHI::ShaderVisibility::FRAGMENT);

    RERHI::RootParameterBuilder rootParameters[2];
    rootParameters[0].initializeAsDescriptorTable(2, &ranges[0]);
    rootParameters[1].initializeAsDescriptorTable(1, &ranges[2]);

    // Setup
    RERHI::RootSignatureBuilder rootSignatureBuilder;
    rootSignatureBuilder.initialize(static_cast<uint32_t>(GLM_COUNTOF(rootParameters)), rootParameters, 0, nullptr, RERHI::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Create the instance
    mRootSignature = rhi->createRootSignature(rootSignatureBuilder RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
  }

  { // Create the graphics pipeline state instance
    { // Create the graphics program
      // Get the shader source code (outsourced to keep an overview)
      const char* vertexShaderSourceCode = nullptr;
      const char* fragmentShaderSourceCode = nullptr;
//#include "REGui/Gui/Shader/Gui_GLSL_450.h"	// For Vulkan
#include "REGui/Gui/Shader/Gui_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
//#include "REGui/Gui/Shader/Gui_GLSL_ES3.h"
//#include "REGui/Gui/Shader/Gui_HLSL_D3D9.h"
//#include "REGui/Gui/Shader/Gui_HLSL_D3DXX.h"
#include "REGui/Gui/Shader/Gui_Null.h"

      // Create the graphics program
      RERHI::RHIShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
      mGraphicsProgram = shaderLanguage.createGraphicsProgram(
        *mRootSignature,
        ::detail::VertexAttributes,
        shaderLanguage.createVertexShaderFromSourceCode(::detail::VertexAttributes, vertexShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("GUI")),
        shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode, nullptr RHI_RESOURCE_DEBUG_NAME("GUI"))
        RHI_RESOURCE_DEBUG_NAME("GUI"));
    }

    // Create the graphics pipeline state object (PSO)
    if (nullptr != mGraphicsProgram)
    {
      // TODO(naetherm) Render pass related update, the render pass in here is currently just a dummy so the debug compositor works
      RERHI::RHIRenderPass* renderPass = rhi->createRenderPass(
        1,
        &rhi->getCapabilities().preferredSwapChainColorTextureFormat,
        rhi->getCapabilities().preferredSwapChainDepthStencilTextureFormat,
        1 RHI_RESOURCE_DEBUG_NAME("GUI"));

      RERHI::GraphicsPipelineState graphicsPipelineState = RERHI::GraphicsPipelineStateBuilder(
        mRootSignature,
        mGraphicsProgram,
        ::detail::VertexAttributes,
        *renderPass);
      graphicsPipelineState.rasterizerState.cullMode				 = RERHI::CullMode::NONE;
      graphicsPipelineState.rasterizerState.scissorEnable			 = 1;
      graphicsPipelineState.depthStencilState.depthEnable			 = false;
      graphicsPipelineState.depthStencilState.depthWriteMask		 = RERHI::DepthWriteMask::ZERO;
      graphicsPipelineState.blendState.renderTarget[0].blendEnable = true;
      graphicsPipelineState.blendState.renderTarget[0].srcBlend	 = RERHI::Blend::SRC_ALPHA;
      graphicsPipelineState.blendState.renderTarget[0].destBlend	 = RERHI::Blend::INV_SRC_ALPHA;
      mGraphicsPipelineState = rhi->createGraphicsPipelineState(graphicsPipelineState RHI_RESOURCE_DEBUG_NAME("GUI"));
    }
  }

  {
    // Create vertex uniform buffer instance
    if (rhi->getCapabilities().maximumUniformBufferSize > 0)
    {
      mVertexShaderUniformBuffer = getRenderer()->getBufferManager().createUniformBuffer(
        sizeof(float) * 4 * 4,
        nullptr,
        RERHI::BufferUsage::DYNAMIC_DRAW RHI_RESOURCE_DEBUG_NAME("GUI"));
    }
    else if (nullptr != mGraphicsProgram)
    {
      mObjectSpaceToClipSpaceMatrixUniformHandle = mGraphicsProgram->getUniformHandle("ObjectSpaceToClipSpaceMatrix");
    }

    // Create sampler state instance and wrap it into a resource group instance
    RERHI::RHIResource* samplerStateResource = nullptr;
    {
      RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
      samplerState.addressU = RERHI::TextureAddressMode::WRAP;
      samplerState.addressV = RERHI::TextureAddressMode::WRAP;
      samplerStateResource = rhi->createSamplerState(samplerState RHI_RESOURCE_DEBUG_NAME("GUI"));
      mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource, nullptr RHI_RESOURCE_DEBUG_NAME("GUI"));
    }

    { // Create resource group
      RERHI::RHIResource *resources[2] = {mVertexShaderUniformBuffer, mDefaultFontTexture};
      RERHI::RHISamplerState *samplerStates[2] = {nullptr, static_cast<RERHI::RHISamplerState *>(samplerStateResource)};
      mResourceGroup = mRootSignature->createResourceGroup(
        0,
        static_cast<uint32_t>(GLM_COUNTOF(resources)),
        resources,
        samplerStates RHI_RESOURCE_DEBUG_NAME("GUI"));
      this->mTextures.emplace(mDefaultFontTexture, mResourceGroup);
    }
  }
}

void GuiRenderer::startFrame(MainWindow* mainWindow) {
  {
    ImGuiIO& imGuiIo = ImGui::GetIO();

    { // Setup display size (every frame to accommodate for render target resizing)
      RECore::uint32 width = 0;
      RECore::uint32 height = 0;
      mainWindow->getSwapChain()->getWidthAndHeight(width, height);
      imGuiIo.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

    { // Setup time step
      timeval currentTimeValue;
      gettimeofday(&currentTimeValue, nullptr);
      const RECore::uint64 currentTime = currentTimeValue.tv_sec * 1000000 + currentTimeValue.tv_usec;
      imGuiIo.DeltaTime = static_cast<float>(currentTime - mTime) / 1000000.0f;
      mTime = currentTime;
    }
  }

  ImGui::NewFrame();
}

void GuiRenderer::endFrame() {

}

const RERHI::RHIVertexArrayPtr& GuiRenderer::getFillVertexArrayPtr(RERHI::RHICommandBuffer* commandBuffer)
{
  if (GImGui->Initialized)
  {
    // Ask ImGui to render into the internal command buffer and then request the resulting draw data
    ImGui::Render();
    const ImDrawData* imDrawData = ImGui::GetDrawData();
    RERHI::RHIDynamicRHI& rhi = mRenderer->getRhi();
    RERHI::RHIBufferManager& bufferManager = mRenderer->getBufferManager();

    { // Vertex and index buffers
      // Create and grow vertex/index buffers if needed
      if (nullptr == mVertexBuffer || mNumberOfAllocatedVertices < static_cast<RECore::uint32>(imDrawData->TotalVtxCount))
      {
        mNumberOfAllocatedVertices = static_cast<RECore::uint32>(imDrawData->TotalVtxCount + 5000);	// Add some reserve to reduce reallocations
        mVertexBuffer = bufferManager.createVertexBuffer(mNumberOfAllocatedVertices * sizeof(ImDrawVert), nullptr, 0, RERHI::BufferUsage::DYNAMIC_DRAW RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        mVertexArray = nullptr;
      }
      if (nullptr == mIndexBuffer || mNumberOfAllocatedIndices < static_cast<RECore::uint32>(imDrawData->TotalIdxCount))
      {
        mNumberOfAllocatedIndices = static_cast<RECore::uint32>(imDrawData->TotalIdxCount + 10000);	// Add some reserve to reduce reallocations
        mIndexBuffer = bufferManager.createIndexBuffer(mNumberOfAllocatedIndices * sizeof(ImDrawIdx), nullptr, 0, RERHI::BufferUsage::DYNAMIC_DRAW, RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
        mVertexArray = nullptr;
      }
      if (nullptr == mVertexArray)
      {
        RHI_ASSERT(nullptr != mVertexBuffer, "Invalid vertex buffer")
        RHI_ASSERT(nullptr != mIndexBuffer, "Invalid index buffer")

        // Create vertex array object (VAO)
        const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { mVertexBuffer };
        mVertexArray = bufferManager.createVertexArray(::detail::VertexAttributes, static_cast<RECore::uint32>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers, mIndexBuffer RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      }

      { // Copy and convert all vertices and indices into a single contiguous buffer
        RERHI::MappedSubresource vertexBufferMappedSubresource;
        if (rhi.map(*mVertexBuffer, 0, RERHI::MapType::WRITE_DISCARD, 0, vertexBufferMappedSubresource))
        {
          RERHI::MappedSubresource indexBufferMappedSubresource;
          if (rhi.map(*mIndexBuffer, 0, RERHI::MapType::WRITE_DISCARD, 0, indexBufferMappedSubresource))
          {
            ImDrawVert* imDrawVert = static_cast<ImDrawVert*>(vertexBufferMappedSubresource.data);
            ImDrawIdx* imDrawIdx = static_cast<ImDrawIdx*>(indexBufferMappedSubresource.data);
            for (int i = 0; i < imDrawData->CmdListsCount; ++i)
            {
              const ImDrawList* imDrawList = imDrawData->CmdLists[i];
              memcpy(imDrawVert, &imDrawList->VtxBuffer[0], imDrawList->VtxBuffer.size() * sizeof(ImDrawVert));
              memcpy(imDrawIdx, &imDrawList->IdxBuffer[0], imDrawList->IdxBuffer.size() * sizeof(ImDrawIdx));
              imDrawVert += imDrawList->VtxBuffer.size();
              imDrawIdx += imDrawList->IdxBuffer.size();
            }

            // Unmap the index buffer
            rhi.unmap(*mIndexBuffer, 0);
          }

          // Unmap the vertex buffer
          rhi.unmap(*mVertexBuffer, 0);
        }
      }
    }
  }

  // Done
  return mVertexArray;
}

void GuiRenderer::fillGraphicsCommandBuffer(RERHI::RHICommandBuffer& commandBuffer)
{
  if (GImGui->Initialized)
  {
    // No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
    // RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Debug GUI")

    // Render command lists
    // -> There's no need to try to gather draw calls and batch them into multi-draw-indirect buffers, ImGui does already a pretty good job
    int vertexOffset = 0;
    int indexOffset = 0;
    const ImDrawData* imDrawData = ImGui::GetDrawData();
    for (int commandListIndex = 0; commandListIndex < imDrawData->CmdListsCount; ++commandListIndex)
    {
      const ImDrawList* imDrawList = imDrawData->CmdLists[commandListIndex];
      for (int commandIndex = 0; commandIndex < imDrawList->CmdBuffer.size(); ++commandIndex)
      {
        const ImDrawCmd* pcmd = &imDrawList->CmdBuffer[commandIndex];
        if (nullptr != pcmd->UserCallback)
        {
          RE_LOG(Info, "Callback method")
          pcmd->UserCallback(imDrawList, pcmd);
        }
        else
        {
          if (pcmd->TextureId) {
            { // Setup orthographic projection matrix into our vertex shader uniform buffer
              const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
              ImVec2 texelOffset(0.0f, 0.0f);
              if (mRenderer->getRhi().getNameId() == RERHI::NameId::DIRECT3D9)
              {
                // Take care of the Direct3D 9 half-pixel/half-texel offset
                // -> See "Directly Mapping Texels to Pixels (Direct3D 9)" at https://docs.microsoft.com/en-gb/windows/win32/direct3d9/directly-mapping-texels-to-pixels?redirectedfrom=MSDN
                texelOffset.x += 0.5f;
                texelOffset.y += 0.5f;
              }
              const float l = texelOffset.x;
              const float r = displaySize.x + texelOffset.x;
              const float t = texelOffset.y;
              const float b = displaySize.y + texelOffset.y;
              const float objectSpaceToClipSpaceMatrix[4][4] =
                {
                  { 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
                  { 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
                  { 0.0f,				 0.0f,        0.5f, 0.0f },
                  { (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
                };
              if (nullptr != mVertexShaderUniformBuffer)
              {
                // Copy data into the uniform buffer
                RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
              }
              else
              {
                // Set legacy uniforms
                RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
              }
            }
            RERHI::RHITexture2D *tex = reinterpret_cast<RERHI::RHITexture2D *>(pcmd->TextureId);

            //RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

            // Set the used graphics pipeline state object (PSO)
            //RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

            // Set graphics resource groups
            RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->getResourceGroupByTexture(tex));
            RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
            //RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, tex);
          } else {

            //RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

            // Set the used graphics pipeline state object (PSO)
            //RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

            // Set graphics resource groups
            RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, this->getResourceGroupByTexture(mDefaultFontTexture));
            RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
          }

          // Set graphics scissor rectangle
          RERHI::Command::SetGraphicsScissorRectangles::create(commandBuffer, static_cast<long>(pcmd->ClipRect.x), static_cast<long>(pcmd->ClipRect.y), static_cast<long>(pcmd->ClipRect.z), static_cast<long>(pcmd->ClipRect.w));

          // Draw graphics
          RERHI::Command::DrawIndexedGraphics::create(commandBuffer, static_cast<RECore::uint32>(pcmd->ElemCount), 1, static_cast<RECore::uint32>(indexOffset), static_cast<RECore::int32>(vertexOffset));
        }
        indexOffset += pcmd->ElemCount;
      }
      vertexOffset += imDrawList->VtxBuffer.size();
    }
  }
}

void GuiRenderer::fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(RERHI::RHICommandBuffer& commandBuffer, RERHI::RHITexture2D* tex)
{
  if (GImGui->Initialized)
  {
    // No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
    // RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Fixed debug GUI")

    // Create fixed build in RHI configuration resources, if required
    if (nullptr == mRootSignature)
    {
      createFixedBuildInRhiConfigurationResources();
    }

    { // Setup orthographic projection matrix into our vertex shader uniform buffer
      const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
      ImVec2 texelOffset(0.0f, 0.0f);
      if (mRenderer->getRhi().getNameId() == RERHI::NameId::DIRECT3D9)
      {
        // Take care of the Direct3D 9 half-pixel/half-texel offset
        // -> See "Directly Mapping Texels to Pixels (Direct3D 9)" at https://docs.microsoft.com/en-gb/windows/win32/direct3d9/directly-mapping-texels-to-pixels?redirectedfrom=MSDN
        texelOffset.x += 0.5f;
        texelOffset.y += 0.5f;
      }
      const float l = texelOffset.x;
      const float r = displaySize.x + texelOffset.x;
      const float t = texelOffset.y;
      const float b = displaySize.y + texelOffset.y;
      const float objectSpaceToClipSpaceMatrix[4][4] =
        {
          { 2.0f / (r - l),	 0.0f,        0.0f, 0.0f },
          { 0.0f,				 2.0f/(t-b),  0.0f, 0.0f },
          { 0.0f,				 0.0f,        0.5f, 0.0f },
          { (r + l) / (l - r), (t+b)/(b-t), 0.5f, 1.0f }
        };
      if (nullptr != mVertexShaderUniformBuffer)
      {
        // Copy data into the uniform buffer
        RERHI::Command::CopyUniformBufferData::create(commandBuffer, *mVertexShaderUniformBuffer, objectSpaceToClipSpaceMatrix, sizeof(objectSpaceToClipSpaceMatrix));
      }
      else
      {
        // Set legacy uniforms
        RERHI::Command::SetUniform::createMatrix4fv(commandBuffer, *mGraphicsProgram, mObjectSpaceToClipSpaceMatrixUniformHandle, &objectSpaceToClipSpaceMatrix[0][0]);
      }
    }

    { // RHI configuration
      // Set the used graphics root signature
      RERHI::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

      // Set the used graphics pipeline state object (PSO)
      RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

      // Set graphics resource groups
      RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 0, mResourceGroup);
      RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, 1, mSamplerStateGroup);
    }

    // Setup input assembly (IA): Set the used vertex array
    RERHI::Command::SetGraphicsVertexArray::create(commandBuffer, getFillVertexArrayPtr(&commandBuffer));

    // Render command lists
    fillGraphicsCommandBuffer(commandBuffer);
  }
}

void GuiRenderer::registerTexture(RERHI::RHITexture2D *texture) {

  if (nullptr != mRootSignature)
  {
    //createFixedBuildInRhiConfigurationResources();
    if (this->mTextures.find(texture) == this->mTextures.end()) {
      RE_LOG(Info, "Creating mapping")
      RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
      samplerState.addressU = RERHI::TextureAddressMode::WRAP;
      samplerState.addressV = RERHI::TextureAddressMode::WRAP;
      RERHI::RHIResource* samplerStateResource = this->mRenderer->getRhi().createSamplerState(samplerState RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      RERHI::RHIResource *resources[2] = {mVertexShaderUniformBuffer, texture};
      RERHI::RHISamplerState *samplerStates[2] = {nullptr, static_cast<RERHI::RHISamplerState *>(samplerStateResource)};
      RERHI::RHIResourceGroup* resourceGroup = mRootSignature->createResourceGroup(0, static_cast<RECore::uint32>(GLM_COUNTOF(resources)), resources, samplerStates RHI_RESOURCE_DEBUG_NAME("Debug GUI"));
      resourceGroup->AddReference();
      // Add to map
      this->mTextures.emplace(texture, resourceGroup);
    }

  }
}

void GuiRenderer::initializeKeyMap() {
  ImGuiIO& imGuiIo = ImGui::GetIO();

  imGuiIo.KeyMap[ImGuiKey_Tab]		= (XK_Tab & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_LeftArrow]	= (XK_Left & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_RightArrow]	= (XK_Right & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_UpArrow]	= (XK_Up & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_DownArrow]	= (XK_Down & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_PageUp]		= (XK_Page_Up & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_PageDown]	= (XK_Page_Down & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Home]		= (XK_Home & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_End]		= (XK_End & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Insert]		= (XK_Insert & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Delete]		= (XK_Delete & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Backspace]	= (XK_BackSpace & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Space]		= (XK_space & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Enter]		= (XK_Return & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Escape]		= (XK_Escape & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_A]			= REGUIKEY_A;
  imGuiIo.KeyMap[ImGuiKey_C]			= REGUIKEY_C;
  imGuiIo.KeyMap[ImGuiKey_V]			= REGUIKEY_V;
  imGuiIo.KeyMap[ImGuiKey_X]			= REGUIKEY_X;
  imGuiIo.KeyMap[ImGuiKey_Y]			= REGUIKEY_Y;
  imGuiIo.KeyMap[ImGuiKey_Z]			= REGUIKEY_Z;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
