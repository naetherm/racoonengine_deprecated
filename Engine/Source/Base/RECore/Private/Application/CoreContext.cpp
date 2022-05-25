#////////////////////////////////////////////////////////////////////////////////////////////////////
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
#include "RECore/Application/CoreContext.h"
#include "RECore/File/IFileManager.h"
#include "RECore/Asset/AssetManager.h"
#include "RECore/Resource/ResourceStreamer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
CoreContext::CoreContext()
: mpFileManager(nullptr)
, mpAssetManager(nullptr)
, mpResourceStreamer(nullptr) {

}

CoreContext::~CoreContext() {
  delete this->mpResourceStreamer;
  delete this->mpAssetManager;
  // Do not delete the file manager as it is managed outside this context!
}


void CoreContext::initialize(IFileManager &fileManager) {
  this->mpFileManager = &fileManager;
  this->mpAssetManager = new AssetManager(*this->mpFileManager);
  this->mpResourceStreamer = new ResourceStreamer(*this->mpFileManager);
}

const IFileManager &CoreContext::getFileManager() const {
  return *this->mpFileManager;
}

IFileManager &CoreContext::getFileManager() {
  return *this->mpFileManager;
}

const AssetManager &CoreContext::getAssetManager() const {
  return *this->mpAssetManager;
}

AssetManager &CoreContext::getAssetManager() {
  return *this->mpAssetManager;
}

const ResourceStreamer &CoreContext::getResourceStreamer() const {
  return *this->mpResourceStreamer;
}

ResourceStreamer &CoreContext::getResourceStreamer() {
  return *this->mpResourceStreamer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore