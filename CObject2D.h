#pragma once

#include "SharedHeader.h"

struct SVertex2D
{
	SVertex2D() {}
	SVertex2D(const XMFLOAT2& _Position) : Position{ _Position } {}

	XMFLOAT2 Position{};
};

struct STriangle
{
	STriangle() {}
	STriangle(uint32_t _0, uint32_t _1, uint32_t _2) : Index{ _0, _1, _2 } {}

	uint32_t Index[3]{};
};

class CObject2D
{
public:
	CObject2D(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(m_PtrDevice);
		assert(m_PtrDeviceContext);
	}
	~CObject2D() {}

	void CreateRectangle(const XMFLOAT2& Size);

	void Draw();

private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

private:
	ComPtr<ID3D11Buffer> m_VertexBuffer{};
	ComPtr<ID3D11Buffer> m_IndexBuffer{};

	vector<SVertex2D> m_Vertices{};
	vector<STriangle> m_Triangles{};

	UINT m_VStride{ (UINT)sizeof(SVertex2D) };
	UINT m_VOffset{};
};