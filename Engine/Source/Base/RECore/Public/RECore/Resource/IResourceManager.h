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
#include "RECore/RECore.h"
#include "RECore/Resource/ResourceTypes.h"
#include "RECore/Core/Manager.h"
#include "RECore/Resource/IResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IResourceLoader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Public abstract resource manager interface
*/
class RECORE_API IResourceManager : private RECore::Manager {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class RendererImpl;    // Needs to be able to destroy resource manager instances
  friend class ResourceStreamer;  // Needs to be able to create resource loader instances


  //[-------------------------------------------------------]
  //[ Public virtual Renderer::IResourceManager methods     ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual uint32 getNumberOfResources() const = 0;

  [[nodiscard]] virtual IResource &getResourceByIndex(uint32 index) const = 0;

  [[nodiscard]] virtual IResource &getResourceByResourceId(ResourceId resourceId) const = 0;

  [[nodiscard]] virtual IResource *tryGetResourceByResourceId(ResourceId resourceId) const = 0;

  virtual void reloadResourceByAssetId(AssetId assetId) = 0;

  virtual void update() = 0;


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
public:
  inline IResourceManager() {
    // Nothing here
  }

  inline virtual ~IResourceManager() {
    // Noting here
  }

  explicit IResourceManager(const IResourceManager &) = delete;

  IResourceManager &operator=(const IResourceManager &) = delete;

  inline void setResourceLoadingState(IResource &resource, IResource::LoadingState loadingState) {
    resource.setLoadingState(loadingState);
  }


  //[-------------------------------------------------------]
  //[ Private virtual Renderer::IResourceManager methods    ]
  //[-------------------------------------------------------]
private:
  [[nodiscard]] virtual IResourceLoader *createResourceLoaderInstance(ResourceLoaderTypeId resourceLoaderTypeId) = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
