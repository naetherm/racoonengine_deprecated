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
#include "REGui/Gui/GuiManager.h"
#if defined(LINUX)
#include "REGui/Backend/Linux/GuiManagerLinux.h"
#elif defined(WIN32)
#include "REGui/Backend/Windows/GuiManagerWindows.h"
#endif
#include <RECore/Asset/Asset.h>
#include <RECore/File/IFileManager.h>
#include <RECore/Log/Log.h>
#include <RECore/Memory/Memory.h>
#include <RERenderer/IRenderer.h>
#include <RERenderer/Context.h>
#include <RERenderer/Resource/Texture/TextureResourceManager.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/detail/setup.hpp>	// For "GLM_COUNTOF()"

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
      0,											// semanticIndex (uint32_t)
      // Data source
      0,											// inputSlot (uint32_t)
      0,											// alignedByteOffset (uint32_t)
      sizeof(float) * 4 + sizeof(uint8_t) * 4,	// strideInBytes (uint32_t)
      0											// instancesPerElement (uint32_t)
    },
    { // Attribute 1
      // Data destination
      RERHI::VertexAttributeFormat::R8G8B8A8_UNORM,	// vertexAttributeFormat (RERHI::VertexAttributeFormat)
      "Color",									// name[32] (char)
      "COLOR",									// semanticName[32] (char)
      0,											// semanticIndex (uint32_t)
      // Data source
      0,											// inputSlot (uint32_t)
      sizeof(float) * 4,							// alignedByteOffset (uint32_t)
      sizeof(float) * 4 + sizeof(uint8_t) * 4,	// strideInBytes (uint32_t)
      0											// instancesPerElement (uint32_t)
    }
  };
const RERHI::VertexAttributes VertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(VertexAttributesLayout)), VertexAttributesLayout);


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
//[ Classes                                               ]
//[-------------------------------------------------------]
void GuiManager::setImGuiAllocatorFunctions() {
  ImGui::SetAllocatorFunctions(::detail::AllocFunc, ::detail::FreeFunc, nullptr);
}

void GuiManager::getDefaultTextureAssetIds(AssetIds &assetIds) {
  assetIds.push_back(::detail::IMGUI_GLYPH_MAP_2D);
}

GuiManager::GuiManager(RERenderer::IRenderer& renderer)
: mRenderer(renderer)
, mImGuiContext(nullptr)
, mTheme(nullptr)
, mIsRunning(false)
, mObjectSpaceToClipSpaceMatrixUniformHandle(NULL_HANDLE)
, mNumberOfAllocatedVertices(0)
, mNumberOfAllocatedIndices(0) {
#if defined(LINUX)
  mImpl = new GuiManagerLinux(this);
#elif defined(WIN32)

#endif
}

GuiManager::~GuiManager() {
  if (mImpl) {
    delete mImpl;
  }

  mImpl = nullptr;
}


void GuiManager::startUp() {
  { // Create default font texture instance
    // Build texture atlas
    unsigned char* pixels = nullptr;
    int width = 0;
    int height = 0;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload the texture information to the RHI
    mDefaultFontTexture = mRenderer.getTextureManager().createTexture2D(
      static_cast<RECore::uint32>(width),
      static_cast<RECore::uint32>(height),
      RERHI::TextureFormat::R8G8B8A8,
      pixels,
      RERHI::TextureFlag::GENERATE_MIPMAPS | RERHI::TextureFlag::SHADER_RESOURCE,
      RERHI::TextureUsage::DEFAULT,
      1,
      nullptr
      RHI_RESOURCE_DEBUG_NAME("Debug 2D GUI glyph texture atlas"));

    // Tell the texture resource manager about the glyph texture, so it can be referenced later on
    mRenderer.getTextureResourceManager().createTextureResourceByAssetId(
      ::detail::IMGUI_GLYPH_MAP_2D,
      *mDefaultFontTexture);
  }
}

void GuiManager::initializeImGuiKeyMap() {
  mImpl->initializeImGuiKeyMap();
}

void GuiManager::onNewFrame(RERHI::RHIRenderTarget &renderTarget) {
  // If not already started
  if (!mIsRunning) {
    startUp();
    mIsRunning = true;
  }

  // Call the platform specific implementation
  mImpl->onNewFrame(renderTarget);

  // Start the frame
  ImGui::NewFrame();
}

const RERHI::RHIVertexArrayPtr &GuiManager::getFillVertexArrayPtr(RERHI::RHICommandBuffer *commandBuffer) {
  if (GImGui->Initialized) {

  }

  // Done
  return mVertexArray;
}

void GuiManager::fillGraphicsCommandBuffer(RERHI::RHICommandBuffer &commandBuffer) {

}

void GuiManager::fillGraphicsCommandBufferUsingFixedBuiltinRhiConfiguration(RERHI::RHICommandBuffer &commandBuffer) {

}

void GuiManager::registerTexture(RERHI::RHITexture2D *texture) {

}

RERHI::RHIResourceGroupPtr GuiManager::getResourceGroupByTexture(RERHI::RHITexture2D *texture) {
  return RERHI::RHIResourceGroupPtr();
}

void GuiManager::createFixedBuiltinRhiConfigurationResources() {

}


void GuiManager::onWindowResize(RECore::uint32 width, RECore::uint32 height) {
  mImpl->onWindowResize(width, height);
}

void GuiManager::onKeyInput(RECore::uint32 keySym, char character, bool pressed) {
  mImpl->onKeyInput(keySym, character, pressed);
}

void GuiManager::onMouseMoveInput(int x, int y) {
  mImpl->onMouseMoveInput(x, y);
}

void GuiManager::onMouseButtonInput(RECore::uint32 button, bool pressed) {
  mImpl->onMouseButtonInput(button, pressed);
}

void GuiManager::onMouseWheelInput(bool scrollUp) {
  mImpl->onMouseWheelInput(scrollUp);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
