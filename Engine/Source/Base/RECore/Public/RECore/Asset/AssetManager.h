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
#include "RECore/Core/Manager.h"
#include "RECore/Asset/Asset.h"
#include "RECore/String/StringId.h"

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
class AssetPackage;

class IFileManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class AssetManager final : private RECore::Manager {


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  typedef std::vector<AssetPackage *> AssetPackageVector;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  inline explicit AssetManager(IFileManager &fileManager) :
    mFileManager(fileManager) {
    // Nothing here
  }

  explicit AssetManager(const AssetManager &) = delete;

  AssetManager &operator=(const AssetManager &) = delete;

  inline ~AssetManager() {
    clear();
  }

  //[-------------------------------------------------------]
  //[ Asset package                                         ]
  //[-------------------------------------------------------]
  void clear();

  [[nodiscard]] AssetPackage &addAssetPackage(AssetPackageId assetPackageId);

  AssetPackage *mountAssetPackage(AbsoluteDirectoryName absoluteDirectoryName, const char *projectName);

  [[nodiscard]] AssetPackage &getAssetPackageById(AssetPackageId assetPackageId) const;

  [[nodiscard]] AssetPackage *tryGetAssetPackageById(AssetPackageId assetPackageId) const;

  void removeAssetPackage(AssetPackageId assetPackageId);

  //[-------------------------------------------------------]
  //[ Asset                                                 ]
  //[-------------------------------------------------------]
  [[nodiscard]] const Asset *tryGetAssetByAssetId(AssetId assetId) const;

  [[nodiscard]] inline const Asset &getAssetByAssetId(AssetId assetId) const {
    const Asset *asset = tryGetAssetByAssetId(assetId);
    RHI_ASSERT(nullptr != asset, "Invalid asset")
    return *asset;
  }

  [[nodiscard]] inline VirtualFilename tryGetVirtualFilenameByAssetId(AssetId assetId) const {
    const Asset *asset = tryGetAssetByAssetId(assetId);
    return (nullptr != asset) ? asset->virtualFilename : nullptr;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:

  [[nodiscard]] AssetPackage *
  addAssetPackageByVirtualFilename(AssetPackageId assetPackageId, VirtualFilename virtualFilename);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  IFileManager &mFileManager;  ///< File Manager, do not destroy the instance
  AssetPackageVector mAssetPackageVector;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
