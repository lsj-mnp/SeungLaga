#include "CConstantBuffer.h"

void CConstantBuffer::Create(EShaderType ShaderType, size_t ByteWidth, UINT Slot)
{
	m_Slot = Slot;

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = m_ByteWidth = ByteWidth;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;

	m_PtrDevice->CreateBuffer(&buffer_desc, nullptr, m_ConstantBuffer.GetAddressOf());
}

void CConstantBuffer::Update(const void* PtrData)
{
	D3D11_MAPPED_SUBRESOURCE mapped{};
	if (SUCCEEDED(m_PtrDeviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		memcpy(mapped.pData, PtrData, m_ByteWidth);

		m_PtrDeviceContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
}

void CConstantBuffer::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetConstantBuffers(m_Slot, 1, m_ConstantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}