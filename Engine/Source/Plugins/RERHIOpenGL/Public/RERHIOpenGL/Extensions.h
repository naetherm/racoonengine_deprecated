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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGL/RERHIOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class RHIOpenGLContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Supported OpenGL graphic card extensions
*
*  @remarks
*    You must check if the extension is supported by the current hardware before
*    you use it. If the extension isn't available you should offer an alternative
*    technique aka fallback.
*
*  @see
*    - OpenGL extension registry at http://oss.sgi.com/projects/ogl-sample/registry/ for more information about
*      the different extensions
*/
class Extensions final
{


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
  *  @param[in] openGLContext
  *    Owner OpenGL context
  */
  Extensions(RHIDynamicRHI& openGLRhi, RHIOpenGLContext& openGLContext);

  /**
  *  @brief
  *    Destructor
  *
  *  @note
  *    - Not virtual by intent
  */
  ~Extensions() = default;

  /**
  *  @brief
  *    Returns whether the extensions are initialized or not
  *
  *  @return
  *    "true" if the extension are initialized, else "false"
  */
  [[nodiscard]] inline bool isInitialized() const
  {
    return mInitialized;
  }

  ///////////////////////////////////////////////////////////
  // Returns whether an extension is supported or not
  ///////////////////////////////////////////////////////////
  // WGL (Windows only)
  [[nodiscard]] inline bool isWGL_ARB_extensions_string() const
  {
    return mWGL_ARB_extensions_string;
  }

  [[nodiscard]] inline bool isWGL_EXT_swap_control() const
  {
    return mWGL_EXT_swap_control;
  }

  [[nodiscard]] inline bool isWGL_EXT_swap_control_tear() const
  {
    return mWGL_EXT_swap_control_tear;
  }

  // NV
  [[nodiscard]] inline bool isGL_NV_mesh_shader() const
  {
    return mGL_NV_mesh_shader;
  }

  // EXT
  [[nodiscard]] inline bool isGL_EXT_texture_lod_bias() const
  {
    return mGL_EXT_texture_lod_bias;
  }

  [[nodiscard]] inline bool isGL_EXT_texture_filter_anisotropic() const
  {
    return mGL_EXT_texture_filter_anisotropic;
  }

  [[nodiscard]] inline bool isGL_EXT_texture_array() const
  {
    return mGL_EXT_texture_array;
  }

  [[nodiscard]] inline bool isGL_EXT_texture3D() const
  {
    return mGL_EXT_texture3D;
  }

  [[nodiscard]] inline bool isGL_EXT_direct_state_access() const
  {
    return mGL_EXT_direct_state_access;
  }

  [[nodiscard]] inline bool isGL_EXT_shader_image_load_store() const
  {
    return mGL_EXT_shader_image_load_store;
  }

  // KHR
  [[nodiscard]] inline bool isGL_KHR_debug() const
  {
    return mGL_KHR_debug;
  }

  // ARB
  [[nodiscard]] inline bool isGL_ARB_framebuffer_object() const
  {
    return mGL_ARB_framebuffer_object;
  }

  [[nodiscard]] inline bool isGL_ARB_multitexture() const
  {
    return mGL_ARB_multitexture;
  }

  [[nodiscard]] inline bool isGL_ARB_texture_multisample() const
  {
    return mGL_ARB_texture_multisample;
  }

  [[nodiscard]] inline bool isGL_ARB_vertex_buffer_object() const
  {
    return mGL_ARB_vertex_buffer_object;
  }

  [[nodiscard]] inline bool isGL_ARB_texture_compression() const
  {
    return mGL_ARB_texture_compression;
  }

  [[nodiscard]] inline bool isGL_ARB_draw_buffers() const
  {
    return mGL_ARB_draw_buffers;
  }

  [[nodiscard]] inline bool isGL_ARB_shader_objects() const
  {
    return mGL_ARB_shader_objects;
  }

  [[nodiscard]] inline bool isGL_ARB_separate_shader_objects() const
  {
    return mGL_ARB_separate_shader_objects;
  }

  [[nodiscard]] inline bool isGL_ARB_get_program_binary() const
  {
    return mGL_ARB_get_program_binary;
  }

