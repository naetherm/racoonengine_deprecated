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
#ifdef RHI_OPENGL
if (rhi->getNameId() == RERHI::NameId::OPENGL)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
if (rhi->getCapabilities().upperLeftOrigin)
{
	vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

	// Attribute input/output
	in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
	out gl_PerVertex
	{
		vec4 gl_Position;
	};
	out vec2 TexCoord;	// Normalized texture coordinate as output

	// Programs
	void main()
	{
		// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
		gl_Position = vec4(Position, 0.5, 1.0);

		// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
		// -> In OpenGL with "GL_ARB_clip_control"-extension, the texture origin is left/top which does not map well to clip space coordinates
		// -> We have to flip the y-axis to map the coordinate system to the texture coordinate system
		// -> (-1,-1) -> (0,1)
		// -> (1,1) -> (1,0)
		TexCoord = vec2(Position.x * 0.5f + 0.5f, 1.0f - (Position.y * 0.5f + 0.5f));
	}
	)";
}
else
{
	vertexShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

	// Attribute input/output
	in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
	out gl_PerVertex
	{
		vec4 gl_Position;
	};
	out vec2 TexCoord;	// Normalized texture coordinate as output

	// Programs
	void main()
	{
		// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
		gl_Position = vec4(Position, 0.5, 1.0);

		// Calculate the texture coordinate by mapping the clip space coordinate to a texture space coordinate
		// -> In OpenGL without "GL_ARB_clip_control"-extension, the texture origin is left/bottom which maps well to clip space coordinates
		// -> (-1,-1) -> (0,0)
		// -> (1,1) -> (1,1)
		TexCoord = Position.xy * 0.5 + 0.5;
	}
	)";
}


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode_MultipleRenderTargets = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in vec2 TexCoord;	// Normalized texture coordinate as input

// Programs
void main()
{
	gl_FragData[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);	// Red
	gl_FragData[1] = vec4(0.0f, 0.0f, 1.0f, 0.0f);	// Blue
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 410 core	// OpenGL 4.1

// Attribute input/output
in  vec2 TexCoord;		// Normalized texture coordinate as input
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
uniform sampler2D AlbedoMap0;
uniform sampler2D AlbedoMap1;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate from render target 0 (which should contain a red triangle)
	vec4 color0 = texture2D(AlbedoMap0, TexCoord);

	// Fetch the texel at the given texture coordinate from render target 1 (which should contain a blue triangle)
	vec4 color1 = texture2D(AlbedoMap1, TexCoord);

	// Calculate the final color by subtracting the colors of the both render targets from white
	// -> The result should be white or green
	OutputColor = vec4(1.0, 1.0, 1.0, 1.0) - color0 - color1;
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
