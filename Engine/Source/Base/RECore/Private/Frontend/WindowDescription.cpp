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

#include "RECore/Frontend/WindowDescription.h"

namespace RECore {

WindowDescription::WindowDescription() {

}

WindowDescription::~WindowDescription() {

}

float WindowDescription::getPositionOnScreenX() const {
  return this->PositionOnScreenX;
}

void WindowDescription::setPositionOnScreenX(float PositionOnScreenX) {
  this->PositionOnScreenX = PositionOnScreenX;
}

float WindowDescription::getPositionOnScreenY() const {
  return this->PositionOnScreenY;
}

void WindowDescription::setPositionOnScreenY(float PositionOnScreenY) {
  this->PositionOnScreenY = PositionOnScreenY;
}

float WindowDescription::getWidthOnScreen() const {
  return this->WidthOnScreen;
}

void WindowDescription::setWidthOnScreen(float WidthOnScreen) {
  this->WidthOnScreen = WidthOnScreen;
}

float WindowDescription::getHeightOnScreen() const {
  return this->HeightOnScreen;
}

void WindowDescription::setHeightOnScreen(float HeightOnScreen) {
  this->HeightOnScreen = HeightOnScreen;
}

bool WindowDescription::isHasOSWindowBorder() const {
  return this->HasOSWindowBorder;
}

void WindowDescription::setHasOSWindowBorder(bool HasOSWindowBorder) {
  this->HasOSWindowBorder = HasOSWindowBorder;
}

bool WindowDescription::isInTaskbar() const {
  return this->InTaskbar;
}

void WindowDescription::setInTaskbar(bool InTaskbar) {
  this->InTaskbar = InTaskbar;
}

bool WindowDescription::isTopmostWindow() const {
  return this->IsTopmostWindow;
}

void WindowDescription::setTopmostWindow(bool IsTopmostWindow) {
  this->IsTopmostWindow = IsTopmostWindow;
}

bool WindowDescription::isAcceptsInput() const {
  return this->AcceptsInput;
}

void WindowDescription::setAcceptsInput(bool AcceptsInput) {
  this->AcceptsInput = AcceptsInput;
}

bool WindowDescription::isHasCloseButton() const {
  return this->HasCloseButton;
}

void WindowDescription::setHasCloseButton(bool HasCloseButton) {
  this->HasCloseButton = HasCloseButton;
}

bool WindowDescription::isSupportsMinimize() const {
  return this->SupportsMinimize;
}

void WindowDescription::setSupportsMinimize(bool SupportsMinimize) {
  this->SupportsMinimize = SupportsMinimize;
}

bool WindowDescription::isSupportsMaximize() const {
  return this->SupportsMaximize;
}

void WindowDescription::setSupportsMaximize(bool SupportsMaximize) {
  this->SupportsMaximize = SupportsMaximize;
}

RECore::String WindowDescription::getTitle() const {
  return this->Title;
}

void WindowDescription::setTitle(const RECore::String &sTitle) {
  this->Title = sTitle;
}

float WindowDescription::getTransparency() const {
  return this->Transparency;
}

void WindowDescription::setTransparency(float fTransparency) {
  this->Transparency = fTransparency;
}

}
