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
#include "RECore/Core/AbstractContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class AssetManager;
class IFileManager;
class ResourceStreamer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CoreContext
 *
 * @brief
 * Core context, containing information about the file- and asset manager as well as
 * resource streaming.
 *
 * @note
 * We do not predefine the root path of the file manager, it is the job of the developer
 * to define this path for a specific application. For example, the root path of an
 * application launcher will be different to the root path of an editor implementation
 * or a simple example.
 */
class RECORE_API CoreContext : public AbstractContext {
public:

  /**
   * @brief
   * Constructor.
   */
  CoreContext();

  /**
   * @brief
   * Destructor.
   */
  ~CoreContext() override;


  /**
   * @brief
   * Initializes the core context.
   * We do not predefine the root path of the file manager, it is the job of the developer
   * to define this path for a specific application. For example, the root path of an
   * application launcher will be different to the root path of an editor implementation
   * or a simple example.
   *
   * When this method is called it will further initialize the asset manager and the resource
   * streamer as those processes are dependent on the file manager.
   *
   * @param[in] fileManager
   * Reference to the file manager.
   */
  void initialize(IFileManager& fileManager);


  /**
   * @brief
   * Returns reference to the file manager.
   *
   * @return
   * File manager.
   */
  [[nodiscard]] const IFileManager &getFileManager() const;

  /**
   * @brief
   * Returns reference to the file manager.
   *
   * @return
   * File manager.
   */
  [[nodiscard]] IFileManager &getFileManager();

  /**
   * @brief
   * Returns reference to the asset manager.
   *
   * @return
   * Asset manager.
   */
  [[nodiscard]] const AssetManager &getAssetManager() const;

  /**
   * @brief
   * Returns reference to the asset manager.
   *
   * @return
   * Asset manager.
   */
  [[nodiscard]] AssetManager &getAssetManager();

  /**
   * @brief
   * Returns reference to the resource streamer.
   *
   * @return
   * Resource streamer.
   */
  [[nodiscard]] const ResourceStreamer &getResourceStreamer() const;

  /**
   * @brief
   * Returns reference to the resource streamer.
   *
   * @return
   * Resource streamer.
   */
  [[nodiscard]] ResourceStreamer &getResourceStreamer();

protected:
  /** Pointer to the file manager */
  IFileManager *mpFileManager;
  /** Pointer to the asset manager */
  AssetManager *mpAssetManager;
  /** Pointer to the resource streamer */
  ResourceStreamer *mpResourceStreamer;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
