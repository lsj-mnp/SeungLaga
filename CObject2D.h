#pragma once

#include "SharedHeader.h"

struct SVertex2D
{
	SVertex2D() {}
	SVertex2D(const XMFLOAT2& _Position, const XMFLOAT4& _Color, const XMFLOAT2& _TexCoord) :
		Position{ _Position }, Color{ _Color }, TexCoord{ _TexCoord } {}

	XMFLOAT2 Position{};
	XMFLOAT4 Color{};
	XMFLOAT2 TexCoord{};
};

//하나의 삼각형의 각 정점을 저장함.
struct STriangle
{
	STriangle() {}
	STriangle(uint32_t _0, uint32_t _1, uint32_t _2) : Index{ _0, _1, _2 } {}

	//int는 국제 표준이 없어서 언어에 맞게 알아서 바꿔주는 자료형을 씀.
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

public:
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
	//Offset. 몇 번째부터 시작할지. 
	UINT m_VOffset{};
};