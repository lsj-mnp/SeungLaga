#pragma once

#include "SharedHeader.h"

class CObject2D;
class CTexture;

enum class ESampler
{
	LinearWrap,
	PointWrap
};

class CGameObject2D
{
	struct SComponentTransform
	{
		XMVECTOR Translation{};
		XMVECTOR Rotation{};
		XMVECTOR Scaling{ XMVectorSet(1, 1, 1, 1) };
		XMMATRIX WorldMatrix{ XMMatrixIdentity() };
	};

	struct SComponentRender
	{
		CObject2D* PtrObject2D{};
		CTexture* PtrTexture{};
	};

public:
	void* operator new(size_t Size)
	{
		return _aligned_malloc(Size, 16);
	}

	void operator delete(void* Ptr)
	{
		_aligned_free(Ptr);
	}

public:
	CGameObject2D() {}
	~CGameObject2D() {}

public:
	void SetTranslation(const XMVECTOR& Translation);

	void SetTranslationY(float Y);

	void SetTranslationX(float X);

	void SetRotation(const XMVECTOR& Rotation);

	void SetScaling(const XMVECTOR& Scaling);

	void Translate(const XMVECTOR& Delta);

	void SetObject2D(CObject2D* Object2D);

	void SetTexture(CTexture* Texture);

	void SetSampler(ESampler eSampler);

	void Draw();

public:
	ESampler GetSampler() const;

	const XMMATRIX& GetWorldMatrix() const;

public:
	const XMVECTOR& GetTranslation() const;

private:
	void UpdateWorldMatrix();

private:
	SComponentTransform m_ComponentTransform{};
	SComponentRender m_ComponentRender{};
	ESampler m_eSampler{};
};