  [[nodiscard]] inline bool isGL_ARB_uniform_buffer_object() const
  {
    return mGL_ARB_uniform_buffer_object;
  }

  [[nodiscard]] inline bool isGL_ARB_texture_buffer_object() const
  {
    return mGL_ARB_texture_buffer_object;
  }

  [[nodiscard]] inline bool isGL_ARB_draw_indirect() const
  {
    return mGL_ARB_draw_indirect;
  }

  [[nodiscard]] inline bool isGL_ARB_multi_draw_indirect() const
  {
    return mGL_ARB_multi_draw_indirect;
  }

  [[nodiscard]] inline bool isGL_ARB_vertex_shader() const
  {
    return mGL_ARB_vertex_shader;
  }

  [[nodiscard]] inline bool isGL_ARB_vertex_program() const
  {
    return mGL_ARB_vertex_program;
  }

  [[nodiscard]] inline bool isGL_ARB_tessellation_shader() const
  {
    return mGL_ARB_tessellation_shader;
  }

  [[nodiscard]] inline bool isGL_ARB_geometry_shader4() const
  {
    return mGL_ARB_geometry_shader4;
  }

  [[nodiscard]] inline bool isGL_ARB_fragment_shader() const
  {
    return mGL_ARB_fragment_shader;
  }

  [[nodiscard]] inline bool isGL_ARB_fragment_program() const
  {
    return mGL_ARB_fragment_program;
  }

  [[nodiscard]] inline bool isGL_ARB_compute_shader() const
  {
    return mGL_ARB_compute_shader;
  }

  [[nodiscard]] inline bool isGL_ARB_draw_instanced() const
  {
    return mGL_ARB_draw_instanced;
  }

  [[nodiscard]] inline bool isGL_ARB_base_instance() const
  {
    return mGL_ARB_base_instance;
  }

  [[nodiscard]] inline bool isGL_ARB_instanced_arrays() const
  {
    return mGL_ARB_instanced_arrays;
  }

  [[nodiscard]] inline bool isGL_ARB_vertex_array_object() const
  {
    return mGL_ARB_vertex_array_object;
  }

  [[nodiscard]] inline bool isGL_ARB_sampler_objects() const
  {
    return mGL_ARB_sampler_objects;
  }

  [[nodiscard]] inline bool isGL_ARB_draw_elements_base_vertex() const
  {
    return mGL_ARB_draw_elements_base_vertex;
  }

  [[nodiscard]] inline bool isGL_ARB_debug_output() const
  {
    return mGL_ARB_debug_output;
  }

  [[nodiscard]] inline bool isGL_ARB_direct_state_access() const
  {
    return mGL_ARB_direct_state_access;
  }

  [[nodiscard]] inline bool isGL_ARB_texture_storage() const
  {
    return mGL_ARB_texture_storage;
  }

  [[nodiscard]] inline bool isGL_ARB_shader_storage_buffer_object() const
  {
    return mGL_ARB_shader_storage_buffer_object;
  }

  [[nodiscard]] inline bool isGL_ARB_copy_image() const
  {
    return mGL_ARB_copy_image;
  }

  [[nodiscard]] inline bool isGL_ARB_gl_spirv() const
  {
    return mGL_ARB_gl_spirv;
  }

  [[nodiscard]] inline bool isGL_ARB_clip_control() const
  {
    return mGL_ARB_clip_control;
  }

  [[nodiscard]] inline bool isGL_ARB_occlusion_query() const
  {
    return mGL_ARB_occlusion_query;
  }

  [[nodiscard]] inline bool isGL_ARB_pipeline_statistics_query() const
  {
    return mGL_ARB_pipeline_statistics_query;
  }

