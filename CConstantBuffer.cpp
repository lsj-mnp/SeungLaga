#include "CConstantBuffer.h"

void CConstantBuffer::Create(EShaderType ShaderType, size_t ByteWidth)
{
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = m_ByteWidth = ByteWidth;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;

	m_PtrDevice->CreateBuffer(&buffer_desc, nullptr, m_ConstantBuffer.GetAddressOf());
}

void CConstantBuffer::Update(void* PtrData)
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
	case EShaderType::Vertex:
		m_PtrDeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	case EShaderType::Pixel:
		m_PtrDeviceContext->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}