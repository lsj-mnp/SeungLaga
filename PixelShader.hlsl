#include "Header.hlsli"

//PixelShader는 OM바로 전 단계. 배출, 출력하는 역할을 함.

//텍스쳐에서 색을 채집해오는 역할을 함.
SamplerState CurrentSamplerState;
//텍스쳐. 이미지를 담아두는 버퍼.
Texture2D CurrentTexture2D;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return CurrentTexture2D.Sample(CurrentSamplerState, input.texcoord);

	//return input.color;
}