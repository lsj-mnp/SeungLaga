#include "Header.hlsli"

//PixelShader�� OM�ٷ� �� �ܰ�. ����, ����ϴ� ������ ��.

//�ؽ��Ŀ��� ���� ä���ؿ��� ������ ��.
SamplerState CurrentSamplerState;
//�ؽ���. �̹����� ��Ƶδ� ����.
Texture2D CurrentTexture2D;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return CurrentTexture2D.Sample(CurrentSamplerState, input.texcoord);

	//return input.color;
}