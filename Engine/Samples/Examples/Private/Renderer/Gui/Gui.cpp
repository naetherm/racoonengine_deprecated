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


#ifndef RENDERER_IMGUI
	#error "This file only works when the renderer is build with ImGui support enabled"
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Examples/Renderer/Gui/Gui.h"
#include "Examples/Renderer/Gui/SceneView.h"
#include <RECore/Color/Color4.h>
#include "Examples/ExampleRunner.h"

#include <RERenderer/Context.h>
#include <RERenderer/IRenderer.h>
#include <RECore/File/IFile.h>
#include <RECore/File/IFileManager.h>
#include <RERenderer/DebugGui/DebugGuiManager.h>
#ifdef RENDERER_OPENVR
	#include <RERenderer/Vr/IVrManager.h>
#endif
#include <RERenderer/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.h>
#include <RERenderer/Resource/Scene/SceneNode.h>

// "ini.h"-library
#define INI_MALLOC(ctx, size) (static_cast<RECore::IAllocator*>(ctx)->reallocate(nullptr, 0, size, 1))
#define INI_FREE(ctx, ptr) (static_cast<RECore::IAllocator*>(ctx)->reallocate(ptr, 0, 0, 1))
#include <Examples/ini.h>

#include <imgui.h>
#include <imgui_internal.h>
#include "Examples/Renderer/Gui/ImGuizmo.h"
#include <algorithm>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace ImGuiExampleSelectorDetail
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		static constexpr char* VIRTUAL_SETTINGS_FILENAME = "LocalData/ImGuiExampleSelectorExample.ini";


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Gui::Gui() :
	// Ini settings indices
	mIni(nullptr),
	mSelectedRhiNameIndex(INI_NOT_FOUND),
	mSelectedExampleNameIndex(INI_NOT_FOUND),
  mShowDemoWindow(true),
  mSceneView(new SceneView()),
  mSceneViewOnFocus(false)
{
	// Nothing here
  mSceneView->mExampleRunner = this->mExampleRunner;
}

