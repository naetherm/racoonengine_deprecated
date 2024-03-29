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
#include "REGui/Widget/Dialog/FileDialog.h"
#include "REGui/Widget/Layout/Layout.h"
#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>
#include <imgui_internal.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FileDialog, "REGui", REGui::ModalDialog, "Application class")
// Constructors
re_class_metadata_end(FileDialog)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
FileDialog::FileDialog() {

}

FileDialog::~FileDialog() {

}

void FileDialog::construct(ConstructionArguments args) {
  mTitle = args.getTitle();
  mLayout = args.getLayout();
  mFileDialogLocation = args.getFileDialogLocation();
  mDirectoryMode = args.getDirectoryMode();
}

void FileDialog::onUpdate(float deltaTime) {

}

void FileDialog::onDraw() {
  /*
  // Nothing to do here, just drawn Compound
  ImGuiFileDialog ::Instance()->OpenDialog(
    mDirectoryMode ? "ChooseDirDlgKey" : "ChooseFileDlgKey",
    mDirectoryMode ? "Choose Directory" : "Choose File",
    "",
    mFileDialogLocation.cstr(), 1, nullptr, ImGuiFileDialogFlags_Modal);

  // Display
  if (ImGuiFileDialog::Instance()->Display(mDirectoryMode ? "ChooseDirDlgKey" : "ChooseFileDlgKey")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      // Get filepath name
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      // get current path
      std::string filePath = ImGuiFileDialog ::Instance()->GetCurrentPath();
    }

    // Close
    ImGuiFileDialog::Instance()->Close();
  }
 */
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
