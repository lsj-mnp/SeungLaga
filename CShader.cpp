#include "CShader.h"

void CShader::Create(EShaderType ShaderType, const wstring& ShaderFileName, const string& EntryPoint)
{
	m_ShaderType = ShaderType;

	switch (m_ShaderType)
	{
	case EShaderType::Vertex:
		D3DCompileFromFile(ShaderFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPoint.c_str(), "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.GetAddressOf(), nullptr);

		m_PtrDevice->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(),
			nullptr, m_VertexShader.GetAddressOf());

		m_PtrDevice->CreateInputLayout(KInputLayout, ARRAYSIZE(KInputLayout),
			m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), m_InputLayout.GetAddressOf());

		break;
	case EShaderType::Pixel:
		//플래그 1번에 디버그 플래그를 줘야 디버깅에서 버그 추적이 가능함. 
		D3DCompileFromFile(ShaderFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPoint.c_str(), "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.GetAddressOf(), nullptr);

		m_PtrDevice->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(),
			nullptr, m_PixelShader.GetAddressOf());
		break;
	default:
		break;
	}
}

void CShader::Use()
{
	switch (m_ShaderType)
	{
	case EShaderType::Vertex:
		m_PtrDeviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);

		m_PtrDeviceContext->IASetInputLayout(m_InputLayout.Get());
		break;
	case EShaderType::Pixel:
		m_PtrDeviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);
		break;
	default:
		break;
	}
}