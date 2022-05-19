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
if (rhi->getNameId() == RERHI::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec4 PositionTexCoord;	// xy = object space vertex position as input with left/bottom is (0,0) and right/top is (1,1), zw = normalized 32 bit texture coordinate as input
in  vec4 Color;				// sRGB vertex color as input
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 TexCoordVs;		// Normalized texture coordinate as output
out vec4 ColorVs;			// sRGB vertex color as output

// Uniforms
layout(std140) uniform UniformBlockDynamicVs
{
	mat4 ObjectSpaceToClipSpaceMatrix;
};

// Programs
void main()
{
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix * vec4(PositionTexCoord.xy, 0.5, 1.0);

	// Pass through the vertex texture coordinate
	TexCoordVs = PositionTexCoord.zw;

	// Pass through the vertex color
	ColorVs = Color;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 TexCoordVs;	// Normalized texture coordinate as input
in  vec4 ColorVs;		// sRGB vertex color
out vec4 OutputColor;	// Output variable for sRGB fragment color

// Uniforms
uniform sampler2D GlyphMap;	// Glyph atlas texture map

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
  //OutputColor.rgb = ColorVs.rgb * texture2D(GlyphMap, TexCoordVs).rgb;
	//OutputColor = ColorVs * vec4(1.0f, 1.0f, 1.0f, texture2D(GlyphMap, TexCoordVs).r);
  OutputColor = ColorVs * texture2D(GlyphMap, TexCoordVs);
  //OutputColor = OutputColor * vec4(1.0f, 1.0f, 1.0f, texture2D(GlyphMap, TexCoordVs).r);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
