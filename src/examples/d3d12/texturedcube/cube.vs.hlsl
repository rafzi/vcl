/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2020 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
struct VertexShaderInput
{
	float3 Position : Position;
	float2 TexCoord : UV;
};

struct VertexShaderOutput
{
	float4 Color    : COLOR;
	float2 TexCoord : TEXCOORD0;
	float4 Position : SV_Position;
};

struct ModelViewProjection
{
	#pragma pack_matrix(column_major)
	float4x4 MVP;
};
ConstantBuffer<ModelViewProjection> PerObjectData : register(b0);

VertexShaderOutput main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.Position = mul(PerObjectData.MVP, float4(IN.Position, 1.0f));
	OUT.TexCoord = IN.TexCoord;
	OUT.Color = float4(0.5f * (IN.Position + float3(1, 1, 1)), 1.0f);

	return OUT;
}
