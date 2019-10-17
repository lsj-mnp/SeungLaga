#include "CSamplerState.h"

void CSamplerState::Create(const D3D11_SAMPLER_DESC& SamplerDesc)
{
	m_PtrDevice->CreateSamplerState(&SamplerDesc, m_SamplerState.GetAddressOf());
}

void CSamplerState::Use()
{
	m_PtrDeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
}