Gui::~Gui() {
  delete mSceneView;
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Gui::onInitialization()
{
  mSceneView->mApplicationFrontend = this->mApplicationFrontend;
  mSceneView->onInitialization();
  RERHI::RHIDynamicRHIPtr rhi(getRhi());
  if (nullptr != rhi)
  {
    // Create the buffer and texture manager
    mBufferManager = rhi->createBufferManager();
    mTextureManager = rhi->createTextureManager();

    { // Create the root signature
      /*
      RERHI::DescriptorRangeBuilder ranges[2];
      ranges[0].initialize(RERHI::ResourceType::TEXTURE_2D, 0, "AlbedoMap", RERHI::ShaderVisibility::FRAGMENT);
      ranges[1].initializeSampler(0, RERHI::ShaderVisibility::FRAGMENT);

      RERHI::RootParameterBuilder rootParameters[2];
      rootParameters[0].initializeAsDescriptorTable(1, &ranges[0]);
      rootParameters[1].initializeAsDescriptorTable(1, &ranges[1]);
*/
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
      mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
    }

    // Create sampler state and wrap it into a resource group instance
    RERHI::RHIResource* samplerStateResource = nullptr;
    {
      RERHI::SamplerState samplerState = RERHI::RHISamplerState::getDefaultSamplerState();
      samplerState.maxLod = 0.0f;
      samplerState.addressU = RERHI::TextureAddressMode::WRAP;
      samplerState.addressV = RERHI::TextureAddressMode::WRAP;
      samplerStateResource = rhi->createSamplerState(samplerState);
      mSamplerStateGroup = mRootSignature->createResourceGroup(1, 1, &samplerStateResource);
    }

    { // Texture resource related
      // Create the texture instance, but without providing texture data (we use the texture as render target)
      // -> Use the "RERHI::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
      // -> Required for Vulkan, Direct3D 9, Direct3D 10, Direct3D 11 and Direct3D 12
      // -> Not required for OpenGL and OpenGL ES 3
      // -> The optimized texture clear value is a Direct3D 12 related option
      const RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::Enum::R8G8B8A8;
      { // Create the 2D texture
        static constexpr uint32_t TEXTURE_WIDTH = 64;
        static constexpr uint32_t TEXTURE_HEIGHT = 64;
        static constexpr uint32_t TEXEL_ELEMENTS = 4;
        static constexpr uint32_t NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXEL_ELEMENTS;
        uint8_t data[NUMBER_OF_BYTES];

        { // Fill the texture data with a defective checkboard
          const uint32_t rowPitch = TEXTURE_WIDTH * TEXEL_ELEMENTS;
          const uint32_t cellPitch = rowPitch >> 3;    // The width of a cell in the checkboard texture
          const uint32_t cellHeight = TEXTURE_WIDTH >> 3;  // The height of a cell in the checkerboard texture
          for (uint32_t n = 0; n < NUMBER_OF_BYTES; n += TEXEL_ELEMENTS) {
            const uint32_t x = n % rowPitch;
            const uint32_t y = n / rowPitch;
            const uint32_t i = x / cellPitch;
            const uint32_t j = y / cellHeight;

            if (i % 2 == j % 2) {
              // Black
              data[n + 0] = 0;  // R
              data[n + 1] = 0;  // G
              data[n + 2] = 0;  // B
              data[n + 3] = 255;  // A
            } else {
              // Add some color fun instead of just boring white
              data[n + 0] = static_cast<uint8_t>(rand() % 255);  // R
              data[n + 1] = static_cast<uint8_t>(rand() % 255);  // G
              data[n + 2] = static_cast<uint8_t>(rand() % 255);  // B
              data[n + 3] = static_cast<uint8_t>(rand() % 255);  // A
            }
          }
        }

        // Create the texture instance
        mTexture2DPtr = mTextureManager->createTexture2D(TEXTURE_WIDTH, TEXTURE_HEIGHT, RERHI::TextureFormat::R8G8B8A8,
                                                         data, RERHI::TextureFlag::GENERATE_MIPMAPS |
                                                               RERHI::TextureFlag::SHADER_RESOURCE);
        //RERenderer::DebugGuiManager& debugGuiManager = getRendererSafe().getDebugGuiManager();
        //debugGuiManager.registerTexture(mTexture2DPtr);
      }
        //mTexture2DPtr = mTextureManager->createTexture2D(200, 200, textureFormat, nullptr, RERHI::TextureFlag::SHADER_RESOURCE | RERHI::TextureFlag::RENDER_TARGET, RERHI::TextureUsage::DEFAULT, 1, reinterpret_cast<const RERHI::OptimizedTextureClearValue*>(&RECore::Color4::GREEN));

      { // Create texture group
        RERHI::RHIResource* resource[2] = { mBufferManager->createUniformBuffer(sizeof(float) * 4 * 4, nullptr, RERHI::BufferUsage::DYNAMIC_DRAW RHI_RESOURCE_DEBUG_NAME("Debug GUI")), mTexture2DPtr };
        RERHI::RHISamplerState* samplerState[2] = { nullptr, static_cast<RERHI::RHISamplerState*>(samplerStateResource) };
        mTextureGroup = mRootSignature->createResourceGroup(0, 2, resource, samplerState);
      }

      { // Create the framebuffer object (FBO) instance
        const RERHI::FramebufferAttachment colorFramebufferAttachment(mTexture2DPtr);
        mFramebuffer = rhi->createFramebuffer(*rhi->createRenderPass(1, &textureFormat), &colorFramebufferAttachment);
      }
    }

    // Vertex input layout
    static constexpr RERHI::VertexAttribute vertexAttributesLayout[] =
      {
        { // Attribute 0
          // Data destination
          RERHI::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
          "Position",								// name[32] (char)
          "POSITION",								// semanticName[32] (char)
          0,										// semanticIndex (uint32_t)
          // Data source
          0,										// inputSlot (uint32_t)
          0,										// alignedByteOffset (uint32_t)
          sizeof(float) * 2,						// strideInBytes (uint32_t)
          0										// instancesPerElement (uint32_t)
        }
      };
    const RERHI::VertexAttributes vertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

    { // Create vertex array object (VAO)
      // Create the vertex buffer object (VBO)
      // -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
      static constexpr float VERTEX_POSITION[] =
        {					// Vertex ID	Triangle on screen
          -1.0f, 1.0f,	// 0				0
          1.0f, 1.0f,
          1.0f, -1.0f,	// 1			   .   .
          -1.0f, 1.0f,	// 0				0
          1.0f, -1.0f,	// 1			   .   .
          -1.0f, -1.0f		// 2			  2.......1
        };
      RERHI::RHIVertexBufferPtr vertexBuffer(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION));

      // Create vertex array object (VAO)
      // -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
      // -> This means that there's no need to keep an own vertex buffer object (VBO) reference
      // -> When the vertex array object (VAO) is destroyed, it automatically decreases the
      //    reference of the used vertex buffer objects (VBO). If the reference counter of a
      //    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
      const RERHI::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
      mVertexArray = mBufferManager->createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers);
    }

    {
      // Create the graphics program
      RERHI::RHIGraphicsProgramPtr graphicsProgram;
      {
        // Get the shader source code (outsourced to keep an overview)
        const char* vertexShaderSourceCode = nullptr;
        const char* fragmentShaderSourceCode = nullptr;
#include "Examples/Basics/RenderToTexture/RenderToTexture_GLSL_450.h"	// For Vulkan
#include "Examples/Basics/RenderToTexture/RenderToTexture_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "Examples/Basics/RenderToTexture/RenderToTexture_GLSL_ES3.h"
#include "Examples/Basics/RenderToTexture/RenderToTexture_HLSL_D3D9.h"
#include "Examples/Basics/RenderToTexture/RenderToTexture_HLSL_D3D10_D3D11_D3D12.h"
#include "Examples/Basics/RenderToTexture/RenderToTexture_Null.h"

        // Create the graphics program
        RERHI::RHIShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
        graphicsProgram = shaderLanguage.createGraphicsProgram(
          *mRootSignature,
          vertexAttributes,
          shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
          shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
      }

      // Create the graphics pipeline state object (PSO)
      if (nullptr != graphicsProgram)
      {
        mGraphicsPipelineState = rhi->createGraphicsPipelineState(RERHI::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass()));
      }
    }

    // Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
    fillCommandBuffer();
  }

	loadIni();


  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Ease-of-use: If a HMD is present automatically start the "Scene"-example so the user can see something
	#ifdef RENDERER_OPENVR
  	if (getRendererSafe().getVrManager().isHmdPresent())
  	{
  		switchExample("Scene");
  	}
  #endif
}

