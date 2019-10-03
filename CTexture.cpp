#include "CTexture.h"

void CTexture::CreateFromFile(const wstring& FileName)
{
	CreateWICTextureFromFile(m_PtrDevice, FileName.c_str(), (ID3D11Resource * *)m_Texture2D.GetAddressOf(), m_ShaderResourceView.GetAddressOf());
}

void CTexture::Use()
{
	m_PtrDeviceContext->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}