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
#include "RERHIOpenGL/Linux/RHIOpenGLContextLinux.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include <RERHI/Linux/X11Context.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {

static bool ctxErrorOccurred = false;
[[nodiscard]] static int ctxErrorHandler(Display*, XErrorEvent*)
{
  ctxErrorOccurred = true;
  return 0;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
OpenGLContextLinux::OpenGLContextLinux(RHIDynamicRHI& openGLRhi, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux) :
OpenGLContextLinux(openGLRhi, nullptr, depthStencilAttachmentTextureFormat, nativeWindowHandle, useExternalContext, shareContextLinux)
{}

OpenGLContextLinux::~OpenGLContextLinux()
{
  // Release the device context of the OpenGL window
  if (nullptr != mDisplay)
  {
    // Is the device context of the OpenGL window is the currently active OpenGL device context?
    if (glXGetCurrentContext() == mWindowRenderContext)
    {
      glXMakeCurrent(mDisplay, XLib::None, nullptr);
    }

    // Destroy the render context of the OpenGL window
    if (nullptr != mWindowRenderContext && mOwnsRenderContext)
    {
      glXDestroyContext(mDisplay, mWindowRenderContext);
    }

    if (mOwnsX11Display)
    {
      XCloseDisplay(mDisplay);
    }
  }
}

Display* OpenGLContextLinux::getDisplay() const
{
  return mDisplay;
}

GLXContext OpenGLContextLinux::getRenderContext() const
{
  return mWindowRenderContext;
}

bool OpenGLContextLinux::isInitialized() const
{
  return (nullptr != mWindowRenderContext || mUseExternalContext);
}

void OpenGLContextLinux::makeCurrent() const
{
  // Only do something when have created our RHI context and don't use a external RHI context
  if (!mUseExternalContext)
  {
    glXMakeCurrent(getDisplay(), mNativeWindowHandle, getRenderContext());
  }
}

OpenGLContextLinux::OpenGLContextLinux(RHIDynamicRHI& openGLRhi, OpenGLRuntimeLinking* openGLRuntimeLinking, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux) :
RHIOpenGLContext(openGLRuntimeLinking),
mOpenGLRhi(openGLRhi),
mNativeWindowHandle(nativeWindowHandle),
mDisplay(nullptr),
mOwnsX11Display(true),
mWindowRenderContext(nullptr),
mUseExternalContext(useExternalContext),
mOwnsRenderContext(true)
{
  if (mUseExternalContext)
  {
    // We use an external context so just load the OpenGL 3 entry points
    [[maybe_unused]] const bool result = loadOpenGL3EntryPoints();
  }
  else
  {
    const RERHI::RHIContext& context = openGLRhi.getContext();
    RHI_ASSERT(context.getType() == RERHI::RHIContext::ContextType::X11, "Invalid OpenGL context type")

    // If the given RHI context is an X11 context use the display connection object provided by the context
    if (context.getType() == RERHI::RHIContext::ContextType::X11)
    {
      mDisplay = static_cast<const RERHI::X11Context&>(context).getDisplay();
      mOwnsX11Display = mDisplay == nullptr;
    }

    if (mOwnsX11Display)
    {
      mDisplay = XOpenDisplay(0);
    }
  }
  if (nullptr != mDisplay)
  {
    // Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
    // "State" objects are not shared between contexts, including but not limited to:
    // - Vertex Array Objects (VAOs)
    // - Framebuffer Objects (FBOs)
    // -> Keep away from the share context parameter of "glxCreateContextAttribsARB()" and just share the OpenGL render context instead
    if (nullptr != shareContextLinux)
    {
      mWindowRenderContext = shareContextLinux->getRenderContext();
      mOwnsRenderContext = false;
    }
    else
    {
      // TODO(sw) We don't need a dummy context to load gl/glx entry points see "Misconception #2" from https://dri.freedesktop.org/wiki/glXGetProcAddressNeverReturnsNULL/
      // Load the >= OpenGL 3.0 entry points
      if (loadOpenGL3EntryPoints())
      {
        // Create the render context of the OpenGL window
        mWindowRenderContext = createOpenGLContext(depthStencilAttachmentTextureFormat);

        // If there's an OpenGL context, do some final initialization steps
        if (nullptr != mWindowRenderContext)
        {
          // Make the OpenGL context to the current one, native window handle can be zero -> thus only offscreen rendering is supported/wanted
          const int result = glXMakeCurrent(mDisplay, mNativeWindowHandle, mWindowRenderContext);
          RE_LOG(Debug, "Make new OpenGL context current: " + RECore::String(result))
          {
            int major = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major);

            int minor = 0;
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            GLint profile = 0;
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

            RE_LOG(Debug, "OpenGL context version: %d.%d %s" + RECore::to_string(major) + RECore::to_string(minor) + ((profile & GL_CONTEXT_CORE_PROFILE_BIT) ? "core" : "noncore"))
            int numberOfExtensions = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
            RE_LOG(Debug, "Number of supported OpenGL extensions: " + RECore::to_string(numberOfExtensions))
            for (GLuint extensionIndex = 0; extensionIndex < static_cast<GLuint>(numberOfExtensions); ++extensionIndex)
            {
              RE_LOG(Debug, RECore::String(">> ") + reinterpret_cast<char*>(glGetStringi(GL_EXTENSIONS, extensionIndex)))
            }
          }
        }
      }
      else
      {
        // Error, failed to load >= OpenGL 3 entry points!
      }
    }
  }
  else
  {
    // Error, failed to get display!
  }
}

