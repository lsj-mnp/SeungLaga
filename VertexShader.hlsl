#include "Header.hlsli"

cbuffer cbProjection : register(b0)
{
	float4x4 Projection;
};

cbuffer cbWorld : register(b1)
{
	float4x4 World;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, Projection);
	

	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}