#include "Header.hlsli"

SamplerState CurrentSamplerState;
Texture2D CurrentTexture2D;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return CurrentTexture2D.Sample(CurrentSamplerState, input.texcoord);

	//return input.color;
}