GLXContext OpenGLContextLinux::createOpenGLContext(RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat)
{
#define GLX_CONTEXT_MAJOR_VERSION_ARB	0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB	0x2092

  // Get the available GLX extensions as string
  const char* extensions = glXQueryExtensionsString(mDisplay, XDefaultScreen(mDisplay));

  // Check whether or not "GLX_ARB_create_context" is a substring of the GLX extension string meaning that this OpenGL extension is supported
  if (nullptr != strstr(extensions, "GLX_ARB_create_context"))
  {
    // Get the OpenGL extension "glXCreateContextAttribsARB" function pointer
    typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
    GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = reinterpret_cast<GLXCREATECONTEXTATTRIBSARBPROC>(glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB"));
    if (nullptr != glXCreateContextAttribsARB)
    {
      // TODO(naetherm) Cleanup
      ctxErrorOccurred = false;
      int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

      // Create the OpenGL context
      // -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
      // TODO(naetherm) Add support for the "GL_KHR_no_error"-extension
      int ATTRIBUTES[] =
        {
          // We want an OpenGL context
          GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
          GLX_CONTEXT_MINOR_VERSION_ARB, 1,
          // -> "GLX_CONTEXT_DEBUG_BIT_ARB" comes from the "GL_ARB_debug_output"-extension
          GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
          // TODO(naetherm) Make it possible to activate "GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB" from the outside
#ifdef DEBUG
          //	GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
							//	GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,	// Error messages like "Implicit version number 110 not supported by GL3 forward compatible context" might occur
#else
          //	GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,	// Error messages like "Implicit version number 110 not supported by GL3 forward compatible context" might occur
#endif
          // Done
          0
        };

      // TODO(naetherm) Use more detailed color and depth/stencil information from render pass
      const int depthBufferBits = 24;//(RERHI::TextureFormat::Enum::UNKNOWN == depthStencilAttachmentTextureFormat) ? 24 : 24;
      int numberOfElements = 0;
      
      int visualAttributes[] =
        {
          GLX_RENDER_TYPE,	GLX_RGBA_BIT,
          GLX_DOUBLEBUFFER,	true,
          GLX_RED_SIZE,		8,
          GLX_GREEN_SIZE,		8,
          GLX_BLUE_SIZE,		8,
          GLX_ALPHA_SIZE,		8,
          GLX_DEPTH_SIZE,		depthBufferBits,
          GLX_STENCIL_SIZE,	8,
          XLib::None
        };
        
      /*
      int visualAttributes[] = {
        GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        GLX_DEPTH_SIZE, 16,
        0};  // = "None"
      */
      GLXFBConfig* fbc = glXChooseFBConfig(mDisplay, DefaultScreen(mDisplay), visualAttributes, &numberOfElements);
      RE_LOG(Debug, "Got %d of OpenGL GLXFBConfig" + RECore::to_string(numberOfElements))
      GLXContext glxContext = glXCreateContextAttribsARB(mDisplay, *fbc, 0, true, ATTRIBUTES);

      XSync(mDisplay, False);

      // TODO(sw) make this fallback optional (via an option)
      if (ctxErrorOccurred)
      {
        RE_LOG(Debug, "Could not create OpenGL 3+ context try creating pre 3+ context")
        ctxErrorOccurred = false;

        // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
        ATTRIBUTES[1] = 1;
        // GLX_CONTEXT_MINOR_VERSION_ARB = 0
        ATTRIBUTES[3] = 0;
        glxContext = glXCreateContextAttribsARB(mDisplay, *fbc, 0, true, ATTRIBUTES);

        // Synchronize to ensure any errors generated are processed
        XSync(mDisplay, False);

        // Restore the original error handler
        XSetErrorHandler(oldHandler);
      }

      if (nullptr != glxContext)
      {
        // Done
        RE_LOG(Debug, "OpenGL context with glXCreateContextAttribsARB created")
        return glxContext;
      }
      else
      {
        // Error, context creation failed!
        RE_LOG(Critical, "Could not create OpenGL context with glXCreateContextAttribsARB")
        return nullptr;
      }
    }
    else
    {
      // Error, failed to obtain the "GLX_ARB_create_context" function pointer (wow, something went terrible wrong!)
      RE_LOG(Critical, "Could not find OpenGL glXCreateContextAttribsARB")
      return nullptr;
    }
  }
  else
  {
    // Error, the OpenGL extension "GLX_ARB_create_context" is not supported... as a result we can't create an OpenGL context!
    RE_LOG(Critical, "OpenGL GLX_ARB_create_context not supported")
    return nullptr;
  }

#undef GLX_CONTEXT_MAJOR_VERSION_ARB
#undef GLX_CONTEXT_MINOR_VERSION_ARB
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
