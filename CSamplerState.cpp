#include "CSamplerState.h"

void CSamplerState::Create(const D3D11_SAMPLER_DESC& SamplerDesc)
{
	m_PtrDevice->CreateSamplerState(&SamplerDesc, m_SamplerState.GetAddressOf());
}

void CSamplerState::CreateLinear()
{
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.BorderColor[0] = 1.0f;
	sampler_desc.BorderColor[1] = 1.0f;
	sampler_desc.BorderColor[2] = 1.0f;
	sampler_desc.BorderColor[3] = 1.0f;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MinLOD = 0;
	sampler_desc.MipLODBias = 0;

	m_PtrDevice->CreateSamplerState(&sampler_desc, m_SamplerState.GetAddressOf());
}

void CSamplerState::CreatePoint()
{
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.BorderColor[0] = 1.0f;
	sampler_desc.BorderColor[1] = 1.0f;
	sampler_desc.BorderColor[2] = 1.0f;
	sampler_desc.BorderColor[3] = 1.0f;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MinLOD = 0;
	sampler_desc.MipLODBias = 0;

	m_PtrDevice->CreateSamplerState(&sampler_desc, m_SamplerState.GetAddressOf());
}

void CSamplerState::Use()
{
	m_PtrDeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
}