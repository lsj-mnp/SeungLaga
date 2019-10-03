#include "CObject2D.h"

void CObject2D::CreateRectangle(const XMFLOAT2& Size)
{
	float half_width{ Size.x / 2 };
	float half_height{ Size.y / 2 };

	m_Vertices.emplace_back(XMFLOAT2(-half_width, +half_height));
	m_Vertices.emplace_back(XMFLOAT2(+half_width, +half_height));
	m_Vertices.emplace_back(XMFLOAT2(-half_width, -half_height));
	m_Vertices.emplace_back(XMFLOAT2(+half_width, -half_height));

	m_Triangles.emplace_back(0, 1, 2);
	m_Triangles.emplace_back(1, 3, 2);

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
	m_PtrDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VStride, &m_VOffset);

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_PtrDeviceContext->DrawIndexed(static_cast<UINT>(m_Triangles.size() * 3), 0, 0);
}