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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define FNDEF_EX(retType, funcName, args) retType (GL_APIENTRY *funcPtr_##funcName) args = nullptr
#ifndef FNPTR
#define FNPTR(name) funcPtr_##name
#endif

//[-------------------------------------------------------]
//[ EXT definitions                                       ]
//[-------------------------------------------------------]
// GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT		0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT	0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT	0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT	0x83F3

// GL_EXT_texture_compression_dxt1
// #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT	0x83F0	// Already defined for GL_EXT_texture_compression_s3tc

// GL_EXT_texture_compression_latc
#define GL_COMPRESSED_LUMINANCE_LATC1_EXT				0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT		0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT			0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT	0x8C73

// GL_EXT_texture_buffer
FNDEF_EX(void,	glTexBufferEXT,	(GLenum target, GLenum internalformat, GLuint buffer));
#define glTexBufferEXT	FNPTR(glTexBufferEXT)

// GL_EXT_draw_elements_base_vertex
FNDEF_EX(void,	glDrawElementsBaseVertexEXT,			(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex));
FNDEF_EX(void,	glDrawElementsInstancedBaseVertexEXT,	(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex));
#define glDrawElementsBaseVertexEXT				FNPTR(glDrawElementsBaseVertexEXT)
#define glDrawElementsInstancedBaseVertexEXT	FNPTR(glDrawElementsInstancedBaseVertexEXT)

// GL_EXT_base_instance
FNDEF_EX(void,	glDrawArraysInstancedBaseInstanceEXT,				(GLenum mode, int first, GLsizei count, GLsizei instancecount, GLuint baseinstance));
FNDEF_EX(void,	glDrawElementsInstancedBaseInstanceEXT,				(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLuint baseinstance));
FNDEF_EX(void,	glDrawElementsInstancedBaseVertexBaseInstanceEXT,	(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance));
#define glDrawArraysInstancedBaseInstanceEXT				FNPTR(glDrawArraysInstancedBaseInstanceEXT)
#define glDrawElementsInstancedBaseInstanceEXT				FNPTR(glDrawElementsInstancedBaseInstanceEXT)
#define glDrawElementsInstancedBaseVertexBaseInstanceEXT	FNPTR(glDrawElementsInstancedBaseVertexBaseInstanceEXT)

// GL_EXT_clip_control
FNDEF_EX(void,	glClipControlEXT,	(GLenum origin, GLenum depth));
#define glClipControlEXT			FNPTR(glClipControlEXT)

//[-------------------------------------------------------]
//[ AMD definitions                                       ]
//[-------------------------------------------------------]
// GL_AMD_compressed_3DC_texture
#define GL_3DC_X_AMD	0x87F9
#define GL_3DC_XY_AMD	0x87FA

//[-------------------------------------------------------]
//[ NV definitions                                        ]
//[-------------------------------------------------------]
// GL_NV_fbo_color_attachments
#define GL_COLOR_ATTACHMENT0_NV		0x8CE0	// Same value as GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT1_NV		0x8CE1
#define GL_COLOR_ATTACHMENT2_NV		0x8CE2
#define GL_COLOR_ATTACHMENT3_NV		0x8CE3
#define GL_COLOR_ATTACHMENT4_NV		0x8CE4
#define GL_COLOR_ATTACHMENT5_NV		0x8CE5
#define GL_COLOR_ATTACHMENT6_NV		0x8CE6
#define GL_COLOR_ATTACHMENT7_NV		0x8CE7
#define GL_COLOR_ATTACHMENT8_NV		0x8CE8
#define GL_COLOR_ATTACHMENT9_NV		0x8CE9
#define GL_COLOR_ATTACHMENT10_NV	0x8CEA
#define GL_COLOR_ATTACHMENT11_NV	0x8CEB
#define GL_COLOR_ATTACHMENT12_NV	0x8CEC
#define GL_COLOR_ATTACHMENT13_NV	0x8CED
#define GL_COLOR_ATTACHMENT14_NV	0x8CEE
#define GL_COLOR_ATTACHMENT15_NV	0x8CEF

//[-------------------------------------------------------]
//[ OES definitions                                       ]
//[-------------------------------------------------------]
// GL_OES_element_index_uint
#define GL_UNSIGNED_INT	0x1405

// GL_OES_packed_depth_stencil
#define GL_DEPTH_STENCIL_OES		0x84F9
#define GL_UNSIGNED_INT_24_8_OES	0x84FA
#define GL_DEPTH24_STENCIL8_OES		0x88F0

// GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES	0x81A6

// GL_OES_depth32
#define GL_DEPTH_COMPONENT32_OES	0x81A7

//[-------------------------------------------------------]
//[ KHR definitions                                       ]
//[-------------------------------------------------------]
// GL_KHR_debug
FNDEF_EX(void,	glDebugMessageCallbackKHR,	(GLDEBUGPROCKHR callback, const void* userParam));
FNDEF_EX(void,	glDebugMessageControlKHR,	(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled));
FNDEF_EX(void,	glDebugMessageInsertKHR,	(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf));
FNDEF_EX(void,	glPushDebugGroupKHR,		(GLenum source, GLuint id, GLsizei length, const GLchar* message));
FNDEF_EX(void,	glPopDebugGroupKHR,			(void));
FNDEF_EX(void,	glObjectLabelKHR,			(GLenum identifier, GLuint name, GLsizei length, const GLchar* label));
#define glDebugMessageCallbackKHR	FNPTR(glDebugMessageCallbackKHR)
#define glDebugMessageControlKHR	FNPTR(glDebugMessageControlKHR)
#define glDebugMessageInsertKHR		FNPTR(glDebugMessageInsertKHR)
#define glPushDebugGroupKHR			FNPTR(glPushDebugGroupKHR)
#define glPopDebugGroupKHR			FNPTR(glPopDebugGroupKHR)
#define glObjectLabelKHR			FNPTR(glObjectLabelKHR)

//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef FNDEF_EX
#undef FNPTR