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
//[ Shader start                                          ]
//[-------------------------------------------------------]
#ifdef RHI_VULKAN
if (rhi->getNameId() == RERHI::NameId::VULKAN)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 0) in  vec3 Position;	// Object space vertex position
layout(location = 0) out gl_PerVertex
{
	vec4 gl_Position;
};
layout(location = 1) out vec3 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(std140, set = 0, binding = 0) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
};

// Programs
void main()
{
	// Calculate the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(Position, 1.0);
	TexCoord = normalize(Position);
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 450 core	// OpenGL 4.5

// Attribute input/output
layout(location = 1) in  vec3 TexCoord;		// Normalized texture coordinate as input
layout(location = 0) out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(set = 0, binding = 1) uniform samplerCubeArray CubeMapArray;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture(CubeMapArray, vec4(TexCoord, 1.0f));	// The cube at index zero is black by intent, use index one
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
