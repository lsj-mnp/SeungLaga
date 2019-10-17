#pragma once

#include "SharedHeader.h"

class CConstantBuffer
{
public:
	CConstantBuffer(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(m_PtrDevice);
		assert(m_PtrDeviceContext);
	}
	~CConstantBuffer() {}

public:
	void Create(EShaderType ShaderType, size_t ByteWidth, UINT Slot);

	void Update(const void* PtrData);

	void Use();
	
private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

private:
	ComPtr<ID3D11Buffer> m_ConstantBuffer{};
	size_t m_ByteWidth{};
	UINT m_Slot{};

	EShaderType m_eShaderType{};
};
