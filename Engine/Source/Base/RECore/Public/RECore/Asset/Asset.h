/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/String/FixedString.h"
#include "RECore/String/StringId.h"
#include "RECore/Asset/AssetTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Asset
 *
 * @brief
 * Asset class
 *
 * @remarks
 * The asset reference table is always kept in memory so we have to implement it in an efficient way.
 * No "std::string" by intent to be cache friendly and avoid memory trashing, which is important here.
 * 140 bytes per asset might sound not much, but when having e.g. 30.000 assets which is not unusual for a
 * more complex project, you end up in having a 4 MiB asset reference table in memory.
 */
struct Asset final {
  static constexpr uint32 MAXIMUM_ASSET_FILENAME_LENGTH = 127 + 1;  ///< +1 for the terminating zero

  AssetId assetId;                      ///< Asset ID
  RECore::uint64 fileHash;                      ///< 64-bit FNV-1a hash of the asset file
  char virtualFilename[MAXIMUM_ASSET_FILENAME_LENGTH];  ///< Virtual asset UTF-8 filename inside the asset package, including terminating zero
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
