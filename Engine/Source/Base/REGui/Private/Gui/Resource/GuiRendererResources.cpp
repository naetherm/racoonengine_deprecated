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
#include "REGui/Gui/Resource/GuiRendererResources.h"
#include "REGui/Gui/GuiRenderer.h"
#include <RECore/Asset/Asset.h>
#include <RECore/Memory/Memory.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
#include <glm/detail/setup.hpp>	// For "GLM_COUNTOF()"
PRAGMA_WARNING_POP


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
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiRendererResources::GuiRendererResources(GuiRenderer *guiRenderer)
: mGuiRenderer(guiRenderer) {

}

GuiRendererResources::~GuiRendererResources() {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui