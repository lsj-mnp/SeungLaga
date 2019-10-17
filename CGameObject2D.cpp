#include "CGameObject2D.h"
#include "CObject2D.h"
#include "CTexture.h"

void CGameObject2D::SetTranslation(const XMVECTOR& Translation)
{
	m_ComponentTransform.Translation = Translation;

	UpdateWorldMatrix();
}

void CGameObject2D::SetTranslationY(float Y)
{
	m_ComponentTransform.Translation = XMVectorSetY(m_ComponentTransform.Translation, Y);

	UpdateWorldMatrix();
}

void CGameObject2D::SetTranslationX(float X)
{
	m_ComponentTransform.Translation = XMVectorSetX(m_ComponentTransform.Translation, X);

	UpdateWorldMatrix();
}

void CGameObject2D::SetRotation(const XMVECTOR& Rotation)
{
	m_ComponentTransform.Rotation = Rotation;

	UpdateWorldMatrix();
}

void CGameObject2D::SetScaling(const XMVECTOR& Scaling)
{
	m_ComponentTransform.Scaling = Scaling;

	UpdateWorldMatrix();
}

void CGameObject2D::Translate(const XMVECTOR& Delta)
{
	m_ComponentTransform.Translation += Delta;

	UpdateWorldMatrix();
}

void CGameObject2D::SetObject2D(CObject2D* Object2D)
{
	m_ComponentRender.PtrObject2D = Object2D;
}

void CGameObject2D::SetTexture(CTexture* Texture)
{
	m_ComponentRender.PtrTexture = Texture;
}

void CGameObject2D::SetSampler(ESampler eSampler)
{
	m_eSampler = eSampler;
}

void CGameObject2D::Draw()
{
	if (!m_ComponentRender.PtrObject2D) { return; }

	if (m_ComponentRender.PtrTexture) { m_ComponentRender.PtrTexture->Use(); }

	m_ComponentRender.PtrObject2D->Draw();
}

ESampler CGameObject2D::GetSampler() const
{
	return m_eSampler;
}

const XMMATRIX& CGameObject2D::GetWorldMatrix() const
{
	return m_ComponentTransform.WorldMatrix;
}

const XMVECTOR& CGameObject2D::GetTranslation() const
{
	return m_ComponentTransform.Translation;
}

void CGameObject2D::UpdateWorldMatrix()
{
	XMMATRIX MatrixTranslation{ XMMatrixTranslationFromVector(m_ComponentTransform.Translation) };
	XMMATRIX MatrixRotation{ XMMatrixRotationRollPitchYawFromVector(m_ComponentTransform.Rotation) };
	XMMATRIX MatrixScaling{ XMMatrixScalingFromVector(m_ComponentTransform.Scaling) };

	m_ComponentTransform.WorldMatrix = MatrixScaling * MatrixRotation * MatrixTranslation;
}