  [[nodiscard]] inline bool isGL_ARB_timer_query() const
  {
    return mGL_ARB_timer_query;
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
public:
  explicit Extensions(const Extensions& source) = delete;
  Extensions& operator =(const Extensions& source) = delete;

  /**
  *  @brief
  *    Checks whether an extension is supported by the given hardware or not
  *
  *  @param[in] extension
  *    ASCII name of the extension, if a null pointer, nothing happens
  *
  *  @return
  *    "true" if the extensions is supported, else "false"
  */
  [[nodiscard]] bool isSupported(const char* extension) const;

  /**
  *  @brief
  *    Checks whether an extension is supported by the given hardware or not
  *
  *  @param[in] extension
  *    ASCII name of the extension, if a null pointer, nothing happens
  *
  *  @return
  *    "true" if the extensions is supported, else "false"
  */
  [[nodiscard]] bool checkExtension(const char* extension) const;

  /**
  *  @brief
  *    Resets the extensions
  */
  void resetExtensions();

  /**
  *  @brief
  *    Initialize the supported extensions
  *
  *  @param[in] useExtensions
  *    Use extensions?
  *
  *  @return
  *    "true" if all went fine, else "false"
  *
  *  @note
  *    - Platform dependent implementation
  */
  [[nodiscard]] bool initialize(bool useExtensions = true);

  /**
  *  @brief
  *    Initialize the supported universal extensions
  *
  *  @return
  *    "true" if all went fine, else "false"
  *
  *  @note
  *    - Platform independent implementation
  */
  [[nodiscard]] bool initializeUniversal();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI&		mOpenGLRhi;		///< Owner OpenGL RHI instance
  RHIOpenGLContext* mOpenGLContext;	///< Owner OpenGL context, always valid!
  bool			mInitialized;	///< Are the extensions initialized?

  // Supported extensions
  // WGL (Windows only)
  bool mWGL_ARB_extensions_string;
  bool mWGL_EXT_swap_control;
  bool mWGL_EXT_swap_control_tear;
  // EXT
  bool mGL_NV_mesh_shader;
  // EXT
  bool mGL_EXT_texture_lod_bias;
  bool mGL_EXT_texture_filter_anisotropic;
  bool mGL_EXT_texture_array;
  bool mGL_EXT_texture3D;
  bool mGL_EXT_direct_state_access;
  bool mGL_EXT_shader_image_load_store;
  // KHR
  bool mGL_KHR_debug;
  // ARB
  bool mGL_ARB_framebuffer_object;
  bool mGL_ARB_multitexture;
  bool mGL_ARB_texture_multisample;
  bool mGL_ARB_vertex_buffer_object;
  bool mGL_ARB_texture_compression;
  bool mGL_ARB_draw_buffers;
  bool mGL_ARB_shader_objects;
  bool mGL_ARB_separate_shader_objects;
  bool mGL_ARB_get_program_binary;
  bool mGL_ARB_uniform_buffer_object;
  bool mGL_ARB_texture_buffer_object;
  bool mGL_ARB_draw_indirect;
  bool mGL_ARB_multi_draw_indirect;
  bool mGL_ARB_vertex_shader;
  bool mGL_ARB_vertex_program;
  bool mGL_ARB_tessellation_shader;
  bool mGL_ARB_geometry_shader4;
  bool mGL_ARB_fragment_shader;
  bool mGL_ARB_fragment_program;
  bool mGL_ARB_compute_shader;
  bool mGL_ARB_draw_instanced;
  bool mGL_ARB_base_instance;
  bool mGL_ARB_instanced_arrays;
  bool mGL_ARB_vertex_array_object;
  bool mGL_ARB_sampler_objects;
  bool mGL_ARB_draw_elements_base_vertex;
  bool mGL_ARB_debug_output;
  bool mGL_ARB_direct_state_access;
  bool mGL_ARB_texture_storage;
  bool mGL_ARB_shader_storage_buffer_object;
  bool mGL_ARB_copy_image;
  bool mGL_ARB_gl_spirv;
  bool mGL_ARB_clip_control;
  bool mGL_ARB_occlusion_query;
  bool mGL_ARB_pipeline_statistics_query;
  bool mGL_ARB_timer_query;


};




//[-------------------------------------------------------]
//[ OpenGL functions                                      ]
//[-------------------------------------------------------]
#ifdef GL_DEFINE_EXTENSIONS
#define FNDEF_EX(funcName, funcTypedef) funcTypedef funcName = nullptr
#else
#define FNDEF_EX(funcName, funcTypedef) extern funcTypedef funcName
#endif

//[-------------------------------------------------------]
//[ WGL (Windows only) definitions                        ]
//[-------------------------------------------------------]
#if defined(WIN32)
// WGL_ARB_extensions_string
	FNDEF_EX(wglGetExtensionsStringARB,	PFNWGLGETEXTENSIONSSTRINGARBPROC);

