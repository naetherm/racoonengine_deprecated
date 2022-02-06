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
#define GL_DEFINE_RUNTIMELINKING

#include "RERHIOpenGL/OpenGLRuntimeLinking.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
//#undef GL_DEFINE_RUNTIMELINKING


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


// Define a helper macro
#ifdef _WIN32
#define IMPORT_FUNC(funcName)																																			\
    if (result)																																							\
    {																																									\
      void* symbol = ::GetProcAddress(static_cast<HMODULE>(mOpenGLSharedLibrary), #funcName);																			\
      if (nullptr == symbol)																																			\
      {																																								\
        symbol = wglGetProcAddress(#funcName);																														\
      }																																								\
      if (nullptr != symbol)																																			\
      {																																								\
        *(reinterpret_cast<void**>(&(funcName))) = symbol;																											\
      }																																								\
      else																																							\
      {																																								\
        wchar_t moduleFilename[MAX_PATH];																															\
        moduleFilename[0] = '\0';																																	\
        ::GetModuleFileNameW(static_cast<HMODULE>(mOpenGLSharedLibrary), moduleFilename, MAX_PATH);																	\
        RE_LOG(Critical, "Failed to locate the entry point \"%s\" within the OpenGL shared library \"%s\"", #funcName, moduleFilename)	\
        result = false;																																				\
      }																																								\
    }
#elif LINUX
#define IMPORT_FUNC(funcName)                                                                    \
    if (result)                                                                            \
    {                                                                                \
      void* symbol = ::dlsym(mOpenGLSharedLibrary, #funcName);                                                  \
      if (nullptr != symbol)                                                                    \
      {                                                                              \
        *(reinterpret_cast<void**>(&(funcName))) = symbol;                                                    \
      }                                                                              \
      else                                                                            \
      { \
        RE_LOG(Critical, std::string("Failed to locate the entry point \"") + #funcName)  \
        result = false;                                                                      \
      }                                                                              \
    }
#else
#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
OpenGLRuntimeLinking::OpenGLRuntimeLinking(RHIDynamicRHI &openGLRhi) :
  mOpenGLRhi(openGLRhi),
  mOpenGLSharedLibrary(mOpenGLRhi.getContext().getRhiApiSharedLibrary()),
  mOwnsOpenGLSharedLibrary(true),
//    nullptr == mOpenGLSharedLibrary),  // We can do this here because "mOpenGLSharedLibrary" lays before this variable
  mEntryPointsRegistered(false),
  mInitialized(false) {}

OpenGLRuntimeLinking::~OpenGLRuntimeLinking() {
  if (mOwnsOpenGLSharedLibrary) {
    // Destroy the shared library instances
#ifdef _WIN32
    if (nullptr != mOpenGLSharedLibrary)
          {
            ::FreeLibrary(static_cast<HMODULE>(mOpenGLSharedLibrary));
          }
#elif LINUX
    //if (nullptr != mOpenGLSharedLibrary) {
    //  ::dlclose(mOpenGLSharedLibrary);
    //}
    if (mOpenGLSharedLibrary) {
      bool res = ::dlclose(mOpenGLSharedLibrary);
    }
#else
#error "Unsupported platform"
#endif
  }
}

bool OpenGLRuntimeLinking::isOpenGLAvaiable() {
  // Already initialized?
  if (!mInitialized) {
    // We're now initialized
    mInitialized = true;

    // Load the shared libraries
    if (loadSharedLibraries()) {
      // Load the OpenGL entry points
      mEntryPointsRegistered = loadOpenGLEntryPoints();
    }
  }

  // Entry points successfully registered?
  return mEntryPointsRegistered;
}

bool OpenGLRuntimeLinking::loadSharedLibraries() {
  if (mOwnsOpenGLSharedLibrary) {
    // Load the shared library
#ifdef _WIN32
    mOpenGLSharedLibrary = ::LoadLibraryExA("opengl32.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
          if (nullptr == mOpenGLSharedLibrary)
          {
            RE_LOG(Critical, "Failed to load in the shared OpenGL library \"opengl32.dll\"")
          }
#elif LINUX
    mOpenGLSharedLibrary = ::dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
    if (mOpenGLSharedLibrary == nullptr) {
      RE_LOG(Critical, "Failed to load in the shared OpenGL library \"libGL.so\"")
    }
#else
#error "Unsupported platform"
#endif
  }

  // Done
  return (mOpenGLSharedLibrary != nullptr);
}

bool OpenGLRuntimeLinking::loadOpenGLEntryPoints() {
  bool result = true;  // Success by default

  // Load the entry points
  IMPORT_FUNC(glGetString)
  IMPORT_FUNC(glGetIntegerv)
  IMPORT_FUNC(glBindTexture)
  IMPORT_FUNC(glClear)
  IMPORT_FUNC(glClearStencil)
  IMPORT_FUNC(glClearDepth)
  IMPORT_FUNC(glClearColor)
  IMPORT_FUNC(glDrawArrays)
  IMPORT_FUNC(glDrawElements)
  IMPORT_FUNC(glColor4f)
  IMPORT_FUNC(glEnable)
  IMPORT_FUNC(glDisable)
  IMPORT_FUNC(glBlendFunc)
  IMPORT_FUNC(glFrontFace)
  IMPORT_FUNC(glCullFace)
  IMPORT_FUNC(glPolygonMode)
  IMPORT_FUNC(glTexParameteri)
  IMPORT_FUNC(glGenTextures)
  IMPORT_FUNC(glDeleteTextures)
  IMPORT_FUNC(glTexImage1D)
  IMPORT_FUNC(glTexImage2D)
  IMPORT_FUNC(glPixelStorei)
  IMPORT_FUNC(glDepthFunc)
  IMPORT_FUNC(glDepthMask)
  IMPORT_FUNC(glViewport)
  IMPORT_FUNC(glDepthRange)
  IMPORT_FUNC(glScissor)
  IMPORT_FUNC(glFlush)
  IMPORT_FUNC(glFinish)
#ifdef _WIN32
    IMPORT_FUNC(wglGetCurrentDC)
          IMPORT_FUNC(wglGetProcAddress)
          IMPORT_FUNC(wglCreateContext)
          IMPORT_FUNC(wglDeleteContext)
          IMPORT_FUNC(wglMakeCurrent)
#elif LINUX
  IMPORT_FUNC(glXMakeCurrent)
  IMPORT_FUNC(glXGetProcAddress)
  IMPORT_FUNC(glXGetProcAddressARB)
  IMPORT_FUNC(glXChooseVisual)
  IMPORT_FUNC(glXCreateContext)
  IMPORT_FUNC(glXDestroyContext)
  IMPORT_FUNC(glXGetCurrentContext)
  IMPORT_FUNC(glXQueryExtensionsString)
  IMPORT_FUNC(glXChooseFBConfig)
  IMPORT_FUNC(glXSwapBuffers)
  IMPORT_FUNC(glXGetClientString)
#else
#error "Unsupported platform"
#endif

  // Done
  return result;
}

bool OpenGLRuntimeLinking::loadOpenGL3EntryPoints() {
  bool result = true;  // Success by default

  // Optional >= OpenGL 4.5: Load the entry points
  IMPORT_FUNC(glCreateQueries)

  // Mandatory >= OpenGL 3.0: Load the entry points
  result = true;  // Success by default
  IMPORT_FUNC(glGetStringi)

  // Done
  return result;
}

#undef IMPORT_FUNC


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
