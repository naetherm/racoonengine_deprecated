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
#include "REGui/Backend/Linux/GuiManagerLinux.h"
#include <RECore/Linux/X11Includes.h>
#include <sys/time.h>
#include <imgui.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
GuiManagerLinux::GuiManagerLinux(GuiManager *guiManager)
: GuiManagerImpl(guiManager) {

}

GuiManagerLinux::~GuiManagerLinux() {
  // Nothing to do here
}

void GuiManagerLinux::onWindowResize(RECore::uint32 width, RECore::uint32 height) {
  mWindowWidth = width;
  mWindowHeight = height;
}

void GuiManagerLinux::onKeyInput(RECore::uint32 keySym, char character, bool pressed) {
  ImGuiIO& imGuiIo = ImGui::GetIO();
  if (keySym < 512)
  {
    imGuiIo.KeysDown[keySym] = pressed;
  }
  else if (XK_Alt_L == keySym)
  {
    imGuiIo.KeyAlt = pressed;
  }
  else if (XK_Shift_L == keySym)
  {
    imGuiIo.KeyShift = pressed;
  }
  else if (XK_Control_L == keySym)
  {
    imGuiIo.KeyCtrl = pressed;
  }
  else if (XK_Super_L == keySym)
  {
    imGuiIo.KeySuper = pressed;
  }
  else if ((keySym & 0xff00) == 0xff00)
  {
    // It is a special key (e.g. tab key) map the value to a range between 0x0ff and 0x1ff
    imGuiIo.KeysDown[(keySym & 0x1ff)] = pressed;
  }
  if (pressed && character > 0)
  {
    imGuiIo.AddInputCharacter(character);
  }
}

void GuiManagerLinux::onMouseMoveInput(int x, int y) {
  float windowWidth = 1.0f;
  float windowHeight = 1.0f;

  {
    // Ensure that none of them is ever zero
    if (mWindowWidth >= 1) {
      windowWidth = static_cast<float>(mWindowWidth);
    }
    if (mWindowHeight >= 1) {
      windowHeight = static_cast<float>(mWindowHeight);
    }
  }

  // Update
  {
    ImGuiIO& imGuiIO = ImGui::GetIO();
    imGuiIO.MousePos.x = static_cast<float>(x) * (imGuiIO.DisplaySize.x / windowWidth);
    imGuiIO.MousePos.y = static_cast<float>(y) * (imGuiIO.DisplaySize.y / windowHeight);
  }
}

void GuiManagerLinux::onMouseButtonInput(RECore::uint32 button, bool pressed) {
  if (button > 0 && button <= 5) {
    ImGui::GetIO().MouseDown[button - 1] = pressed;
  }
}

void GuiManagerLinux::onMouseWheelInput(bool scrollUp) {
  ImGui::GetIO().MouseWheel += scrollUp ? -1.0f : 1.0f;
}

void GuiManagerLinux::initializeImGuiKeyMap() {
  ImGuiIO& imGuiIo = ImGui::GetIO();

  imGuiIo.KeyMap[ImGuiKey_Tab]		= (XK_Tab & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_LeftArrow]	= (XK_Left & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_RightArrow]	= (XK_Right & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_UpArrow]	= (XK_Up & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_DownArrow]	= (XK_Down & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_PageUp]		= (XK_Page_Up & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_PageDown]	= (XK_Page_Down & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Home]		= (XK_Home & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_End]		= (XK_End & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Insert]		= (XK_Insert & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Delete]		= (XK_Delete & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Backspace]	= (XK_BackSpace & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Space]		= (XK_space & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Enter]		= (XK_Return & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_Escape]		= (XK_Escape & 0x1ff);
  imGuiIo.KeyMap[ImGuiKey_A]			= XK_a;
  imGuiIo.KeyMap[ImGuiKey_B]			= XK_b;
  imGuiIo.KeyMap[ImGuiKey_C]			= XK_c;
  imGuiIo.KeyMap[ImGuiKey_D]			= XK_d;
  imGuiIo.KeyMap[ImGuiKey_E]			= XK_e;
  imGuiIo.KeyMap[ImGuiKey_F]			= XK_f;
  imGuiIo.KeyMap[ImGuiKey_G]			= XK_g;
  imGuiIo.KeyMap[ImGuiKey_H]			= XK_h;
  imGuiIo.KeyMap[ImGuiKey_I]			= XK_i;
  imGuiIo.KeyMap[ImGuiKey_J]			= XK_j;
  imGuiIo.KeyMap[ImGuiKey_K]			= XK_k;
  imGuiIo.KeyMap[ImGuiKey_L]			= XK_l;
  imGuiIo.KeyMap[ImGuiKey_M]			= XK_m;
  imGuiIo.KeyMap[ImGuiKey_N]			= XK_n;
  imGuiIo.KeyMap[ImGuiKey_O]			= XK_o;
  imGuiIo.KeyMap[ImGuiKey_P]			= XK_p;
  imGuiIo.KeyMap[ImGuiKey_Q]			= XK_q;
  imGuiIo.KeyMap[ImGuiKey_R]			= XK_r;
  imGuiIo.KeyMap[ImGuiKey_S]			= XK_s;
  imGuiIo.KeyMap[ImGuiKey_T]			= XK_t;
  imGuiIo.KeyMap[ImGuiKey_U]			= XK_u;
  imGuiIo.KeyMap[ImGuiKey_V]			= XK_v;
  imGuiIo.KeyMap[ImGuiKey_W]			= XK_w;
  imGuiIo.KeyMap[ImGuiKey_X]			= XK_x;
  imGuiIo.KeyMap[ImGuiKey_Y]			= XK_y;
  imGuiIo.KeyMap[ImGuiKey_Z]			= XK_z;
}

void GuiManagerLinux::onNewFrame(RERHI::RHIRenderTarget &renderTarget) {
  ImGuiIO& imGuiIo = ImGui::GetIO();

  { // Setup display size (every frame to accommodate for render target resizing)
    RECore::uint32 width = 0;
    RECore::uint32 height = 0;
    renderTarget.getWidthAndHeight(width, height);
    imGuiIo.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
  }

  { // Setup time step
    timeval currentTimeValue;
    gettimeofday(&currentTimeValue, nullptr);
    const RECore::uint64 currentTime = currentTimeValue.tv_sec * 1000000 + currentTimeValue.tv_usec;
    imGuiIo.DeltaTime = static_cast<float>(currentTime - mTime) / 1000000.0f;
    mTime = currentTime;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
