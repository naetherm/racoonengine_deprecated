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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGL/RERHIOpenGL.h"
#include <RECore/System/DynLib.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL runtime linking
*/
class OpenGLRuntimeLinking final
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class RHIOpenGLContext;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  */
  explicit OpenGLRuntimeLinking(RHIDynamicRHI& openGLRhi);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~OpenGLRuntimeLinking();

  /**
  *  @brief
  *    Return whether or not OpenGL is available
  *
  *  @return
  *    "true" if OpenGL is available, else "false"
  */
  [[nodiscard]] bool isOpenGLAvaiable();


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit OpenGLRuntimeLinking(const OpenGLRuntimeLinking& source) = delete;
  OpenGLRuntimeLinking& operator =(const OpenGLRuntimeLinking& source) = delete;

  /**
  *  @brief
  *    Load the shared libraries
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadSharedLibraries();

  /**
  *  @brief
  *    Load the OpenGL entry points
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadOpenGLEntryPoints();

  /**
  *  @brief
  *    Load the >= OpenGL 3.0 entry points
  *
  *  @return
  *    "true" if all went fine, else "false"
  *
  *  @note
  *    - This method is only allowed to be called after an >= OpenGL context has been created and set
  */
  [[nodiscard]] bool loadOpenGL3EntryPoints();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI&	mOpenGLRhi;					///< Owner OpenGL RHI instance
  void*		mOpenGLSharedLibrary;		///< OpenGL shared library, can be a null pointer
  bool		mOwnsOpenGLSharedLibrary;	///< Indicates if the OpenGL shared library was loaded from ourself or provided from external
  bool		mEntryPointsRegistered;		///< Entry points successfully registered?
  bool		mInitialized;				///< Already initialized?


};



//[-------------------------------------------------------]
//[ OpenGL functions                                      ]
//[-------------------------------------------------------]

#ifdef GL_DEFINE_RUNTIMELINKING
#define FNDEF_GL(retType, funcName, args) retType (GLAPIENTRY *funcPtr_##funcName) args
#else
#define FNDEF_GL(retType, funcName, args) extern retType (GLAPIENTRY *funcPtr_##funcName) args
#endif

FNDEF_GL(GLubyte*, glGetString, (GLenum));
FNDEF_GL(void, glGetIntegerv, (GLenum, GLint *));
FNDEF_GL(void, glBindTexture, (GLenum, GLuint));
FNDEF_GL(void, glClear, (GLbitfield));
FNDEF_GL(void, glClearStencil, (GLint));
FNDEF_GL(void, glClearDepth, (GLclampd));
FNDEF_GL(void, glClearColor, (GLclampf, GLclampf, GLclampf, GLclampf));
FNDEF_GL(void, glDrawArrays, (GLenum, GLint, GLsizei));
FNDEF_GL(void, glDrawElements, (GLenum, GLsizei, GLenum, const GLvoid*));
FNDEF_GL(void, glColor4f, (GLfloat, GLfloat, GLfloat, GLfloat));
FNDEF_GL(void, glEnable, (GLenum));
FNDEF_GL(void, glDisable, (GLenum));
FNDEF_GL(void, glBlendFunc, (GLenum, GLenum));
FNDEF_GL(void, glFrontFace, (GLenum));
FNDEF_GL(void, glCullFace, (GLenum));
FNDEF_GL(void, glPolygonMode, (GLenum, GLenum));
FNDEF_GL(void, glTexParameteri, (GLenum, GLenum, GLint));
FNDEF_GL(void, glGenTextures, (GLsizei, GLuint *));
FNDEF_GL(void, glDeleteTextures, (GLsizei, const GLuint*));
FNDEF_GL(void, glTexImage1D, (GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid*));
FNDEF_GL(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*));
FNDEF_GL(void, glPixelStorei, (GLenum, GLint));
FNDEF_GL(void, glDepthFunc, (GLenum func));
FNDEF_GL(void, glDepthMask, (GLboolean));
FNDEF_GL(void, glViewport, (GLint, GLint, GLsizei, GLsizei));
FNDEF_GL(void, glDepthRange, (GLclampd, GLclampd));
FNDEF_GL(void, glScissor, (GLint, GLint, GLsizei, GLsizei));
FNDEF_GL(void, glFlush, (void));
FNDEF_GL(void, glFinish, (void));
// >= OpenGL 3.0
FNDEF_GL(GLubyte *, glGetStringi, (GLenum, GLuint));
// >= OpenGL 4.5
FNDEF_GL(void, glCreateQueries, (GLenum, GLsizei, GLuint *));

