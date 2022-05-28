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
#include "RECore/Windows/WindowsConsole.h"
#include <Tchar.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <shellapi.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public virtual Console functions                      ]
//[-------------------------------------------------------]
void WindowsConsole::print(const String &sString) const {
  fputs(sString, stdout);
}

int WindowsConsole::isKeyHit() const {
  return _kbhit();
}

int WindowsConsole::getCharacter(bool bEcho) const {
  return bEcho ? _getche() : _getch();
}

void WindowsConsole::clearScreen() const {
  // This implementation comes from: http://www.cprogramming.com/faq/cgi-bin/smartfaq.cgi?answer=1031963460&id=1043284385
  //                                 "OPTION 6 (Credit: Sunlight)"

  // Get a handle to the console
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(h, &csbi);

  // Find the number of characters to overwrite
  const DWORD nSize = csbi.dwSize.X * csbi.dwSize.Y;

  // Overwrite the screen buffer with whitespace
  COORD sCoord = {0, 0};  // Top left screen position
  DWORD n = 0;      // Number of characters written
  FillConsoleOutputCharacter(h, TEXT(' '), nSize, sCoord, &n);
  GetConsoleScreenBufferInfo(h, &csbi);
  FillConsoleOutputAttribute(h, csbi.wAttributes, nSize, sCoord, &n);

  // Reset the cursor to the top left position
  ::SetConsoleCursorPosition(h, sCoord);
}

void WindowsConsole::getCursorPosition(uint16 &nX, uint16 &nY) const {
  CONSOLE_SCREEN_BUFFER_INFO sConsoleScreenBufferInfo;

  // Get the cursor position
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sConsoleScreenBufferInfo);
  nX = sConsoleScreenBufferInfo.dwCursorPosition.X;
  nY = sConsoleScreenBufferInfo.dwCursorPosition.Y;
}

void WindowsConsole::setCursorPosition(uint16 nX, uint16 nY) const {
  // Get a handle to the console
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

  // Set the cursor position
  const COORD sCoord = {nX, nY};
  ::SetConsoleCursorPosition(h, sCoord);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore