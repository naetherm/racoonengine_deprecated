/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#define GLES3_DEFINE_RUNTIMELINKING
#include "RERHIOpenGLES3/OpenGLES3ContextRuntimeLinking.h"
#include "RERHIOpenGLES3/ExtensionsRuntimeLinking.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


OpenGLES3ContextRuntimeLinking::OpenGLES3ContextRuntimeLinking(RHIDynamicRHI& openGLES3Rhi, RECore::handle nativeWindowHandle, bool useExternalContext) :
IOpenGLES3Context(openGLES3Rhi, nativeWindowHandle, useExternalContext),
mRHIDynamicRHI(openGLES3Rhi),
mEGLSharedLibrary(nullptr),
mGLESSharedLibrary(nullptr),
mEntryPointsRegistered(false),
mExtensions(RHI_NEW(openGLES3Rhi.getContext(), ExtensionsRuntimeLinking)(openGLES3Rhi))
{
  // Load the shared libraries
  if (loadSharedLibraries())
  {
    // Load the EGL entry points
    if (loadEGLEntryPoints())
    {
      // Load the OpenGL ES 3 entry points
      if (loadGLESEntryPoints())
      {
        // Entry points successfully registered
        mEntryPointsRegistered = true;
      }
      else
      {
        RE_LOG(Critical, "Failed to load in the OpenGL ES 3 entry points")
      }
    }
    else
    {
      RE_LOG(Critical, "Failed to load in the OpenGL ES 3 EGL entry points")
    }
  }
  else
  {
    RE_LOG(Critical, "Failed to load in the OpenGL ES 3 shared libraries")
  }
}

OpenGLES3ContextRuntimeLinking::~OpenGLES3ContextRuntimeLinking()
{
// De-initialize the context while we still can
deinitialize();

// Destroy the extensions instance
RHI_DELETE(mRHIDynamicRHI.getContext(), ExtensionsRuntimeLinking, mExtensions);

// Destroy the shared library instances
#ifdef _WIN32
if (nullptr != mEGLSharedLibrary)
				{
					::FreeLibrary(static_cast<HMODULE>(mEGLSharedLibrary));
				}
				if (nullptr != mGLESSharedLibrary)
				{
					::FreeLibrary(static_cast<HMODULE>(mGLESSharedLibrary));
				}
#elif defined LINUX
if (nullptr != mEGLSharedLibrary)
{
::dlclose(mEGLSharedLibrary);
}
if (nullptr != mGLESSharedLibrary)
{
::dlclose(mGLESSharedLibrary);
}
#else
#error "Unsupported platform"
#endif
}

const IExtensions& OpenGLES3ContextRuntimeLinking::getExtensions() const
{
return *mExtensions;
}

bool OpenGLES3ContextRuntimeLinking::initialize(RECore::uint32 multisampleAntialiasingSamples)
{
// Entry points successfully registered?
if (mEntryPointsRegistered)
{
// Call base implementation
if (IOpenGLES3Context::initialize(multisampleAntialiasingSamples))
{
// Initialize the supported extensions
mExtensions->initialize();

// Done
return true;
}
}

// Error!
return false;
}

EGLConfig OpenGLES3ContextRuntimeLinking::chooseConfig(RECore::uint32 multisampleAntialiasingSamples) const
{
// Try to find a working EGL configuration
EGLConfig eglConfig = nullptr;
EGLint numberOfConfigurations = 0;
bool chooseConfigCapitulated = false;
EGLint multisampleAntialiasingSamplesCurrent = static_cast<EGLint>(multisampleAntialiasingSamples);
do
{
// Get the current multisample antialiasing settings
const bool multisampleAntialiasing = (multisampleAntialiasingSamplesCurrent > 1);	// Multisample antialiasing with just one sample per pixel isn't real multisample, is it? :D
// const EGLint multisampleAntialiasingSampleBuffers = multisampleAntialiasing ? 1 : 0;

// Set desired configuration
const EGLint configAttribs[] =
  {
    EGL_LEVEL,				0,										// Frame buffer level
    EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,							// Which types of EGL surfaces are supported
    EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES3_BIT_KHR,					// Which client APIs are supported
    EGL_RED_SIZE,			8,										// Bits of red color component
    EGL_GREEN_SIZE,			8,										// Bits of red color component
    EGL_BLUE_SIZE,			8,										// Bits of red color component
    EGL_DEPTH_SIZE,			16,										// Bits of Z in the depth buffer TODO(naetherm) Make it possible to set this from the outside, but do also automatically go down if it fails, e.g. 24 doesn't work for me
    // TODO(naetherm) Currently something looks wrong when using the desktop drivers - just black screen when using multisample ("AMD Catalyst 11.8" on a "ATI Mobility Radeon HD 4850")
    //       -> No issues with Android on the device (but it looks like there's no antialiasing, check it later in detail)
    //	EGL_SAMPLE_BUFFERS,		multisampleAntialiasingSampleBuffers,	// Number of multisample buffers (enable/disable multisample antialiasing)
    //	EGL_SAMPLES,			multisampleAntialiasingSamplesCurrent,	// Number of samples per pixel (multisample antialiasing samples)
    EGL_NONE
  };

// Choose exactly one matching configuration
if (eglChooseConfig(mEGLDisplay, configAttribs, &eglConfig, 1, &numberOfConfigurations) == EGL_FALSE || numberOfConfigurations < 1)
{
// Can we change something on the multisample antialiasing? (may be the cause that no configuration was found!)
if (multisampleAntialiasing)
{
if (multisampleAntialiasingSamplesCurrent > 8)
{
multisampleAntialiasingSamplesCurrent = 8;
}
else if (multisampleAntialiasingSamplesCurrent > 4)
{
multisampleAntialiasingSamplesCurrent = 4;
}
else if (multisampleAntialiasingSamplesCurrent > 2)
{
multisampleAntialiasingSamplesCurrent = 2;
}
else if (2 == multisampleAntialiasingSamplesCurrent)
{
multisampleAntialiasingSamplesCurrent = 0;
}
}
else
{
// Don't mind, forget it...
chooseConfigCapitulated = true;
}
}
} while (numberOfConfigurations < 1 && !chooseConfigCapitulated);

// Done
return eglConfig;
}

bool OpenGLES3ContextRuntimeLinking::loadSharedLibraries()
{
  // We don't need to check m_pEGLSharedLibrary and m_pGLESSharedLibrary at this point because we know they must contain a null pointer

  // EGL and OpenGL ES 3 may be within a single shared library, or within two separate shared libraries
#ifdef _WIN32
  // First, try the OpenGL ES 3 emulator from ARM (it's possible to move around this dll without issues, so, this one first)
				mEGLSharedLibrary = ::LoadLibraryExA("libEGL.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
				if (nullptr != mEGLSharedLibrary)
				{
					mGLESSharedLibrary = ::LoadLibraryExA("libGLESv2.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
				}
				else
				{
					// Second, try the AMD/ATI driver
					mEGLSharedLibrary = ::LoadLibraryExA("atioglxx.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
					if (nullptr != mEGLSharedLibrary)
					{
						mGLESSharedLibrary = ::LoadLibraryExA("atioglxx.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
					}
					else
					{
						// Third, try the NVIDIA driver
						mEGLSharedLibrary = ::LoadLibraryExA("nvoglv32.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
						if (nullptr != mEGLSharedLibrary)
						{
							mGLESSharedLibrary = ::LoadLibraryExA("nvoglv32.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
						}
					}
				}
#elif defined __ANDROID__
  // On Android we have "libEGL.so"
				mEGLSharedLibrary = ::dlopen("libEGL.so", RTLD_LAZY);
				if (nullptr != mEGLSharedLibrary)
				{
					mGLESSharedLibrary = ::dlopen("libGLESv2.so", RTLD_LAZY);
				}
#elif defined LINUX
  // First "libGL.so": The closed source drivers doesn't provide separate libraries for GLES and EGL (at least the drivers from AMD)
  // but the separate EGL/GLES3 libs might be present on the system
  mEGLSharedLibrary = ::dlopen("libGL.so", RTLD_LAZY);
  if (nullptr != mEGLSharedLibrary)
  {
    // Try finding the eglGetProcAddress to determine if this library contains EGL/GLES support.
    // This check is needed because only the closed source drivers have the EGL/GLES support in "libGL.so".
    // The open source drivers (mesa) have separate libraries for this and they can be present on the system even the closed source drivers are used.
    void* symbol = ::dlsym(mEGLSharedLibrary, "eglGetProcAddress");
    if (nullptr != symbol)
    {
      mGLESSharedLibrary = ::dlopen("libGL.so", RTLD_LAZY);
    }
    else
    {
      // Unload the library
      ::dlclose(mEGLSharedLibrary);
      mEGLSharedLibrary = nullptr;
    }
  }
  if (nullptr == mEGLSharedLibrary)
  {
    // Then we try the separate libs for EGL and GLES (provided either by an emulator or native from mesa)
    mEGLSharedLibrary = ::dlopen("libEGL.so", RTLD_LAZY);
    if (nullptr != mEGLSharedLibrary)
    {
      mGLESSharedLibrary = ::dlopen("libGLESv2.so", RTLD_LAZY);
    }
  }
#else
#error "Unsupported platform"
#endif

  // Done
  return (nullptr != mEGLSharedLibrary && nullptr != mGLESSharedLibrary);
}

bool OpenGLES3ContextRuntimeLinking::loadEGLEntryPoints()
{
  bool result = true;	// Success by default

  // Define a helper macro
#ifdef _WIN32
  #define IMPORT_FUNC(funcName)																																						\
					if (result)																																										\
					{																																												\
						void* symbol = ::GetProcAddress(static_cast<HMODULE>(mEGLSharedLibrary), #funcName);																						\
						if (nullptr == symbol)																																						\
						{																																											\
							/* The specification states that "eglGetProcAddress" is only for extension functions, but when using OpenGL ES 3 on desktop PC by using a								\
							   native OpenGL ES 3 capable graphics driver under Linux (tested with "AMD Catalyst 11.8"), only this way will work */													\
							if (nullptr != eglGetProcAddress)																																		\
							{																																										\
								symbol = eglGetProcAddress(#funcName);																																\
							}																																										\
						}																																											\
						if (nullptr != symbol)																																						\
						{																																											\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																														\
						}																																											\
						else																																										\
						{																																											\
							wchar_t moduleFilename[MAX_PATH];																																		\
							moduleFilename[0] = '\0';																																				\
							::GetModuleFileNameW(static_cast<HMODULE>(mEGLSharedLibrary), moduleFilename, MAX_PATH);																				\
							RHI_LOG(mRHIDynamicRHI.getContext(), CRITICAL, "Failed to locate the OpenGL ES 3 entry point \"%s\" within the EGL shared library \"%s\"", #funcName, moduleFilename)	\
							result = false;																																							\
						}																																											\
					}
#elif defined(__ANDROID__)
  #define IMPORT_FUNC(funcName)																																					\
					if (result)																																									\
					{																																											\
						void* symbol = ::dlsym(mEGLSharedLibrary, #funcName);																													\
						if (nullptr == symbol)																																					\
						{																																										\
							/* The specification states that "eglGetProcAddress" is only for extension functions, but when using OpenGL ES 3 on desktop PC by using a							\
							   native OpenGL ES 3 capable graphics driver under Linux (tested with "AMD Catalyst 11.8"), only this way will work */												\
							if (nullptr != eglGetProcAddress)																																	\
							{																																									\
								symbol = eglGetProcAddress(#funcName);																															\
							}																																									\
						}																																										\
						if (nullptr != symbol)																																					\
						{																																										\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																													\
						}																																										\
						else																																									\
						{																																										\
							const char* libraryName = "unknown";																																\
							RHI_LOG(mRHIDynamicRHI.getContext(), CRITICAL, "Failed to locate the OpenGL ES 3 entry point \"%s\" within the EGL shared library \"%s\"", #funcName, libraryName)	\
							result = false;																																						\
						}																																										\
					}
#elif defined LINUX
#define IMPORT_FUNC(funcName)																																					\
					if (result)																																									\
					{																																											\
						void* symbol = ::dlsym(mEGLSharedLibrary, #funcName);																													\
						if (nullptr == symbol)																																					\
						{																																										\
							/* The specification states that "eglGetProcAddress" is only for extension functions, but when using OpenGL ES 3 on desktop PC by using a							\
							   native OpenGL ES 3 capable graphics driver under Linux (tested with "AMD Catalyst 11.8"), only this way will work */												\
							if (nullptr != eglGetProcAddress)																																	\
							{																																									\
								symbol = eglGetProcAddress(#funcName);																															\
							}																																									\
						}																																										\
						if (nullptr != symbol)																																					\
						{																																										\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																													\
						}																																										\
						else																																									\
						{																																										\
							link_map* linkMap = nullptr;																																		\
							const char* libraryName = "unknown";																																\
							if (dlinfo(mEGLSharedLibrary, RTLD_DI_LINKMAP, &linkMap))																											\
							{																																									\
								libraryName = linkMap->l_name;																																	\
							}																																									\
							libraryName = libraryName; /* To avoid -Wunused-but-set-variable warning when RHI_LOG is defined empty */															\
							RE_LOG(Critical, RECore::String("Failed to locate the OpenGL ES 3 entry point _ within the EGL shared library _ ") + #funcName)	\
							result = false;																																						\
						}																																										\
					}
#else
#error "Unsupported platform"
#endif

  // Load the entry points
  IMPORT_FUNC(eglGetProcAddress)
  IMPORT_FUNC(eglGetError)
  IMPORT_FUNC(eglGetDisplay)
  IMPORT_FUNC(eglInitialize)
  IMPORT_FUNC(eglTerminate)
  IMPORT_FUNC(eglQueryString)
  IMPORT_FUNC(eglGetConfigs)
  IMPORT_FUNC(eglChooseConfig)
  IMPORT_FUNC(eglGetConfigAttrib)
  IMPORT_FUNC(eglCreateWindowSurface)
  IMPORT_FUNC(eglDestroySurface)
  IMPORT_FUNC(eglQuerySurface)
  IMPORT_FUNC(eglBindAPI)
  IMPORT_FUNC(eglQueryAPI)
  IMPORT_FUNC(eglWaitClient)
  IMPORT_FUNC(eglReleaseThread)
  IMPORT_FUNC(eglSurfaceAttrib)
  IMPORT_FUNC(eglBindTexImage)
  IMPORT_FUNC(eglReleaseTexImage)
  IMPORT_FUNC(eglSwapInterval)
  IMPORT_FUNC(eglCreateContext)
  IMPORT_FUNC(eglDestroyContext)
  IMPORT_FUNC(eglMakeCurrent)
  IMPORT_FUNC(eglGetCurrentContext)
  IMPORT_FUNC(eglGetCurrentSurface)
  IMPORT_FUNC(eglGetCurrentDisplay)
  IMPORT_FUNC(eglQueryContext)
  IMPORT_FUNC(eglWaitGL)
  IMPORT_FUNC(eglWaitNative)
  IMPORT_FUNC(eglSwapBuffers)
  IMPORT_FUNC(eglCopyBuffers)

  // Undefine the helper macro
#undef IMPORT_FUNC

  // Done
  return result;

}

bool OpenGLES3ContextRuntimeLinking::loadGLESEntryPoints()
{
  bool result = true;	// Success by default

  // Define a helper macro
#ifdef __ANDROID__
  // Native OpenGL ES 3 on mobile device
				#define IMPORT_FUNC(funcName)																														\
					if (result)																																		\
					{																																				\
						void* symbol = ::dlsym(mGLESSharedLibrary, #funcName);																						\
						if (nullptr != symbol)																														\
						{																																			\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																						\
						}																																			\
						else																																		\
						{																																			\
							result = false;																															\
						}																																			\
					}
#else
  // Native OpenGL ES 3 on desktop PC, we need an function entry point work around for this
#define IMPORT_FUNC(funcName)																																\
					if (result)																																				\
					{																																						\
						/* The specification states that "eglGetProcAddress" is only for extension functions, but when using OpenGL ES 3 on desktop PC by using a			\
						   native OpenGL ES 3 capable graphics driver (tested with "AMD Catalyst 11.8"), only this way will work */											\
						void* symbol = eglGetProcAddress(#funcName);																										\
						if (nullptr != symbol)																																\
						{																																					\
							*(reinterpret_cast<void**>(&(funcName))) = symbol;																								\
						}																																					\
						else																																				\
						{																																					\
							RE_LOG(Critical, RECore::String("Failed to locate the OpenGL ES 3 entry point _ within the GLES shared library ") + #funcName)	\
							result = false;																																	\
						}																																					\
					}
#endif

  // Load the entry points
  IMPORT_FUNC(glActiveTexture)
  IMPORT_FUNC(glAttachShader)
  IMPORT_FUNC(glBindAttribLocation)
  IMPORT_FUNC(glBindBuffer)
  IMPORT_FUNC(glBindFramebuffer)
  IMPORT_FUNC(glBindRenderbuffer)
  IMPORT_FUNC(glBindTexture)
  IMPORT_FUNC(glBlendColor)
  IMPORT_FUNC(glBlendEquation)
  IMPORT_FUNC(glBlendEquationSeparate)
  IMPORT_FUNC(glBlendFunc)
  IMPORT_FUNC(glBlendFuncSeparate)
  IMPORT_FUNC(glBufferData)
  IMPORT_FUNC(glBufferSubData)
  IMPORT_FUNC(glCheckFramebufferStatus)
  IMPORT_FUNC(glClear)
  IMPORT_FUNC(glClearColor)
  IMPORT_FUNC(glClearDepthf)
  IMPORT_FUNC(glClearStencil)
  IMPORT_FUNC(glColorMask)
  IMPORT_FUNC(glCompileShader)
  IMPORT_FUNC(glCompressedTexImage2D)
  IMPORT_FUNC(glCompressedTexImage3D)
  IMPORT_FUNC(glCompressedTexSubImage2D)
  IMPORT_FUNC(glCopyTexImage2D)
  IMPORT_FUNC(glCopyTexSubImage2D)
  IMPORT_FUNC(glCreateProgram)
  IMPORT_FUNC(glCreateShader)
  IMPORT_FUNC(glCullFace)
  IMPORT_FUNC(glDeleteBuffers)
  IMPORT_FUNC(glDeleteFramebuffers)
  IMPORT_FUNC(glDeleteProgram)
  IMPORT_FUNC(glDeleteRenderbuffers)
  IMPORT_FUNC(glDeleteShader)
  IMPORT_FUNC(glDeleteTextures)
  IMPORT_FUNC(glDepthFunc)
  IMPORT_FUNC(glDepthMask)
  IMPORT_FUNC(glDepthRangef)
  IMPORT_FUNC(glDetachShader)
  IMPORT_FUNC(glDisable)
  IMPORT_FUNC(glDisableVertexAttribArray)
  IMPORT_FUNC(glDrawArrays)
  IMPORT_FUNC(glDrawArraysInstanced)
  IMPORT_FUNC(glDrawElements)
  IMPORT_FUNC(glDrawElementsInstanced)
  IMPORT_FUNC(glEnable)
  IMPORT_FUNC(glEnableVertexAttribArray)
  IMPORT_FUNC(glFinish)
  IMPORT_FUNC(glFlush)
  IMPORT_FUNC(glFramebufferRenderbuffer)
  IMPORT_FUNC(glFramebufferTexture2D)
  IMPORT_FUNC(glFramebufferTextureLayer)
  IMPORT_FUNC(glBlitFramebuffer)
  IMPORT_FUNC(glFrontFace)
  IMPORT_FUNC(glGenBuffers)
  IMPORT_FUNC(glGenerateMipmap)
  IMPORT_FUNC(glGenFramebuffers)
  IMPORT_FUNC(glGenRenderbuffers)
  IMPORT_FUNC(glGenTextures)
  IMPORT_FUNC(glGetActiveAttrib)
  IMPORT_FUNC(glGetActiveUniform)
  IMPORT_FUNC(glGetAttachedShaders)
  IMPORT_FUNC(glGetAttribLocation)
  IMPORT_FUNC(glGetBooleanv)
  IMPORT_FUNC(glGetBufferParameteriv)
  IMPORT_FUNC(glGetError)
  IMPORT_FUNC(glGetFloatv)
  IMPORT_FUNC(glGetFramebufferAttachmentParameteriv)
  IMPORT_FUNC(glGetIntegerv)
  IMPORT_FUNC(glGetProgramiv)
  IMPORT_FUNC(glGetProgramInfoLog)
  IMPORT_FUNC(glGetRenderbufferParameteriv)
  IMPORT_FUNC(glGetShaderiv)
  IMPORT_FUNC(glGetShaderInfoLog)
  IMPORT_FUNC(glGetShaderPrecisionFormat)
  IMPORT_FUNC(glGetShaderSource)
  IMPORT_FUNC(glGetString)
  IMPORT_FUNC(glGetTexParameterfv)
  IMPORT_FUNC(glGetTexParameteriv)
  IMPORT_FUNC(glGetUniformfv)
  IMPORT_FUNC(glGetUniformiv)
  IMPORT_FUNC(glGetUniformLocation)
  IMPORT_FUNC(glGetUniformBlockIndex)
  IMPORT_FUNC(glUniformBlockBinding)
  IMPORT_FUNC(glGetVertexAttribfv)
  IMPORT_FUNC(glGetVertexAttribiv)
  IMPORT_FUNC(glGetVertexAttribPointerv)
  IMPORT_FUNC(glHint)
  IMPORT_FUNC(glIsBuffer)
  IMPORT_FUNC(glIsEnabled)
  IMPORT_FUNC(glIsFramebuffer)
  IMPORT_FUNC(glIsProgram)
  IMPORT_FUNC(glIsRenderbuffer)
  IMPORT_FUNC(glIsShader)
  IMPORT_FUNC(glIsTexture)
  IMPORT_FUNC(glLineWidth)
  IMPORT_FUNC(glLinkProgram)
  IMPORT_FUNC(glPixelStorei)
  IMPORT_FUNC(glPolygonOffset)
  IMPORT_FUNC(glReadPixels)
  IMPORT_FUNC(glReleaseShaderCompiler)
  IMPORT_FUNC(glRenderbufferStorage)
  IMPORT_FUNC(glSampleCoverage)
  IMPORT_FUNC(glScissor)
  IMPORT_FUNC(glShaderBinary)
  IMPORT_FUNC(glShaderSource)
  IMPORT_FUNC(glStencilFunc)
  IMPORT_FUNC(glStencilFuncSeparate)
  IMPORT_FUNC(glStencilMask)
  IMPORT_FUNC(glStencilMaskSeparate)
  IMPORT_FUNC(glStencilOp)
  IMPORT_FUNC(glStencilOpSeparate)
  IMPORT_FUNC(glTexImage2D)
  IMPORT_FUNC(glTexImage3D)
  IMPORT_FUNC(glTexParameterf)
  IMPORT_FUNC(glTexParameterfv)
  IMPORT_FUNC(glTexParameteri)
  IMPORT_FUNC(glTexParameteriv)
  IMPORT_FUNC(glTexSubImage2D)
  IMPORT_FUNC(glUniform1f)
  IMPORT_FUNC(glUniform1fv)
  IMPORT_FUNC(glUniform1i)
  IMPORT_FUNC(glUniform1iv)
  IMPORT_FUNC(glUniform1ui)
  IMPORT_FUNC(glUniform2f)
  IMPORT_FUNC(glUniform2fv)
  IMPORT_FUNC(glUniform2i)
  IMPORT_FUNC(glUniform2iv)
  IMPORT_FUNC(glUniform3f)
  IMPORT_FUNC(glUniform3fv)
  IMPORT_FUNC(glUniform3i)
  IMPORT_FUNC(glUniform3iv)
  IMPORT_FUNC(glUniform4f)
  IMPORT_FUNC(glUniform4fv)
  IMPORT_FUNC(glUniform4i)
  IMPORT_FUNC(glUniform4iv)
  IMPORT_FUNC(glUniformMatrix2fv)
  IMPORT_FUNC(glUniformMatrix3fv)
  IMPORT_FUNC(glUniformMatrix4fv)
  IMPORT_FUNC(glUseProgram)
  IMPORT_FUNC(glValidateProgram)
  IMPORT_FUNC(glVertexAttrib1f)
  IMPORT_FUNC(glVertexAttrib1fv)
  IMPORT_FUNC(glVertexAttrib2f)
  IMPORT_FUNC(glVertexAttrib2fv)
  IMPORT_FUNC(glVertexAttrib3f)
  IMPORT_FUNC(glVertexAttrib3fv)
  IMPORT_FUNC(glVertexAttrib4f)
  IMPORT_FUNC(glVertexAttrib4fv)
  IMPORT_FUNC(glVertexAttribPointer)
  IMPORT_FUNC(glVertexAttribIPointer)
  IMPORT_FUNC(glVertexAttribDivisor)
  IMPORT_FUNC(glViewport)
  IMPORT_FUNC(glBindBufferBase)
  IMPORT_FUNC(glUnmapBuffer)
  IMPORT_FUNC(glMapBufferRange)
  IMPORT_FUNC(glDrawBuffers)
  IMPORT_FUNC(glTexImage3D)
  IMPORT_FUNC(glTexSubImage3D)
  IMPORT_FUNC(glCopyTexSubImage3D)
  IMPORT_FUNC(glCompressedTexSubImage3D)
  IMPORT_FUNC(glGetBufferPointerv)
  IMPORT_FUNC(glBindVertexArray)
  IMPORT_FUNC(glDeleteVertexArrays)
  IMPORT_FUNC(glGenVertexArrays)

  // Undefine the helper macro
#undef IMPORT_FUNC

  // Done
  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
