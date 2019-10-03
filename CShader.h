#pragma once

#include "SharedHeader.h"

enum class EShaderType
{
	Vertex,
	Pixel
};

constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT		, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0,  8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

public:
	void Create(EShaderType ShaderType, const wstring& ShaderFileName, const string& EntryPoint);

	void Use();

private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

	EShaderType m_ShaderType{};

	ComPtr<ID3DBlob> m_Blob{};
	ComPtr<ID3D11VertexShader> m_VertexShader{};
	ComPtr<ID3D11PixelShader> m_PixelShader{};
	ComPtr<ID3D11InputLayout> m_InputLayout{};
};
