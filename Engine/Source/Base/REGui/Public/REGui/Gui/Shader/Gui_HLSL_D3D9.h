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
#ifdef RHI_DIRECT3D9
if (rhi.getNameId() == RERHI::NameId::DIRECT3D9)
{


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// One vertex shader invocation per vertex
vertexShaderSourceCode = R"(
// Attribute input/output
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;	// Clip space vertex position as output, left/bottom is (-1,-1) and right/top is (1,1)
	float2 TexCoord : TEXCOORD0;	// Normalized texture coordinate as output
	float4 Color    : COLOR0;		// sRGB vertex color
};

// Uniforms
uniform float4x4 ObjectSpaceToClipSpaceMatrix;

// Programs
VS_OUTPUT main(float4 PositionTexCoord : POSITION,	// xy = clip space vertex position as input with left/bottom is (-1,-1) and right/top is (1,1), zw = normalized 32 bit texture coordinate as input
			   float4 Color            : COLOR0)	// sRGB vertex color
{
	VS_OUTPUT output;

	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	output.Position = mul(float4(PositionTexCoord.xy, 0.5f, 1.0f), ObjectSpaceToClipSpaceMatrix);

	// Pass through the vertex texture coordinate
	output.TexCoord = PositionTexCoord.zw;

	// Pass through the vertex color
	output.Color = Color;

	// Done
	return output;
}
)";


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// One fragment shader invocation per fragment
// "pixel shader" in Direct3D terminology
fragmentShaderSourceCode = R"(
// Uniforms
uniform sampler2D GlyphMap : register(s0);	// Glyph atlas texture map

// Programs
// -> Input vertex color is in sRGB, so is the fragment color output
float4 main(float4 Position : SV_POSITION, float2 TexCoord : TEXCOORD0, float4 Color : COLOR0) : SV_TARGET
{
	// Fetch the texel at the given texture coordinate and return its color
	return Color * float4(1.0f, 1.0f, 1.0f, tex2D(GlyphMap, TexCoord).r);
}
)";


//[-------------------------------------------------------]
//[ Shader end                                            ]
//[-------------------------------------------------------]
}
else
#endif
