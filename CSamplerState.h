#pragma once

#include "SharedHeader.h"

class CSamplerState
{
public:
	CSamplerState(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(m_PtrDevice);
		assert(m_PtrDeviceContext);
	}
	~CSamplerState() {}

public:
	void Create(const D3D11_SAMPLER_DESC& SamplerDesc);
	void CreateLinear();
	void CreatePoint();

	void Use();

private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

private:
	ComPtr<ID3D11SamplerState> m_SamplerState{};
};
