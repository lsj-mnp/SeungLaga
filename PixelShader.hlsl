#include "Header.hlsli"

//PixelShader�� OM�ٷ� �� �ܰ�. ����, ����ϴ� ������ ��.

//�ؽ��Ŀ��� ���� ä���ؿ��� ������ ��.
SamplerState CurrentSamplerState;
//�ؽ���. �̹����� ��Ƶδ� ����.
Texture2D CurrentTexture2D;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 Result = CurrentTexture2D.Sample(CurrentSamplerState, input.texcoord);

	//Result.r += 0.5f;

	return Result;

	//return input.color;
}