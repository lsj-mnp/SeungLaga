//이전 파이프라인에서 정보를 받는 역할을 함.
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

//다음 파이프라인으로 정보를 주는 역할을 함.
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};