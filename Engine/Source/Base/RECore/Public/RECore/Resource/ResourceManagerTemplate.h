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
#include "RECore/Resource/PackedElementManager.h"
#include "RECore/Asset/AssetManager.h"
#include "RECore/Resource/ResourceStreamer.h"
#include "RECore/Resource/ResourceTypes.h"



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IResourceManager;
class ResourceStreamer;
class IResourceListener;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Internal resource manager template; not public used to keep template instantiation overhead under control
*/
template<class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32 MAXIMUM_NUMBER_OF_ELEMENTS>
class ResourceManagerTemplate : private RECore::Manager {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  typedef PackedElementManager<TYPE, ID_TYPE, MAXIMUM_NUMBER_OF_ELEMENTS> Resources;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  inline ResourceManagerTemplate(RECore::AssetManager &assetManager, ResourceStreamer &resourceStreamer,
                                 IResourceManager &resourceManager) :
    mAssetManager(assetManager),
    mResourceStreamer(resourceStreamer),
    mResourceManager(resourceManager) {
    // Nothing here
  }

  inline ~ResourceManagerTemplate() {
    // Nothing here
  }

  [[nodiscard]] inline IResourceManager &getResourceManager() const {
    return mResourceManager;
  }

  [[nodiscard]] inline TYPE *
  getResourceByAssetId(AssetId assetId) const  // Considered to be inefficient, avoid method whenever possible
  {
    // TODO(naetherm) Implement more efficient solution later on
    const uint32 numberOfElements = mResources.getNumberOfElements();
    for (uint32 i = 0; i < numberOfElements; ++i) {
      TYPE &resource = mResources.getElementByIndex(i);
      if (resource.getAssetId() == assetId) {
        return &resource;
      }
    }

    // There's no resource using the given asset ID
    return nullptr;
  }

  [[nodiscard]] inline TYPE &createEmptyResourceByAssetId(AssetId assetId)  // Resource is not allowed to exist, yet
  {
    // Sanity check
    RHI_ASSERT(nullptr == getResourceByAssetId(assetId), "The resource isn't allowed to exist, yet")

    // Create the resource instance
    TYPE &resource = mResources.addElement();
    resource.setResourceManager(&mResourceManager);
    resource.setAssetId(assetId);

    // Done
    return resource;
  }

  inline void
  loadResourceByAssetId(AssetId assetId, ID_TYPE &resourceId, IResourceListener *resourceListener, bool reload,
                        ResourceLoaderTypeId resourceLoaderTypeId)  // Asynchronous
  {
    // Choose default resource loader type ID, if necessary
    if (RECore::isInvalid(resourceLoaderTypeId)) {
      resourceLoaderTypeId = LOADER_TYPE::TYPE_ID;
    }

    // Get or create the instance
    TYPE *resource = getResourceByAssetId(assetId);

    // Create the resource instance
    const RECore::Asset *asset = mAssetManager.tryGetAssetByAssetId(assetId);
    RHI_ASSERT(nullptr != asset, "Unknown asset ID")
    bool load = (reload && nullptr != asset);
    if (nullptr == resource && nullptr != asset) {
      resource = &mResources.addElement();
      resource->setResourceManager(&mResourceManager);
      resource->setAssetId(assetId);
      resource->setResourceLoaderTypeId(resourceLoaderTypeId);
      load = true;
    }

    // Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
    if (nullptr != resource) {
      resourceId = resource->getId();
      if (nullptr != resourceListener) {
        resource->connectResourceListener(*resourceListener);
      }
    } else {
      resourceId = RECore::getInvalid<ID_TYPE>();
    }

    // Load the resource, if required
    if (load) {
      // Commit resource streamer asset load request
      mResourceStreamer.commitLoadRequest(
        ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, mResourceManager, resourceId));
    }
  }

  inline void reloadResourceByAssetId(AssetId assetId) {
    // TODO(naetherm) Experimental implementation (take care of resource cleanup etc.)
    const uint32 numberOfElements = mResources.getNumberOfElements();
    for (uint32 i = 0; i < numberOfElements; ++i) {
      const TYPE &resource = mResources.getElementByIndex(i);
      if (resource.getAssetId() == assetId) {
        ID_TYPE resourceId = RECore::getInvalid<ID_TYPE>();
        loadResourceByAssetId(assetId, resourceId, nullptr, true, resource.getResourceLoaderTypeId());
        break;
      }
    }
  }

  [[nodiscard]] inline Resources &getResources() {
    return mResources;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ResourceManagerTemplate(const ResourceManagerTemplate &) = delete;

  ResourceManagerTemplate &operator=(const ResourceManagerTemplate &) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::AssetManager &mAssetManager;  ///< Renderer instance, do not destroy the instance
  ResourceStreamer &mResourceStreamer;
  IResourceManager &mResourceManager;
  Resources mResources;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