void Gui::onDeinitialization()
{
	saveIni();
	destroyIni();
  this->mSceneView->onDeinitialization();
}

void Gui::onDraw(RERHI::RHICommandBuffer& commandBuffer)
{
  RERenderer::DebugGuiManager& debugGuiManager = getRendererSafe().getDebugGuiManager();

  {
    //RERHI::Command::ClearGraphics::create(mFBCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);
    mSceneView->onUpdate();
    debugGuiManager.registerTexture(mSceneView->getTexture2D());
    mSceneView->onDraw(mFBCommandBuffer);
    //debugGuiManager.newFrame(*mSceneView->mFramebuffer);
    //mFBCommandBuffer.dispatchToRhiAndClear(*this->getRhi());
   // mFBCommandBuffer.appendToCommandBuffer(commandBuffer);
    //debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(commandBuffer);
    //debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(mFBCommandBuffer);
  }//ImGui::EndFrame();
  //mCommandBuffer.dispatchToRhiAndClear(*this->getRhi());
  //debugGuiManager.fillGraphicsCommandBuffer(mCommandBuffer);
  //mFBCommandBuffer.appendToCommandBufferAndClear(mCommandBuffer);

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

	// GUI
	if (nullptr != getMainRenderTarget())
	{
    debugGuiManager.newFrame(*getMainRenderTarget());
    ImGuizmo::BeginFrame();
    createDebugGui();
    ImGui::ShowDemoWindow(&mShowDemoWindow);
    createSceneView();
    createAssetBrowser();
		debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(mCommandBuffer);
	}

	// Append command buffer to the given command buffer
	mCommandBuffer.appendToCommandBufferAndClear(commandBuffer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Gui::loadIni()
{
	// Reset ini
	destroyIni();

	// Try to load ini settings from file
	RERenderer::IRenderer& renderer = getRendererSafe();
	const RECore::IFileManager& fileManager = renderer.getFileManager();
	if (fileManager.doesFileExist(::ImGuiExampleSelectorDetail::VIRTUAL_SETTINGS_FILENAME))
	{
		RECore::IFile* file = fileManager.openFile(RECore::IFileManager::FileMode::READ, ::ImGuiExampleSelectorDetail::VIRTUAL_SETTINGS_FILENAME);
		if (nullptr != file)
		{
			mIniFileContent.resize(file->getNumberOfBytes());
			file->read(mIniFileContent.data(), mIniFileContent.size());
			fileManager.closeFile(*file);
			mIni = ini_load(mIniFileContent.data(), nullptr);
			mSelectedRhiNameIndex = ini_find_property(mIni, INI_GLOBAL_SECTION, "SelectedRhiName", 0);
			mSelectedExampleNameIndex = ini_find_property(mIni, INI_GLOBAL_SECTION, "SelectedExampleName", 0);
		}
	}
	if (nullptr == mIni)
	{
		mIni = ini_create(nullptr);
	}
}

void Gui::saveIni()
{
	if (nullptr != mIni)
	{
		const RECore::IFileManager& fileManager = getRendererSafe().getFileManager();
		RECore::IFile* file = fileManager.openFile(RECore::IFileManager::FileMode::WRITE, ::ImGuiExampleSelectorDetail::VIRTUAL_SETTINGS_FILENAME);
		if (nullptr != file)
		{
			// Backup settings to ini
			if (INI_NOT_FOUND == mSelectedRhiNameIndex)
			{
				mSelectedRhiNameIndex = ini_property_add(mIni, INI_GLOBAL_SECTION, "SelectedRhiName", 0, mSelectedRhiName.c_str(), 0);
			}
			else
			{
				ini_property_value_set(mIni, INI_GLOBAL_SECTION, mSelectedRhiNameIndex, mSelectedRhiName.c_str(), 0);
			}
			if (INI_NOT_FOUND == mSelectedExampleNameIndex)
			{
				mSelectedExampleNameIndex = ini_property_add(mIni, INI_GLOBAL_SECTION, "SelectedExampleName", 0, mSelectedExampleName.c_str(), 0);
			}
			else
			{
				ini_property_value_set(mIni, INI_GLOBAL_SECTION, mSelectedExampleNameIndex, mSelectedExampleName.c_str(), 0);
			}

			// Save ini
			mIniFileContent.resize(static_cast<size_t>(ini_save(mIni, nullptr, 0)));
			ini_save(mIni, mIniFileContent.data(), static_cast<int>(mIniFileContent.size()));
			file->write(mIniFileContent.data(), mIniFileContent.size());
			fileManager.closeFile(*file);
		}
	}
}

void Gui::destroyIni()
{
	if (nullptr != mIni)
	{
		ini_destroy(mIni);
		mIni = nullptr;
		mSelectedRhiNameIndex = INI_NOT_FOUND;
		mSelectedExampleNameIndex = INI_NOT_FOUND;
	}
}

void Gui::createSceneView() {
  //ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));

  if (ImGui::Begin("SceneView")) {
    ImGuiIO& io = ImGui::GetIO();
    if (mTexture2DPtr) {
      ImTextureID tex_id = mSceneView->getTexture2D().GetPointer();

      ImVec2 vMin = ImGui::GetWindowContentRegionMin();
      ImVec2 vMax = ImGui::GetWindowContentRegionMax();

      vMin.x += ImGui::GetWindowPos().x;
      vMin.y += ImGui::GetWindowPos().y;
      vMax.x += ImGui::GetWindowPos().x;
      vMax.y += ImGui::GetWindowPos().y;
      float my_tex_w = vMax.x - vMin.x;
      float my_tex_h = vMax.y - vMin.y;
      ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
      ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
      ImGui::Image(tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
      this->mSceneView->onChangeWidthAndHeight(my_tex_w, my_tex_h);
      if (ImGui::IsItemHovered())
      {
        //RE_LOG(Info, "On Focus")
        mSceneViewOnFocus = true;
      } else {
        mSceneViewOnFocus = false;
      }
      mSceneView->setOnFocus(mSceneViewOnFocus);
    }
    ImVec2 vCurPos = ImGui::GetCurrentWindow()->ContentRegionRect.GetTL();
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;
    float my_tex_w = vMax.x - vMin.x;
    float my_tex_h = vMax.y - vMin.y;
    ImVec2 vCurSize = ImVec2(my_tex_w, my_tex_h);
    ImVec2 vNextPos = vCurPos;
    ImGui::End();
    ImGui::SetNextWindowPos(vNextPos);
    ImGui::SetNextWindowSize(ImVec2(my_tex_w, my_tex_h));
    if (ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground))
    {
      // Status
      static const ImVec4 GREY_COLOR(0.5f, 0.5f, 0.5f, 1.0f);
      static const ImVec4 WHITE_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
      static const ImVec4 RED_COLOR(1.0f, 0.0f, 0.0f, 1.0f);
      ImGui::PushStyleColor(ImGuiCol_Text, WHITE_COLOR);
      ImGui::Text("RHI: %s", this->getMainRenderTarget()->getRhi().getName());
      ImGui::Text("GPU: %s", this->getMainRenderTarget()->getRhi().getCapabilities().deviceName);
      ImGui::PushStyleColor(ImGuiCol_Text, WHITE_COLOR);
      ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
#ifdef RENDERER_TOOLKIT
      { // Renderer toolkit
              const RERendererToolkit::IRendererToolkit* rendererToolkit = getRendererToolkit();
              if (nullptr != rendererToolkit)
              {
                const bool idle = (RERendererToolkit::IRendererToolkit::State::IDLE == rendererToolkit->getState());
                ImGui::PushStyleColor(ImGuiCol_Text, idle ? GREY_COLOR : RED_COLOR);
                  ImGui::Text("Renderer Toolkit: %s", idle ? "Idle" : "Busy");
                ImGui::PopStyleColor();
              }
            }
#endif
      ImGui::End();
    }
    if (this->mSceneView->mShowSkeleton && nullptr != this->mSceneView->mSkeletonMeshSceneItem && nullptr != this->mSceneView->mSkeletonMeshSceneItem->getParentSceneNode()) {

      ImGui::SetNextWindowPos(vCurPos);
      ImGui::SetNextWindowSize(vCurSize);
      RERenderer::DebugGuiHelper::drawSkeleton(*this->mSceneView->mCameraSceneItem, *this->mSceneView->mSkeletonMeshSceneItem, vCurSize, vCurPos);
    }
    ImGui::End();

    if (nullptr != this->mSceneView->mSceneNode)
    {
      // Draw gizmo
      RECore::Transform transform = this->mSceneView->mSceneNode->getGlobalTransform();
      RERenderer::DebugGuiHelper::drawGizmo(*this->mSceneView->mCameraSceneItem, this->mSceneView->mGizmoSettings, transform, vCurSize, vCurPos);
      this->mSceneView->mSceneNode->setTransform(transform);

      // Draw grid
      // TODO(naetherm) Make this optional via GUI
      // RERenderer::DebugGuiHelper::drawGrid(*mCameraSceneItem, transform.position.y);
    }
  }
}

void Gui::createAssetBrowser() {
  if (ImGui::Begin("AssetBrowser")){

    ImGui::PushItemWidth(150);
    if (ImGui::InputTextWithHint("##search", "Search", m_filter, sizeof(m_filter), ImGuiInputTextFlags_EnterReturnsTrue)) {

    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::Separator();

    // Left col
    ImVec2 size(120.f, 0);
    ImGui::BeginChild("left_col", size);
    ImGui::PushItemWidth(120);

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::End();
  }
}

void Gui::createDebugGui()
{

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGui::Begin("MainDockspace", nullptr, flags);
  ImGui::PopStyleVar(3);
  ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
  ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);
  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(260.0f, 100.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Example Selector"))
	{
		// Selection of RHI
		static int selectedRhiIndex = -1;
		int previouslySelectedRhiIndex = selectedRhiIndex;
		if (-1 == selectedRhiIndex && INI_NOT_FOUND != mSelectedRhiNameIndex)
		{
			// Restore the previously selected RHI from ini
			mSelectedRhiName = ini_property_value(mIni, INI_GLOBAL_SECTION, mSelectedRhiNameIndex);
		}
		{ // Fill list of RHI names
			std::string itemsSeparatedByZeros;
			const ExampleRunner::AvailableRhis& availableRhis = getExampleRunner().getAvailableRhis();
			{
				int rhiIndex = 0;
				const std::string& defaultRhiName = mSelectedRhiName.empty() ? getExampleRunner().getDefaultRhiName() : mSelectedRhiName;
				for (const std::string_view& rhiName : availableRhis)
				{
					itemsSeparatedByZeros += rhiName;
					itemsSeparatedByZeros += '\0';
					if (-1 == selectedRhiIndex && defaultRhiName == rhiName)
					{
						// Set initially selected RHI index
						selectedRhiIndex = rhiIndex;
					}
					if (rhiIndex == selectedRhiIndex)
					{
						mSelectedRhiName = rhiName;
					}
					++rhiIndex;
				}
				itemsSeparatedByZeros += '\0';
			}

			// Tell ImGui
			ImGui::Combo("RHI", &selectedRhiIndex, itemsSeparatedByZeros.c_str());
			if (previouslySelectedRhiIndex != selectedRhiIndex)
			{
				// Update the selected RHI name at once
				ExampleRunner::AvailableRhis::const_iterator iterator = availableRhis.cbegin();
				std::advance(iterator, selectedRhiIndex);
				mSelectedRhiName = *iterator;
			}
		}

		{ // Selection of example
			static int selectedExampleIndex = -1;
			if (-1 == selectedExampleIndex && INI_NOT_FOUND != mSelectedExampleNameIndex)
			{
				// Restore the previously selected example from ini
				mSelectedExampleName = ini_property_value(mIni, INI_GLOBAL_SECTION, mSelectedExampleNameIndex);
			}
			else if (previouslySelectedRhiIndex != selectedRhiIndex)
			{
				// When changing the RHI the number of supported examples might change, try to keep the previously selected example selected
				selectedExampleIndex = -1;
			}

			// Fill list of examples supported by the currently selected RHI
			std::string itemsSeparatedByZeros;
			{
				int exampleIndex = 0;
				const std::string& defaultExampleName = mSelectedExampleName.empty() ? "Scene" : mSelectedExampleName;
				const ExampleRunner::ExampleToSupportedRhis& exampleToSupportedRhis = getExampleRunner().getExampleToSupportedRhis();
				for (const auto& pair : exampleToSupportedRhis)
				{
					const ExampleRunner::SupportedRhis& supportedRhiList = pair.second;
					if (pair.first != "ImGuiExampleSelector" && std::find(supportedRhiList.begin(), supportedRhiList.end(), mSelectedRhiName) != supportedRhiList.end())
					{
						itemsSeparatedByZeros += pair.first;
						itemsSeparatedByZeros += '\0';
						if (-1 == selectedExampleIndex && defaultExampleName == pair.first)
						{
							// Set initially selected example index, "Scene" is preferred since it's the most advantaged example
							selectedExampleIndex = exampleIndex;
						}
						if (exampleIndex == selectedExampleIndex)
						{
							mSelectedExampleName = pair.first;
						}
						++exampleIndex;
					}
				}
				itemsSeparatedByZeros += '\0';

				// In case the default example isn't supported by the selected RHI, initially select the first best supported example
				if (-1 == selectedExampleIndex && 0 != exampleIndex)
				{
					selectedExampleIndex = 0;
					mSelectedExampleName = itemsSeparatedByZeros;
				}
			}

			// Tell ImGui
			ImGui::Combo("Example", &selectedExampleIndex, itemsSeparatedByZeros.c_str());
		}

		// Start selected example
		if (ImGui::Button("Start"))
		{
			switchExample(mSelectedExampleName.c_str(), mSelectedRhiName.c_str());
		}

		// Exit
		ImGui::SameLine();
		if (ImGui::Button("Exit"))
		{
			exit();
		}
	}
	ImGui::End();
}



void Gui::fillCommandBuffer()
{
  RE_LOG(Info, "FILL CMD");
  // Sanity checks
  ASSERT(nullptr != getRhi(), "Invalid RHI instance")
  RHI_ASSERT(getRhi()->getContext(), nullptr != getMainRenderTarget(), "Invalid main render target")
  RHI_ASSERT(getRhi()->getContext(), mFBCommandBuffer.isEmpty(), "The command buffer is already filled")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mRootSignature, "Invalid root signature")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mFramebuffer, "Invalid framebuffer")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mTextureGroup, "Invalid texture group")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mSamplerStateGroup, "Invalid sampler state group")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
  RHI_ASSERT(getRhi()->getContext(), nullptr != mVertexArray, "Invalid vertex array")

  // Scoped debug event
  COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mFBCommandBuffer)

  { // Render to texture
    // Scoped debug event
    COMMAND_SCOPED_DEBUG_EVENT(mFBCommandBuffer, "Render to texture")

    // This in here is of course just an example. In a real application
    // there would be no point in constantly updating texture content
    // without having any real change.

    // Set the graphics render target to render into
    RERHI::Command::SetGraphicsRenderTarget::create(mFBCommandBuffer, mFramebuffer);

    // Clear the graphics color buffer of the current render target with green
    RERHI::Command::ClearGraphics::create(mFBCommandBuffer, RERHI::ClearFlag::COLOR, RECore::Color4::GREEN);

    // Restore graphics main swap chain as current render target
    RERHI::Command::SetGraphicsRenderTarget::create(mFBCommandBuffer, getMainRenderTarget());
  }

  { // Use the render to texture result
    // Scoped debug event
    COMMAND_SCOPED_DEBUG_EVENT(mFBCommandBuffer, "Use the render to texture result")

    // Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
    RERHI::Command::ClearGraphics::create(mFBCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

    // Set the used graphics root signature
    RERHI::Command::SetGraphicsRootSignature::create(mFBCommandBuffer, mRootSignature);

    // Set the used graphics pipeline state object (PSO)
    RERHI::Command::SetGraphicsPipelineState::create(mFBCommandBuffer, mGraphicsPipelineState);

    // Set graphics resource groups
    RERHI::Command::SetGraphicsResourceGroup::create(mFBCommandBuffer, 0, mTextureGroup);
    RERHI::Command::SetGraphicsResourceGroup::create(mFBCommandBuffer, 1, mSamplerStateGroup);

    // Input assembly (IA): Set the used vertex array
    RERHI::Command::SetGraphicsVertexArray::create(mFBCommandBuffer, mVertexArray);

    // Render the specified geometric primitive, based on an array of vertices
    RERHI::Command::DrawGraphics::create(mFBCommandBuffer, 6);
  }

}

void Gui::onLoadingStateChange(const RECore::IResource &resource) {
  if (this->mSceneView) {
    RE_LOG(Info, "Update")
    this->mSceneView->onLoadingStateChange(resource);
  }
}
