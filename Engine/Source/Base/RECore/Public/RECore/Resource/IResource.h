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
#include "RECore/String/StringId.h"
#include "RECore/Utility/GetInvalid.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(
  4365)  // warning C4365: 'argument': conversion from 'long' to 'unsigned int', signed/unsigned mismatch
PRAGMA_WARNING_DISABLE_MSVC(
  4571)  // warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
PRAGMA_WARNING_DISABLE_MSVC(
  4668)  // warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#include <vector>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IResourceManager;

class IResourceListener;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class RECORE_API IResource {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class ResourceStreamer;  // Is changing the resource loading state
  friend class IResourceManager;  // Is changing the resource loading state


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  enum class LoadingState {
    UNLOADED,  ///< Not loaded
    LOADING,  ///< Loading is in progress
    LOADED,    ///< Fully loaded
    UNLOADING,  ///< Currently unloading	// TODO(naetherm) Currently unused
    FAILED    ///< The last loading attempt failed
  };


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
#ifdef DEBUG
  /**
  *  @brief
  *    Return the renderable debug name
  *
  *  @return
  *    The renderable debug name, never a null pointer and at least an empty string
  *
  *  @note
  *    - If possible, the renderable debug name should use the following convention: "<filename>?[<attribute 0>][<attribute n>]" (for "?" see "RECore::IFileManager::INVALID_CHARACTER")
  */
  [[nodiscard]] inline const char* getDebugName() const
  {
    return mDebugName;
  }

  inline void setDebugName(const char debugName[])
  {
    ASSERT(strlen(debugName) < 256, "Renderable debug name is not allowed to exceed 255 characters")
    strncpy(mDebugName, debugName, 256);
    mDebugName[255] = '\0';
  }
#endif

  [[nodiscard]] inline IResourceManager &getResourceManager() const {
    ASSERT(nullptr != mResourceManager, "Invalid resource manager")
    return *mResourceManager;
  }

  template<typename T>
  [[nodiscard]] inline T &getResourceManager() const {
    ASSERT(nullptr != mResourceManager, "Invalid resource manager")
    return *static_cast<T *>(mResourceManager);
  }

  [[nodiscard]] inline ResourceId getId() const {
    return mResourceId;
  }

  [[nodiscard]] inline AssetId getAssetId() const {
    return mAssetId;
  }

  [[nodiscard]] inline ResourceLoaderTypeId getResourceLoaderTypeId() const {
    return mResourceLoaderTypeId;
  }

  [[nodiscard]] inline LoadingState getLoadingState() const {
    return mLoadingState;
  }

  void connectResourceListener(
    IResourceListener &resourceListener);  // No guaranteed resource listener caller order, if already connected nothing happens (no double registration)
  void disconnectResourceListener(IResourceListener &resourceListener);


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  inline IResource() :
#ifdef DEBUG
    mDebugName{},
#endif
    mResourceManager(nullptr),
    mResourceId(RECore::getInvalid<ResourceId>()),
    mAssetId(RECore::getInvalid<AssetId>()),
    mResourceLoaderTypeId(RECore::getInvalid<ResourceLoaderTypeId>()),
    mLoadingState(LoadingState::UNLOADED) {
    // Nothing here
  }

  inline virtual ~IResource() {
    // Sanity checks
    ASSERT('\0' == mDebugName[0], "Invalid debug name")
    ASSERT(nullptr == mResourceManager, "Invalid resource manager")
    ASSERT(RECore::isInvalid(mResourceId), "Invalid resource ID")
    ASSERT(RECore::isInvalid(mAssetId), "Invalid asset ID")
    ASSERT(RECore::isInvalid(mResourceLoaderTypeId), "Invalid resource loader type ID")
    ASSERT(LoadingState::UNLOADED == mLoadingState || LoadingState::FAILED == mLoadingState, "Invalid loading state")
    ASSERT(mSortedResourceListeners.empty(), "Invalid sorted resource listeners")
  }

  explicit IResource(const IResource &) = delete;

  IResource &operator=(const IResource &) = delete;

  IResource &operator=(IResource &&resource);

  inline void setResourceManager(IResourceManager *resourceManager) {
    mResourceManager = resourceManager;
  }

  inline void setAssetId(AssetId assetId) {
    mAssetId = assetId;
  }

  inline void setResourceLoaderTypeId(ResourceLoaderTypeId resourceLoaderTypeId) {
    mResourceLoaderTypeId = resourceLoaderTypeId;
  }

  void setLoadingState(LoadingState loadingState);

  //[-------------------------------------------------------]
  //[ "Renderer::PackedElementManager" management           ]
  //[-------------------------------------------------------]
  inline void initializeElement(ResourceId resourceId) {
    // Sanity checks
    ASSERT('\0' == mDebugName[0], "Invalid debug name")
    ASSERT(nullptr == mResourceManager, "Invalid resource manager")
    ASSERT(RECore::isInvalid(mResourceId), "Invalid resource ID")
    ASSERT(RECore::isInvalid(mAssetId), "Invalid asset ID")
    ASSERT(RECore::isInvalid(mResourceLoaderTypeId), "Invalid resource loader type ID")
    ASSERT(LoadingState::UNLOADED == mLoadingState, "Invalid loading state")
    ASSERT(mSortedResourceListeners.empty(), "Invalid sorted resource listeners")

    // Set data
    mResourceId = resourceId;
  }

  void deinitializeElement();


  //[-------------------------------------------------------]
  //[ Private definitions                                   ]
  //[-------------------------------------------------------]
private:
  typedef std::vector<IResourceListener *> SortedResourceListeners;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
#ifdef DEBUG
  char				mDebugName[256];		///< Debug name for easier resource identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
#endif
  IResourceManager *mResourceManager;    ///< Owner resource manager, always valid
  ResourceId mResourceId;      ///< Unique resource ID inside the resource manager
  AssetId mAssetId;        ///< In case the resource is an instance of an asset, this is the ID of this asset
  ResourceLoaderTypeId mResourceLoaderTypeId;
  LoadingState mLoadingState;
  SortedResourceListeners mSortedResourceListeners;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
