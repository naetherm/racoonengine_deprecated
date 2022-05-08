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
vertexShaderSourceCode = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Attribute input/output
in  vec2 Position;	// Clip space vertex position as input, left/bottom is (-1,-1) and right/top is (1,1)
out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 TexCoord;	// Normalized texture coordinate as output

// Uniforms
layout(binding = 1) uniform samplerBuffer InputTextureBuffer;
layout(std430, binding = 2) readonly buffer InputStructuredBuffer
{
	Vertex inputStructuredBufferVertex[];
};

// Programs
void main()
{
	// Pass through the clip space vertex position, left/bottom is (-1,-1) and right/top is (1,1)
	gl_Position = vec4(Position + texelFetch(InputTextureBuffer, gl_VertexID).xy + inputStructuredBufferVertex[gl_VertexID].position, 0.5f, 1.0f);
	TexCoord = Position.xy;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
fragmentShaderSourceCode = R"(#version 430 core	// OpenGL 4.3

// Attribute input/output
in  vec2 TexCoord;		// Normalized texture coordinate as input
out vec4 OutputColor;	// Output variable for fragment color

// Uniforms
layout(std140, binding = 0) uniform UniformBuffer
{
	vec4 inputColorUniform;
};
layout(binding = 3) uniform sampler2D AlbedoMap;

// Programs
void main()
{
	// Fetch the texel at the given texture coordinate and return its color
	OutputColor = texture2D(AlbedoMap, TexCoord) * inputColorUniform;
}
)";


//[-------------------------------------------------------]
//[ Compute shader source code                            ]
//[-------------------------------------------------------]
computeShaderSourceCode1 = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
};

// Input
layout(binding = 0) uniform sampler2D InputTexture2D;
layout(std430, binding = 1) readonly buffer InputVertexBuffer
{
	Vertex inputVertices[3];
};
layout(std430, binding = 2) readonly buffer InputIndexBuffer
{
	uint inputIndices[3];
};
layout(std140, binding = 3) uniform InputUniformBuffer
{
	vec4 inputColorUniform;
};

// Output
layout(rgba8, binding = 4) writeonly uniform image2D OutputTexture2D;
layout(std430, binding = 5) writeonly buffer OutputVertexBuffer
{
	Vertex outputVertices[3];
};
layout(std430, binding = 6) writeonly buffer OutputIndexBuffer
{
	uint outputIndices[3];
};

// Programs
layout (local_size_x = 16, local_size_y = 16) in;
void main()
{
	// Fetch input texel
	vec4 color = texelFetch(InputTexture2D, ivec2(gl_GlobalInvocationID.xy), 0) * inputColorUniform;

	// Modify color
	color.g *= 1.0f - (float(gl_GlobalInvocationID.x) / 16.0f);
	color.g *= 1.0f - (float(gl_GlobalInvocationID.y) / 16.0f);

	// Output texel
	imageStore(OutputTexture2D, ivec2(gl_GlobalInvocationID.xy), color);

	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output vertex buffer values
		for (int vertexBufferIndex = 0; vertexBufferIndex < 3; ++vertexBufferIndex)
		{
			outputVertices[vertexBufferIndex] = inputVertices[vertexBufferIndex];
		}

		// Output index buffer values
		for (int indexBufferIndex = 0; indexBufferIndex < 3; ++indexBufferIndex)
		{
			outputIndices[indexBufferIndex] = inputIndices[indexBufferIndex];
		}

		// Output uniform buffer not possible by design
	}
}
)";

computeShaderSourceCode2 = R"(#version 430 core	// OpenGL 4.3
struct Vertex
{
	vec2 position;
	vec2 padding;
};

// Same layout as "RERHI::DrawIndexedArguments"
struct DrawIndexedArguments
{
	uint indexCountPerInstance;
	uint instanceCount;
	uint startIndexLocation;
	uint baseVertexLocation;
	uint startInstanceLocation;
};

// Input
layout(binding = 0) uniform samplerBuffer InputTextureBuffer;
layout(std430, binding = 1) readonly buffer InputStructuredBuffer
{
	Vertex inputStructuredBufferVertex[];
};
layout(std430, binding = 2) readonly buffer InputIndirectBuffer
{
	DrawIndexedArguments inputDrawIndexedArguments;
};

// Output
layout(rgba32f, binding = 3) writeonly uniform imageBuffer OutputTextureBuffer;
layout(std430, binding = 4) writeonly buffer OutputStructuredBuffer
{
	Vertex outputStructuredBufferVertex[];
};
layout(std430, binding = 5) writeonly buffer OutputIndirectBuffer
{
	DrawIndexedArguments outputDrawIndexedArguments;
};

// Programs
layout (local_size_x = 3, local_size_y = 1) in;
void main()
{
	// Output buffer
	if (0 == gl_GlobalInvocationID.x && 0 == gl_GlobalInvocationID.y && 0 == gl_GlobalInvocationID.z)
	{
		// Output texture buffer values
		for (int textureBufferIndex = 0; textureBufferIndex < 3; ++textureBufferIndex)
		{
			imageStore(OutputTextureBuffer, textureBufferIndex, texelFetch(InputTextureBuffer, textureBufferIndex));
		}

		// Output structured buffer values
		for (int structuredBufferIndex = 0; structuredBufferIndex < 3; ++structuredBufferIndex)
		{
			outputStructuredBufferVertex[structuredBufferIndex] = inputStructuredBufferVertex[structuredBufferIndex];
		}

		// Output indirect buffer values (draw calls)
		// outputDrawIndexedArguments.indexCountPerInstance = inputDrawIndexedArguments.indexCountPerInstance;	- Filled by compute shader via atomics counting
		outputDrawIndexedArguments.instanceCount		 = inputDrawIndexedArguments.instanceCount;
		outputDrawIndexedArguments.startIndexLocation	 = inputDrawIndexedArguments.startIndexLocation;
		outputDrawIndexedArguments.baseVertexLocation	 = inputDrawIndexedArguments.baseVertexLocation;
		outputDrawIndexedArguments.startInstanceLocation = inputDrawIndexedArguments.startInstanceLocation;

		// Output uniform buffer not possible by design
	}

	// Atomics for counting usage example
	// -> Change 'layout (local_size_x = 3, local_size_y = 1) in;' into 'layout (local_size_x = 1, local_size_y = 1) in;' and if the triangle is gone you know the counter reset worked
	if (0 == gl_GlobalInvocationID.x)
	{
		// Reset the counter on first invocation
		atomicExchange(outputDrawIndexedArguments.indexCountPerInstance, 0);
	}
	atomicAdd(outputDrawIndexedArguments.indexCountPerInstance, 1);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