// Platform specific
#if defined(WIN32)
FNDEF_GL(HDC,	wglGetCurrentDC,	(VOID));
FNDEF_GL(PROC,	wglGetProcAddress,	(LPCSTR));
FNDEF_GL(HGLRC,	wglCreateContext,	(HDC));
FNDEF_GL(BOOL,	wglDeleteContext,	(HGLRC));
FNDEF_GL(BOOL,	wglMakeCurrent,		(HDC, HGLRC));
#elif defined(LINUX)

FNDEF_GL(Bool, glXMakeCurrent, (Display * , GLXDrawable, GLXContext));
FNDEF_GL(XVisualInfo*, glXChooseVisual, (Display * , int, int *));
FNDEF_GL(GLXContext, glXCreateContext, (Display * , XVisualInfo *, GLXContext, int));
FNDEF_GL(void, glXDestroyContext, (Display * , GLXContext));
FNDEF_GL(GLXContext, glXGetCurrentContext, (void));
FNDEF_GL(const char*, glXQueryExtensionsString, (Display * , int));
FNDEF_GL(__GLXextFuncPtr, glXGetProcAddress, (const GLubyte*));
FNDEF_GL(__GLXextFuncPtr, glXGetProcAddressARB, (const GLubyte*));
FNDEF_GL(GLXFBConfig*, glXChooseFBConfig, (Display * , int, const int*, int*));
FNDEF_GL(void, glXSwapBuffers, (Display * , GLXDrawable));
FNDEF_GL(const char*, glXGetClientString, (Display * , int));

#else
#error "Unsupported platform"
#endif

// Redirect gl* function calls to funcPtr_gl*
#ifndef FNPTR
#define FNPTR(name) funcPtr_##name
#endif

// OpenGL
#define glGetString      FNPTR(glGetString)
#define glGetIntegerv    FNPTR(glGetIntegerv)
#define glBindTexture    FNPTR(glBindTexture)
#define glClear        FNPTR(glClear)
#define glClearStencil    FNPTR(glClearStencil)
#define glClearDepth    FNPTR(glClearDepth)
#define glClearColor    FNPTR(glClearColor)
#define glDrawArrays    FNPTR(glDrawArrays)
#define glDrawElements    FNPTR(glDrawElements)
#define glColor4f      FNPTR(glColor4f)
#define glEnable      FNPTR(glEnable)
#define glDisable      FNPTR(glDisable)
#define glBlendFunc      FNPTR(glBlendFunc)
#define glFrontFace      FNPTR(glFrontFace)
#define glCullFace      FNPTR(glCullFace)
#define glPolygonMode    FNPTR(glPolygonMode)
#define glTexParameteri    FNPTR(glTexParameteri)
#define glGenTextures    FNPTR(glGenTextures)
#define glDeleteTextures  FNPTR(glDeleteTextures)
#define glTexImage1D    FNPTR(glTexImage1D)
#define glTexImage2D    FNPTR(glTexImage2D)
#define glPixelStorei    FNPTR(glPixelStorei)
#define glDepthFunc      FNPTR(glDepthFunc)
#define glDepthMask      FNPTR(glDepthMask)
#define glViewport      FNPTR(glViewport)
#define glDepthRange    FNPTR(glDepthRange)
#define glScissor      FNPTR(glScissor)
#define glFlush        FNPTR(glFlush)
#define glFinish      FNPTR(glFinish)

// >= OpenGL 3.0
#define glGetStringi FNPTR(glGetStringi)

// >= OpenGL 4.5
#define glCreateQueries FNPTR(glCreateQueries)

// Platform specific
#ifdef _WIN32
#define wglGetCurrentDC		FNPTR(wglGetCurrentDC)
#define wglGetProcAddress	FNPTR(wglGetProcAddress)
#define wglCreateContext	FNPTR(wglCreateContext)
#define wglDeleteContext	FNPTR(wglDeleteContext)
#define wglMakeCurrent		FNPTR(wglMakeCurrent)
#elif LINUX
#define glXMakeCurrent        FNPTR(glXMakeCurrent)
#define glXGetProcAddress      FNPTR(glXGetProcAddress)
#define glXGetProcAddressARB    FNPTR(glXGetProcAddressARB)
#define glXChooseVisual        FNPTR(glXChooseVisual)
#define glXCreateContext      FNPTR(glXCreateContext)
#define glXDestroyContext      FNPTR(glXDestroyContext)
#define glXGetCurrentContext    FNPTR(glXGetCurrentContext)
#define glXQueryExtensionsString  FNPTR(glXQueryExtensionsString)
#define glXChooseFBConfig      FNPTR(glXChooseFBConfig)
#define glXSwapBuffers        FNPTR(glXSwapBuffers)
#define glXGetClientString      FNPTR(glXGetClientString)
#else
#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
