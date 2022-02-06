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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"
#include "RERHIOpenGLES3/Extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    OpenGL ES 3 extensions runtime linking
*/
class ExtensionsRuntimeLinking final : public IExtensions
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  */
  inline explicit ExtensionsRuntimeLinking(RHIDynamicRHI& openGLES3Rhi) :
    mRHIDynamicRHI(openGLES3Rhi),
    // EXT
    mGL_EXT_texture_compression_s3tc(false),
    mGL_EXT_texture_compression_dxt1(false),
    mGL_EXT_texture_compression_latc(false),
    mGL_EXT_texture_buffer(false),
    mGL_EXT_draw_elements_base_vertex(false),
    mGL_EXT_base_instance(false),
    mGL_EXT_clip_control(false),
    // AMD
    mGL_AMD_compressed_3DC_texture(false),
    // NV
    mGL_NV_fbo_color_attachments(false),
    // OES
    mGL_OES_element_index_uint(false),
    mGL_OES_packed_depth_stencil(false),
    mGL_OES_depth24(false),
    mGL_OES_depth32(false),
    // KHR
    mGL_KHR_debug(false)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~ExtensionsRuntimeLinking() override
  {}

  /**
  *  @brief
  *    Initialize the supported extensions
  *
  *  @note
  *    - Do only call this method if the EGL functions initialization was successful ("glGetString()" is used)
  *      and there's an active render context
  */
  void initialize()
  {
    // Define a helper macro
#define IMPORT_FUNC(funcName)																															\
				if (result)																																			\
				{																																					\
					void* symbol = eglGetProcAddress(#funcName);																									\
					if (nullptr != symbol)																															\
					{																																				\
						*(reinterpret_cast<void**>(&(funcName))) = symbol;																							\
					}																																				\
					else																																			\
					{																																				\
						RE_LOG(Critical, std::string("Failed to locate the entry point _ within the OpenGL ES 3 shared library ") + #funcName)	\
						result = false;																																\
					}																																				\
				}

    // Get the extensions string and the OpenGL ES version
    const char* extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    GLint majorVersion = 0, minorVersion = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    //[-------------------------------------------------------]
    //[ EXT                                                   ]
    //[-------------------------------------------------------]
    // TODO(naetherm) Review whether or not those extensions are already inside the OpenGL ES 3 core
    mGL_EXT_texture_compression_s3tc = (nullptr != strstr(extensions, "GL_EXT_texture_compression_s3tc"));
    mGL_EXT_texture_compression_dxt1 = (nullptr != strstr(extensions, "GL_EXT_texture_compression_dxt1"));
    mGL_EXT_texture_compression_latc = (nullptr != strstr(extensions, "GL_EXT_texture_compression_latc"));

    // "GL_EXT_texture_buffer"
    // TODO(sw) Core in opengles 3.2
    //mGL_EXT_texture_buffer = (nullptr != strstr(extensions, "GL_EXT_texture_buffer"));
    // TODO(sw) Disabled for now. With mesa 17.1.3 the OpenGLES driver supports version 3.1 + texture buffer. But currently the shader of the Example project supports only the emulation path
    mGL_EXT_texture_buffer = false;
    if (mGL_EXT_texture_buffer)
    {
      // Load the entry points
      bool result = true;	// Success by default
      IMPORT_FUNC(glTexBufferEXT)
      mGL_EXT_texture_buffer = result;
    }

    // "GL_EXT_draw_elements_base_vertex" is part of OpenGL ES 3.2
    if (majorVersion >= 3 && minorVersion >= 2)
    {
#define FNDEF_EX(retType, funcName, args) retType (GL_APIENTRY *funcPtr_##funcName) args = nullptr
      FNDEF_EX(void,	glDrawElementsBaseVertex,			(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex));
      FNDEF_EX(void,	glDrawElementsInstancedBaseVertex,	(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex));
#define glDrawElementsBaseVertex			FNPTR(glDrawElementsBaseVertex)
#define glDrawElementsInstancedBaseVertex	FNPTR(glDrawElementsInstancedBaseVertex)

      bool result = true;	// Success by default
      IMPORT_FUNC(glDrawElementsBaseVertex)
      IMPORT_FUNC(glDrawElementsInstancedBaseVertex)
      mGL_EXT_draw_elements_base_vertex = result;
      glDrawElementsBaseVertexEXT = glDrawElementsBaseVertex;
      glDrawElementsInstancedBaseVertexEXT = glDrawElementsInstancedBaseVertex;

#undef FNDEF_EX
#undef glDrawElementsBaseVertex
#undef glDrawElementsInstancedBaseVertex
    }
    else
    {
      mGL_EXT_draw_elements_base_vertex = (nullptr != strstr(extensions, "GL_EXT_draw_elements_base_vertex"));
      if (mGL_EXT_draw_elements_base_vertex)
      {
        // Load the entry points
        bool result = true;	// Success by default
        IMPORT_FUNC(glDrawElementsBaseVertexEXT)
        IMPORT_FUNC(glDrawElementsInstancedBaseVertexEXT)
        mGL_EXT_draw_elements_base_vertex = result;
      }
    }

    // "GL_EXT_base_instance"
    mGL_EXT_base_instance = (nullptr != strstr(extensions, "GL_EXT_base_instance"));
    if (mGL_EXT_base_instance)
    {
      // Load the entry points
      bool result = true;	// Success by default
      IMPORT_FUNC(glDrawArraysInstancedBaseInstanceEXT)
      IMPORT_FUNC(glDrawElementsInstancedBaseInstanceEXT)
      IMPORT_FUNC(glDrawElementsInstancedBaseVertexBaseInstanceEXT)
      mGL_EXT_base_instance = result;
    }

    // "GL_EXT_clip_control"
    mGL_EXT_clip_control = (nullptr != strstr(extensions, "GL_EXT_clip_control"));
    if (mGL_EXT_clip_control)
    {
      // Load the entry points
      bool result = true;	// Success by default
      IMPORT_FUNC(glClipControlEXT)
      mGL_EXT_clip_control = result;
    }


    //[-------------------------------------------------------]
    //[ AMD                                                   ]
    //[-------------------------------------------------------]
    mGL_AMD_compressed_3DC_texture = (nullptr != strstr(extensions, "GL_AMD_compressed_3DC_texture"));

    //[-------------------------------------------------------]
    //[ NV                                                    ]
    //[-------------------------------------------------------]
    mGL_NV_fbo_color_attachments = (nullptr != strstr(extensions, "GL_NV_fbo_color_attachments"));

    //[-------------------------------------------------------]
    //[ OES                                                   ]
    //[-------------------------------------------------------]
    mGL_OES_element_index_uint	 = (nullptr != strstr(extensions, "GL_OES_element_index_uint"));
    mGL_OES_packed_depth_stencil = (nullptr != strstr(extensions, "GL_OES_packed_depth_stencil"));
    mGL_OES_depth24				 = (nullptr != strstr(extensions, "GL_OES_depth24"));
    mGL_OES_depth32				 = (nullptr != strstr(extensions, "GL_OES_depth32"));

    //[-------------------------------------------------------]
    //[ KHR                                                   ]
    //[-------------------------------------------------------]
    mGL_KHR_debug = (nullptr != strstr(extensions, "GL_KHR_debug"));
    if (mGL_KHR_debug)
    {
      // Load the entry points
      bool result = true;	// Success by default
      IMPORT_FUNC(glDebugMessageCallbackKHR)
      IMPORT_FUNC(glDebugMessageControlKHR)
      IMPORT_FUNC(glDebugMessageInsertKHR)
      IMPORT_FUNC(glPushDebugGroupKHR)
      IMPORT_FUNC(glPopDebugGroupKHR)
      IMPORT_FUNC(glObjectLabelKHR)
      mGL_KHR_debug = result;
    }

    // Undefine the helper macro
#undef IMPORT_FUNC
  }


  //[-------------------------------------------------------]
  //[ Public virtual RHIDynamicRHI::RHIExtensions methods      ]
  //[-------------------------------------------------------]
public:
  ///////////////////////////////////////////////////////////
  // Returns whether an extension is supported or not
  ///////////////////////////////////////////////////////////
  // EXT
  [[nodiscard]] inline virtual bool isGL_EXT_texture_compression_s3tc() const override
  {
    return mGL_EXT_texture_compression_s3tc;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_texture_compression_dxt1() const override
  {
    return mGL_EXT_texture_compression_dxt1;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_texture_compression_latc() const override
  {
    return mGL_EXT_texture_compression_latc;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_texture_buffer() const override
  {
    return mGL_EXT_texture_buffer;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_draw_elements_base_vertex() const override
  {
    return mGL_EXT_draw_elements_base_vertex;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_base_instance() const override
  {
    return mGL_EXT_base_instance;
  }

  [[nodiscard]] inline virtual bool isGL_EXT_clip_control() const override
  {
    return mGL_EXT_clip_control;
  }

  // AMD
  [[nodiscard]] inline virtual bool isGL_AMD_compressed_3DC_texture() const override
  {
    return mGL_AMD_compressed_3DC_texture;
  }

  // NV
  [[nodiscard]] inline virtual bool isGL_NV_fbo_color_attachments() const override
  {
    return mGL_NV_fbo_color_attachments;
  }

  // OES
  [[nodiscard]] inline virtual bool isGL_OES_element_index_uint() const override
  {
    return mGL_OES_element_index_uint;
  }

  [[nodiscard]] inline virtual bool isGL_OES_packed_depth_stencil() const override
  {
    return mGL_OES_packed_depth_stencil;
  }

  [[nodiscard]] inline virtual bool isGL_OES_depth24() const override
  {
    return mGL_OES_depth24;
  }

  [[nodiscard]] inline virtual bool isGL_OES_depth32() const override
  {
    return mGL_OES_depth32;
  }

  // KHR
  [[nodiscard]] inline virtual bool isGL_KHR_debug() const override
  {
    return mGL_KHR_debug;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ExtensionsRuntimeLinking(const ExtensionsRuntimeLinking& source) = delete;
  ExtensionsRuntimeLinking& operator =(const ExtensionsRuntimeLinking& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI& mRHIDynamicRHI;	///< Owner OpenGL ES 3 RHI instance
  // EXT
  bool mGL_EXT_texture_compression_s3tc;
  bool mGL_EXT_texture_compression_dxt1;
  bool mGL_EXT_texture_compression_latc;
  bool mGL_EXT_texture_buffer;
  bool mGL_EXT_draw_elements_base_vertex;
  bool mGL_EXT_base_instance;
  bool mGL_EXT_clip_control;
  // AMD
  bool mGL_AMD_compressed_3DC_texture;
  // NV
  bool mGL_NV_fbo_color_attachments;
  // OES
  bool mGL_OES_element_index_uint;
  bool mGL_OES_packed_depth_stencil;
  bool mGL_OES_depth24;
  bool mGL_OES_depth32;
  // KHR
  bool mGL_KHR_debug;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
