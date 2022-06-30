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
#include "REEd/REEdMainWindow.h"
#include "REEd/REEdApp.h"
#include "REEd/LogPanel/LogPanel.h"
#include "REEd/AssetBrowser/AssetBrowser.h"
#include "REEd/EntityInspector/EntityInspector.h"
#include "REEd/Views/SceneView/SceneView.h"
#include "REEd/WorldHierarchy/WorldHierarchy.h"


REEdMainWindow::REEdMainWindow(REEdApp* app, REGui::Gui* gui)
: REGui::DockableMainWindow(gui)
, mApp(app)
, mLogPanel(nullptr)
, mAssetBrowser(nullptr)
, mEntityInspector(nullptr)
, mSceneView(nullptr)
, mWorldHierarchy(nullptr) {
  mLogPanel = new LogPanel();
  mAssetBrowser = new AssetBrowser(mApp);
  mEntityInspector = new EntityInspector();
  mSceneView = new SceneView();
  mWorldHierarchy = new WorldHierarchy();
}

REEdMainWindow::~REEdMainWindow() {
  delete mLogPanel;
  delete mAssetBrowser;
  delete mEntityInspector;
  delete mSceneView;
  delete mWorldHierarchy;
}

void REEdMainWindow::onDraw() {
  REGui::DockableMainWindow::onDraw();
  mLogPanel->onDraw();
  mAssetBrowser->onDraw();
  mEntityInspector->onDraw();
  mSceneView->onDraw();
  mWorldHierarchy->onDraw();

  ImGui::ShowDemoWindow();
}
