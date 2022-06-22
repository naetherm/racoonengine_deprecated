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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline Pipe::Pipe() {
  // Initialize handles
  mPipe[0] = INVALID_HANDLE;
  mPipe[1] = INVALID_HANDLE;
}

inline Pipe::Pipe(const Pipe &rhs) {
  // Copy Pipe
  mName = rhs.mName;
  mPipe[0] = rhs.mPipe[0];
  mPipe[1] = rhs.mPipe[1];
}

inline Pipe::~Pipe() {
}


inline Pipe &Pipe::operator=(const Pipe &rhs) {
  // Copy Pipe
  mName = rhs.mName;
  mPipe[0] = rhs.mPipe[0];
  mPipe[1] = rhs.mPipe[1];
  return *this;
}

inline bool Pipe::operator==(const Pipe &rhs) const {
  // Compare
  return (mName == rhs.mName && mPipe[0] == rhs.mPipe[0] && mPipe[1] == rhs.mPipe[1]);
}

inline bool Pipe::operator!=(const Pipe &rhs) const {
  // Compare
  return (mName != rhs.mName || mPipe[0] != rhs.mPipe[0] || mPipe[1] != rhs.mPipe[1]);
}


inline bool Pipe::open(handle hRead, handle hWrite) {
  // Save file handles
  mPipe[0] = hRead;
  mPipe[1] = hWrite;

  // Done
  return true;
}

inline bool Pipe::close() {
  // Close read and write
  return (closeRead() && closeWrite());
}

inline String Pipe::getName() const {
  // Return name
  return mName;
}

inline handle Pipe::getReadHandle() const {
  // Return read-end
  return mPipe[0];
}

inline handle Pipe::getWriteHandle() const {
  // Return write-end
  return mPipe[1];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