	// WGL_EXT_swap_control
	FNDEF_EX(wglSwapIntervalEXT,	PFNWGLSWAPINTERVALEXTPROC);
#endif

//[-------------------------------------------------------]
//[ NV                                                    ]
//[-------------------------------------------------------]
// GL_NV_mesh_shader
FNDEF_EX(glDrawMeshTasksNV,	PFNGLDRAWMESHTASKSNVPROC);

//[-------------------------------------------------------]
//[ EXT                                                   ]
//[-------------------------------------------------------]
// GL_EXT_texture3D
FNDEF_EX(glTexImage3DEXT, PFNGLTEXIMAGE3DEXTPROC);
FNDEF_EX(glTexSubImage3DEXT, PFNGLTEXSUBIMAGE3DEXTPROC);

// GL_EXT_direct_state_access
FNDEF_EX(glNamedBufferDataEXT, PFNGLNAMEDBUFFERDATAEXTPROC);
FNDEF_EX(glNamedBufferSubDataEXT, PFNGLNAMEDBUFFERSUBDATAEXTPROC);
FNDEF_EX(glMapNamedBufferEXT, PFNGLMAPNAMEDBUFFEREXTPROC);
FNDEF_EX(glUnmapNamedBufferEXT, PFNGLUNMAPNAMEDBUFFEREXTPROC);
FNDEF_EX(glProgramUniform1iEXT, PFNGLPROGRAMUNIFORM1IEXTPROC);
FNDEF_EX(glProgramUniform1uiEXT, PFNGLPROGRAMUNIFORM1UIEXTPROC);
FNDEF_EX(glProgramUniform1fEXT, PFNGLPROGRAMUNIFORM1FEXTPROC);
FNDEF_EX(glProgramUniform2fvEXT, PFNGLPROGRAMUNIFORM2FVEXTPROC);
FNDEF_EX(glProgramUniform3fvEXT, PFNGLPROGRAMUNIFORM3FVEXTPROC);
FNDEF_EX(glProgramUniform4fvEXT, PFNGLPROGRAMUNIFORM4FVEXTPROC);
FNDEF_EX(glProgramUniformMatrix3fvEXT, PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC);
FNDEF_EX(glProgramUniformMatrix4fvEXT, PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC);
FNDEF_EX(glTextureImage1DEXT, PFNGLTEXTUREIMAGE1DEXTPROC);
FNDEF_EX(glTextureImage2DEXT, PFNGLTEXTUREIMAGE2DEXTPROC);
FNDEF_EX(glTextureImage3DEXT, PFNGLTEXTUREIMAGE3DEXTPROC);
FNDEF_EX(glTextureSubImage3DEXT, PFNGLTEXTURESUBIMAGE3DEXTPROC);
FNDEF_EX(glTextureParameteriEXT, PFNGLTEXTUREPARAMETERIEXTPROC);
FNDEF_EX(glGenerateTextureMipmapEXT, PFNGLGENERATETEXTUREMIPMAPEXTPROC);
FNDEF_EX(glCompressedTextureImage1DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC);
FNDEF_EX(glCompressedTextureImage2DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC);
FNDEF_EX(glCompressedTextureImage3DEXT, PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC);
FNDEF_EX(glVertexArrayVertexAttribOffsetEXT, PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC);
FNDEF_EX(glEnableVertexArrayAttribEXT, PFNGLENABLEVERTEXARRAYATTRIBEXTPROC);
FNDEF_EX(glBindMultiTextureEXT, PFNGLBINDMULTITEXTUREEXTPROC);
FNDEF_EX(glNamedFramebufferTexture2DEXT, PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC);
FNDEF_EX(glNamedFramebufferTextureLayerEXT, PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC);
FNDEF_EX(glCheckNamedFramebufferStatusEXT, PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC);
FNDEF_EX(glNamedRenderbufferStorageEXT, PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC);
FNDEF_EX(glNamedFramebufferRenderbufferEXT, PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC);

// GL_EXT_shader_image_load_store
FNDEF_EX(glBindImageTextureEXT, PFNGLBINDIMAGETEXTUREEXTPROC);
FNDEF_EX(glMemoryBarrierEXT, PFNGLMEMORYBARRIEREXTPROC);

//[-------------------------------------------------------]
//[ KHR                                                   ]
//[-------------------------------------------------------]
// GL_KHR_debug
FNDEF_EX(glDebugMessageInsert, PFNGLDEBUGMESSAGEINSERTPROC);
FNDEF_EX(glPushDebugGroup, PFNGLPUSHDEBUGGROUPPROC);
FNDEF_EX(glPopDebugGroup, PFNGLPOPDEBUGGROUPPROC);
FNDEF_EX(glObjectLabel, PFNGLOBJECTLABELPROC);

//[-------------------------------------------------------]
//[ ARB                                                   ]
//[-------------------------------------------------------]
// GL_ARB_framebuffer_object
FNDEF_EX(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
FNDEF_EX(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
FNDEF_EX(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
FNDEF_EX(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);
FNDEF_EX(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
FNDEF_EX(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
FNDEF_EX(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
FNDEF_EX(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC);
FNDEF_EX(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC);
FNDEF_EX(glFramebufferTextureLayer, PFNGLFRAMEBUFFERTEXTURELAYERPROC);
FNDEF_EX(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
FNDEF_EX(glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC);
FNDEF_EX(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC);

// GL_ARB_multitexture
FNDEF_EX(glActiveTextureARB, PFNGLACTIVETEXTUREARBPROC);

// GL_ARB_texture_multisample
FNDEF_EX(glTexImage2DMultisample, PFNGLTEXIMAGE2DMULTISAMPLEPROC);

// GL_ARB_vertex_buffer_object
FNDEF_EX(glBindBufferARB, PFNGLBINDBUFFERARBPROC);
FNDEF_EX(glDeleteBuffersARB, PFNGLDELETEBUFFERSARBPROC);
FNDEF_EX(glGenBuffersARB, PFNGLGENBUFFERSARBPROC);
FNDEF_EX(glBufferDataARB, PFNGLBUFFERDATAARBPROC);
FNDEF_EX(glBufferSubDataARB, PFNGLBUFFERSUBDATAARBPROC);
FNDEF_EX(glMapBufferARB, PFNGLMAPBUFFERARBPROC);
FNDEF_EX(glUnmapBufferARB, PFNGLUNMAPBUFFERARBPROC);

// GL_ARB_texture_compression
FNDEF_EX(glCompressedTexImage1DARB, PFNGLCOMPRESSEDTEXIMAGE1DARBPROC);
FNDEF_EX(glCompressedTexImage2DARB, PFNGLCOMPRESSEDTEXIMAGE2DARBPROC);
FNDEF_EX(glCompressedTexImage3DARB, PFNGLCOMPRESSEDTEXIMAGE3DARBPROC);

// GL_ARB_vertex_program
FNDEF_EX(glVertexAttribPointerARB, PFNGLVERTEXATTRIBPOINTERARBPROC);
FNDEF_EX(glVertexAttribIPointer, PFNGLVERTEXATTRIBIPOINTERPROC);  // GL_NV_vertex_program4
FNDEF_EX(glEnableVertexAttribArrayARB, PFNGLENABLEVERTEXATTRIBARRAYARBPROC);
FNDEF_EX(glDisableVertexAttribArrayARB, PFNGLDISABLEVERTEXATTRIBARRAYARBPROC);

// GL_ARB_draw_buffers
FNDEF_EX(glDrawBuffersARB, PFNGLDRAWBUFFERSARBPROC);

// GL_ARB_shader_objects
FNDEF_EX(glDeleteShader, PFNGLDELETESHADERPROC);      // glDeleteObjectARB
FNDEF_EX(glGetHandleARB, PFNGLGETHANDLEARBPROC);
FNDEF_EX(glDetachShader, PFNGLDETACHSHADERPROC);      // glDetachObjectARB
FNDEF_EX(glCreateShader, PFNGLCREATESHADERPROC);      // glCreateShaderObjectARB
FNDEF_EX(glShaderSource, PFNGLSHADERSOURCEPROC);      // glShaderSourceARB
FNDEF_EX(glCompileShader, PFNGLCOMPILESHADERPROC);    // glCompileShaderARB
FNDEF_EX(glCreateProgram, PFNGLCREATEPROGRAMPROC);    // glCreateProgramObjectARB
FNDEF_EX(glAttachShader, PFNGLATTACHSHADERPROC);      // glAttachObjectARB
FNDEF_EX(glLinkProgram, PFNGLLINKPROGRAMPROC);      // glLinkProgramARB
FNDEF_EX(glUseProgram, PFNGLUSEPROGRAMPROC);      // glUseProgramObjectARB
FNDEF_EX(glUniform1f, PFNGLUNIFORM1FPROC);      // glUniform1fARB
FNDEF_EX(glUniform1i, PFNGLUNIFORM1IPROC);      // glUniform1iARB
FNDEF_EX(glUniform2fv, PFNGLUNIFORM2FVPROC);      // glUniform2fvARB
FNDEF_EX(glUniform3fv, PFNGLUNIFORM3FVPROC);      // glUniform3fvARB
FNDEF_EX(glUniform4fv, PFNGLUNIFORM4FVPROC);      // glUniform4fvARB
FNDEF_EX(glUniformMatrix3fv, PFNGLUNIFORMMATRIX3FVPROC);    // glUniformMatrix3fvARB
FNDEF_EX(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);    // glUniformMatrix4fvARB
FNDEF_EX(glGetShaderiv, PFNGLGETSHADERIVPROC);      // glGetObjectParameterivARB
FNDEF_EX(glGetProgramiv, PFNGLGETPROGRAMIVPROC);      // glGetObjectParameterivARB
FNDEF_EX(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);    // glGetInfoLogARB
FNDEF_EX(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);  // glGetInfoLogARB
FNDEF_EX(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);  // glGetUniformLocationARB

// GL_ARB_separate_shader_objects
FNDEF_EX(glCreateShaderProgramv, PFNGLCREATESHADERPROGRAMVPROC);
FNDEF_EX(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
FNDEF_EX(glGenProgramPipelines, PFNGLGENPROGRAMPIPELINESPROC);
FNDEF_EX(glDeleteProgramPipelines, PFNGLDELETEPROGRAMPIPELINESPROC);
FNDEF_EX(glBindProgramPipeline, PFNGLBINDPROGRAMPIPELINEPROC);
FNDEF_EX(glUseProgramStages, PFNGLUSEPROGRAMSTAGESPROC);
FNDEF_EX(glValidateProgramPipeline, PFNGLVALIDATEPROGRAMPIPELINEPROC);
FNDEF_EX(glGetProgramPipelineiv, PFNGLGETPROGRAMPIPELINEIVPROC);
FNDEF_EX(glGetProgramPipelineInfoLog, PFNGLGETPROGRAMPIPELINEINFOLOGPROC);
FNDEF_EX(glActiveShaderProgram, PFNGLACTIVESHADERPROGRAMPROC);

// GL_ARB_get_program_binary
FNDEF_EX(glProgramParameteri, PFNGLPROGRAMPARAMETERIPROC);

// GL_ARB_uniform_buffer_object
FNDEF_EX(glGetUniformBlockIndex, PFNGLGETUNIFORMBLOCKINDEXPROC);
FNDEF_EX(glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC);
FNDEF_EX(glBindBufferBase, PFNGLBINDBUFFERBASEPROC);

// GL_ARB_texture_buffer_object
FNDEF_EX(glTexBufferARB, PFNGLTEXBUFFERARBPROC);

// GL_ARB_draw_indirect
FNDEF_EX(glDrawArraysIndirect, PFNGLDRAWARRAYSINDIRECTPROC);
FNDEF_EX(glDrawElementsIndirect, PFNGLDRAWELEMENTSINDIRECTPROC);

// GL_ARB_multi_draw_indirect
FNDEF_EX(glMultiDrawArraysIndirect, PFNGLMULTIDRAWARRAYSINDIRECTPROC);
FNDEF_EX(glMultiDrawElementsIndirect, PFNGLMULTIDRAWELEMENTSINDIRECTPROC);

// GL_ARB_vertex_shader
FNDEF_EX(glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC);  // glBindAttribLocationARB

// GL_ARB_tessellation_shader
FNDEF_EX(glPatchParameteri, PFNGLPATCHPARAMETERIPROC);

// GL_ARB_geometry_shader4
FNDEF_EX(glProgramParameteriARB, PFNGLPROGRAMPARAMETERIARBPROC);

// GL_ARB_compute_shader
FNDEF_EX(glDispatchCompute, PFNGLDISPATCHCOMPUTEPROC);

// GL_ARB_draw_instanced
FNDEF_EX(glDrawArraysInstancedARB, PFNGLDRAWARRAYSINSTANCEDARBPROC);
FNDEF_EX(glDrawElementsInstancedARB, PFNGLDRAWELEMENTSINSTANCEDARBPROC);

// GL_ARB_base_instance
FNDEF_EX(glDrawArraysInstancedBaseInstance, PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC);
FNDEF_EX(glDrawElementsInstancedBaseInstance, PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC);
FNDEF_EX(glDrawElementsInstancedBaseVertexBaseInstance, PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC);

// GL_ARB_instanced_arrays
FNDEF_EX(glVertexAttribDivisorARB, PFNGLVERTEXATTRIBDIVISORARBPROC);

// GL_ARB_vertex_array_object
FNDEF_EX(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
FNDEF_EX(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);
FNDEF_EX(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);

// GL_ARB_sampler_objects
FNDEF_EX(glGenSamplers, PFNGLGENSAMPLERSPROC);
FNDEF_EX(glDeleteSamplers, PFNGLDELETESAMPLERSPROC);
FNDEF_EX(glBindSampler, PFNGLBINDSAMPLERPROC);
FNDEF_EX(glSamplerParameteri, PFNGLSAMPLERPARAMETERIPROC);
FNDEF_EX(glSamplerParameterf, PFNGLSAMPLERPARAMETERFPROC);
FNDEF_EX(glSamplerParameterfv, PFNGLSAMPLERPARAMETERFVPROC);

// GL_ARB_draw_elements_base_vertex
FNDEF_EX(glDrawElementsBaseVertex, PFNGLDRAWELEMENTSBASEVERTEXPROC);
FNDEF_EX(glDrawElementsInstancedBaseVertex, PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC);

// GL_ARB_debug_output
FNDEF_EX(glDebugMessageCallbackARB, PFNGLDEBUGMESSAGECALLBACKARBPROC);
FNDEF_EX(glDebugMessageControlARB, PFNGLDEBUGMESSAGECONTROLARBPROC);

// GL_ARB_direct_state_access
FNDEF_EX(glCreateBuffers, PFNGLCREATEBUFFERSPROC);
FNDEF_EX(glCreateFramebuffers, PFNGLCREATEFRAMEBUFFERSPROC);
FNDEF_EX(glCreateTextures, PFNGLCREATETEXTURESPROC);
FNDEF_EX(glCreateVertexArrays, PFNGLCREATEVERTEXARRAYSPROC);
FNDEF_EX(glNamedBufferData, PFNGLNAMEDBUFFERDATAPROC);
FNDEF_EX(glNamedBufferSubData, PFNGLNAMEDBUFFERSUBDATAPROC);
FNDEF_EX(glMapNamedBuffer, PFNGLMAPNAMEDBUFFERPROC);
FNDEF_EX(glUnmapNamedBuffer, PFNGLUNMAPNAMEDBUFFERPROC);
FNDEF_EX(glProgramUniform1i, PFNGLPROGRAMUNIFORM1IPROC);
FNDEF_EX(glProgramUniform1ui, PFNGLPROGRAMUNIFORM1UIPROC);
FNDEF_EX(glProgramUniform1f, PFNGLPROGRAMUNIFORM1FPROC);
FNDEF_EX(glProgramUniform2fv, PFNGLPROGRAMUNIFORM2FVPROC);
FNDEF_EX(glProgramUniform3fv, PFNGLPROGRAMUNIFORM3FVPROC);
FNDEF_EX(glProgramUniform4fv, PFNGLPROGRAMUNIFORM4FVPROC);
FNDEF_EX(glProgramUniformMatrix3fv, PFNGLPROGRAMUNIFORMMATRIX3FVPROC);
FNDEF_EX(glProgramUniformMatrix4fv, PFNGLPROGRAMUNIFORMMATRIX4FVPROC);
FNDEF_EX(glTextureParameteri, PFNGLTEXTUREPARAMETERIPROC);
FNDEF_EX(glGenerateTextureMipmap, PFNGLGENERATETEXTUREMIPMAPPROC);
FNDEF_EX(glEnableVertexArrayAttrib, PFNGLENABLEVERTEXARRAYATTRIBPROC);
FNDEF_EX(glCheckNamedFramebufferStatus, PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC);
FNDEF_EX(glNamedRenderbufferStorage, PFNGLNAMEDRENDERBUFFERSTORAGEPROC);
FNDEF_EX(glNamedFramebufferRenderbuffer, PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC);
FNDEF_EX(glNamedFramebufferTexture, PFNGLNAMEDFRAMEBUFFERTEXTUREPROC);
FNDEF_EX(glNamedFramebufferTextureLayer, PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC);
FNDEF_EX(glTextureBuffer, PFNGLTEXTUREBUFFERPROC);
FNDEF_EX(glBindTextureUnit, PFNGLBINDTEXTUREUNITPROC);
FNDEF_EX(glCompressedTextureSubImage1D, PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC);
FNDEF_EX(glCompressedTextureSubImage2D, PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC);
FNDEF_EX(glCompressedTextureSubImage3D, PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC);
FNDEF_EX(glTextureSubImage1D, PFNGLTEXTURESUBIMAGE1DPROC);
FNDEF_EX(glTextureSubImage2D, PFNGLTEXTURESUBIMAGE2DPROC);
FNDEF_EX(glTextureSubImage3D, PFNGLTEXTURESUBIMAGE3DPROC);
FNDEF_EX(glVertexArrayAttribFormat, PFNGLVERTEXARRAYATTRIBFORMATPROC);
FNDEF_EX(glVertexArrayAttribIFormat, PFNGLVERTEXARRAYATTRIBIFORMATPROC);
FNDEF_EX(glVertexArrayAttribBinding, PFNGLVERTEXARRAYATTRIBBINDINGPROC);
FNDEF_EX(glVertexArrayVertexBuffer, PFNGLVERTEXARRAYVERTEXBUFFERPROC);
FNDEF_EX(glVertexArrayBindingDivisor, PFNGLVERTEXARRAYBINDINGDIVISORPROC);
FNDEF_EX(glVertexArrayElementBuffer, PFNGLVERTEXARRAYELEMENTBUFFERPROC);

// GL_ARB_texture_storage
FNDEF_EX(glTextureStorage1D, PFNGLTEXTURESTORAGE1DPROC);
FNDEF_EX(glTextureStorage2D, PFNGLTEXTURESTORAGE2DPROC);
FNDEF_EX(glTextureStorage3D, PFNGLTEXTURESTORAGE3DPROC);
FNDEF_EX(glTextureStorage2DMultisample, PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC);

// GL_ARB_copy_image
FNDEF_EX(glCopyImageSubData, PFNGLCOPYIMAGESUBDATAPROC);

// GL_ARB_gl_spirv
FNDEF_EX(glSpecializeShaderARB, PFNGLSPECIALIZESHADERARBPROC);

// GL_ARB_clip_control
FNDEF_EX(glClipControl, PFNGLCLIPCONTROLPROC);

// GL_ARB_occlusion_query
FNDEF_EX(glGenQueriesARB, PFNGLGENQUERIESARBPROC);
FNDEF_EX(glDeleteQueriesARB, PFNGLDELETEQUERIESARBPROC);
FNDEF_EX(glBeginQueryARB, PFNGLBEGINQUERYARBPROC);
FNDEF_EX(glEndQueryARB, PFNGLENDQUERYARBPROC);
FNDEF_EX(glGetQueryObjectuivARB, PFNGLGETQUERYOBJECTUIVARBPROC);

// GL_ARB_timer_query
FNDEF_EX(glQueryCounter, PFNGLQUERYCOUNTERPROC);


//[-------------------------------------------------------]
//[ Core (OpenGL version dependent)                       ]
//[-------------------------------------------------------]
FNDEF_EX(glShaderBinary, PFNGLSHADERBINARYPROC);  // OpenGL 4.1

//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef FNDEF_EX


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL