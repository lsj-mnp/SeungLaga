#pragma once

#include "SharedHeader.h"

enum class EShaderType
{
	Vertex,
	Pixel
};

constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

class CShader
{
public:
	CShader(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) : 
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(m_PtrDevice);
		assert(m_PtrDeviceContext);
	}
	~CShader() {}

	void Create(EShaderType ShaderType, const wstring& ShaderFileName, const string& EntryPoint)
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
			D3DCompileFromFile(ShaderFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				EntryPoint.c_str(), "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.GetAddressOf(), nullptr);

			m_PtrDevice->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(),
				nullptr, m_PixelShader.GetAddressOf());
			break;
		default:
			break;
		}
	}

	void Use()
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

private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

	EShaderType m_ShaderType{};

	ComPtr<ID3DBlob> m_Blob{};
	ComPtr<ID3D11VertexShader> m_VertexShader{};
	ComPtr<ID3D11PixelShader> m_PixelShader{};
	ComPtr<ID3D11InputLayout> m_InputLayout{};
};
