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
#include "RERenderer/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.h"
#include "RERenderer/Resource/Texture/TextureResource.h"
#include "RERenderer/IRenderer.h"
#include "RERenderer/Context.h"
#include <RECore/Log/Log.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(5204)	// warning C5204: 'crnlib::task_pool::executable_task': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
	#include <openvr/openvr.h>
PRAGMA_WARNING_POP

#include <chrono>
#include <thread>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void OpenVRTextureResourceLoader::onProcessing()
	{
		// Load the render model texture
		vr::IVRRenderModels* vrRenderModels = vr::VRRenderModels();
		vr::EVRRenderModelError vrRenderModelError = vr::VRRenderModelError_Loading;
		const vr::TextureID_t albedoTextureId = static_cast<vr::TextureID_t>(std::atoi(getAsset().virtualFilename));
		while (vrRenderModelError == vr::VRRenderModelError_Loading)
		{
			vrRenderModelError = vrRenderModels->LoadTexture_Async(albedoTextureId, &mVrRenderModelTextureMap);
			if (vrRenderModelError == vr::VRRenderModelError_Loading)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1ms);
			}
		}
		if (vr::VRRenderModelError_None != vrRenderModelError)
		{
      RE_LOG(Critical, std::string("The renderer was unable to load OpenVR albedo texture ") + std::to_string(albedoTextureId) + std::string(": ") + vrRenderModels->GetRenderModelErrorNameFromEnum(vrRenderModelError))
			return;
		}

		{ // The "_argb_nxa" texture channel packing stores the x channel of a normal map inside the alpha channel, set identity normal map x value
			uint8_t* rubTextureMapData = const_cast<uint8_t*>(mVrRenderModelTextureMap->rubTextureMapData);	// Evil const-cast since I don't want to copy the data
			const uint8_t* rubTextureMapDataEnd = rubTextureMapData + mVrRenderModelTextureMap->unWidth * mVrRenderModelTextureMap->unHeight * 4;
			for (; rubTextureMapData < rubTextureMapDataEnd; rubTextureMapData += 4)
			{
				rubTextureMapData[3] = 128;
			}
		}

		// Can we create the RHI resource asynchronous as well?
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			mTexture = createRhiTexture();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected RERenderer::ITextureResourceLoader methods    ]
	//[-------------------------------------------------------]
	RERHI::RHITexture* OpenVRTextureResourceLoader::createRhiTexture()
	{
		RERHI::RHITexture2D* texture2D = nullptr;
		if (nullptr != mVrRenderModelTextureMap)
		{
			// Create the RHI texture instance
			const bool rgbHardwareGammaCorrection = true;	// TODO(naetherm) It must be possible to set the property name from the outside: Ask the material blueprint whether or not hardware gamma correction should be used
			texture2D = mRenderer.getTextureManager().createTexture2D(mVrRenderModelTextureMap->unWidth, mVrRenderModelTextureMap->unHeight, rgbHardwareGammaCorrection ? RERHI::TextureFormat::R8G8B8A8_SRGB : RERHI::TextureFormat::R8G8B8A8, static_cast<const void*>(mVrRenderModelTextureMap->rubTextureMapData), RERHI::TextureFlag::GENERATE_MIPMAPS | RERHI::TextureFlag::SHADER_RESOURCE, RERHI::TextureUsage::DEFAULT, 1, nullptr RHI_RESOURCE_DEBUG_NAME(getAsset().virtualFilename));

			// Free the render model texture
			vr::VRRenderModels()->FreeTexture(mVrRenderModelTextureMap);
			mVrRenderModelTextureMap = nullptr;
		}

		// Done
		return texture2D;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
