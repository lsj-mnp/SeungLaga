#include "CObject2D.h"

void CObject2D::CreateRectangle(const XMFLOAT2& Size)
{
	float half_width{ Size.x / 2 };
	float half_height{ Size.y / 2 };

	//4개의 정점의 좌표 및 색상, 가져올 이미지에서 출력할 좌표.
	m_Vertices.emplace_back(XMFLOAT2(-half_width, +half_height), XMFLOAT4(0.2f, 0.8f, 0.1f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_Vertices.emplace_back(XMFLOAT2(+half_width, +half_height), XMFLOAT4(0.8f, 0.1f, 0.5f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_Vertices.emplace_back(XMFLOAT2(-half_width, -half_height), XMFLOAT4(0.2f, 0.1f, 0.9f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	m_Vertices.emplace_back(XMFLOAT2(+half_width, -half_height), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

	//정점의 번호를 저장.
	m_Triangles.emplace_back(0, 1, 2);
	m_Triangles.emplace_back(1, 3, 2);

	//VertexBuffer 생성
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = m_Vertices.size() * sizeof(SVertex2D);
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Vertices[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.GetAddressOf());
	}
	
	//IndexBuffer 생성. 코드펜스를 해두면 lifetime이 펜스 안으로 제한되어 같은 이름의 변수를 선언해도 문제가 없어짐.
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = m_Triangles.size() * sizeof(STriangle);
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Triangles[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_IndexBuffer.GetAddressOf());
	}
}

void CObject2D::Draw()
{
	//인덱스가 값이 여러개면 R32G32...etc로 썼겠지만, 인덱스는 번호가 하나라 R32임.
	m_PtrDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VStride, &m_VOffset);

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_PtrDeviceContext->DrawIndexed(static_cast<UINT>(m_Triangles.size() * 3), 0, 0);
}