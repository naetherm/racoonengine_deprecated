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


#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/Frontend/FrontendApplication.h>


class Application : public RECore::FrontendApplication {
//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
  re_class_def()

  re_class_def_end

public:
/**
*  @brief
*    Constructor
*
*  @param[in] cFrontend
*    Frontend this application instance is running in
*  @param[in] sSurfacePainter
*    Surface painter class to use
*/
  Application(RECore::Frontend &cFrontend);

/** Destructor */
  ~Application() override;

protected:
/**
*  @brief
*    Initialization function that is called prior to OnInit()
*
*  @return
*    'true' if all went fine, else 'false' which will stop the application
*
*  @remarks
*    The default implementation does the following tasks:
*    - Everything that PLCore::CoreApplication::OnStart() does
*    - Call OnCreateRendererContext()
*    - Call OnCreatePainter()
*    - Return and go on with OnInit()
*/
  bool onStart() override;

/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*
*  @remarks
*    The default implementation does the following tasks:
*    - Save renderer related configuration
*    - Destroy renderer context
*    - Everything that FrontendApplication::OnStop() does
*/
  void onStop() override;

protected:

/**
*  @brief
*    Called to let the frontend draw into it's window
*
*  @remarks
*    The default implementation does the following tasks:
*    - Everything that PLCore::FrontendApplication::OnDraw() does
*    - Draw renderer surface
*/
  void onDraw() override;

/**
*  @brief
*    Called to let the frontend update it's states
*
*  @remarks
*    The default implementation does the following tasks:
*    - Everything that PLCore::FrontendApplication::OnUpdate() does
*    - Update renderer context
*/
  void onUpdate() override;


private:

  virtual void main() override;

};