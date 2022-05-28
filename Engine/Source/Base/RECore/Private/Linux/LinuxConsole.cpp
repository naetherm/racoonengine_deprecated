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
#include "RECore/Linux/LinuxConsole.h"
#include <unistd.h>

#ifndef ANDROID
// "term.h" is not available within the Android NDK nor required in this use case, but for *normal* Linux we need it
#include <term.h>

#endif

#include <termios.h>
#include <sys/ioctl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public virtual Console functions                      ]
//[-------------------------------------------------------]
void LinuxConsole::print(const String &sString) const {
  fputs(sString, stdout);
  fflush(stdout);
}

int LinuxConsole::isKeyHit() const {
  // Linux (POSIX) implementation of _kbhit() written by Morgan McGuire, morgan@cs.brown.edu.
  // Published at flipcode's "Developer Toolbox collection": http://www.flipcode.org/archives/_kbhit_for_Linux.shtml

  static const int STDIN = 0;
  static bool initialized = false;

  if (!initialized) {
    // Use termios to turn off line buffering
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, nullptr);
    initialized = true;
  }

  int bytesWaiting;
  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}

int LinuxConsole::getCharacter(bool bEcho) const {
  // This code comes from: http://cboard.cprogramming.com/archive/index.php/t-27714.html
  // "This code sets the terminal into non-canonical mode, thus disabling line buffering, reads a character from stdin
  //  and then restores the old terminal status. For more info on what else you can do with termios, see ``man termios''.
  //  There's also a ``getch()'' function in the curses library, but it is /not/ equivalent to the DOS ``getch()'' and
  //  may only be used within real curses applications (ie: it only works in curses ``WINDOW''s).."

  struct termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  struct termios newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  const int nCharacter = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  // Echo on the console?
  if (bEcho)
    putc(nCharacter, stdout);

  // Return the character
  return nCharacter;
}

void LinuxConsole::clearScreen() const {
  // Don't use the curses library for this, go the more portable way below
//	setupterm(nullptr, 1, nullptr);
//	putp(clear_screen); 

  // Another solution: Use ANSI-sequences to achieve the job
  printf("\033[H\033[J");
  fflush(stdout);
}

void LinuxConsole::getCursorPosition(uint16 &nX, uint16 &nY) const {
  // There's no native curses library on Android, and compiling one
  // just for this usually never used function would be overkill
#ifdef ANDROID
  // [TODO] Any solution using additional libraries?
  nX = 0;
  nY = 0;
#else
  // [TODO] Check whether this works...
  // Use curses library
  /*
  int x, y;
  setupterm(nullptr, 1, nullptr);
  getyx(stdscr, y, x);
  nX = x;
  nY = y;
  */
  nX = 0;
  nY = 0;
#endif
}

void LinuxConsole::setCursorPosition(uint16 nX, uint16 nY) const {
  // There's no native curses library on Android, and compiling one
  // just for this usually never used function would be overkill
#ifdef ANDROID
  // [TODO] Any solution using additional libraries?
  // Another solution: Use ANSI-sequences to achieve the job
  // ("\033" moves the cursor relative to it's current position... which is a problem because we need an absolute position change)
//	printf("\033[%d;%dH", nX, nY);
//	fflush(stdout);
#else
  // Use curses library
  setupterm(nullptr, 1, nullptr);
  putp(tparm(cursor_address, nY, nX));
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
