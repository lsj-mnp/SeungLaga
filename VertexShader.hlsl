#include "Header.hlsli"

cbuffer cbMatrix
{
	float4x4 Projection;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	//output.pos = input.pos;
	output.pos = mul(input.pos, Projection);

	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}