#include "stdafx.h"
#include "RenderObj.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"

RenderObj::RenderObj()
	: mMesh(nullptr)
	, mConstBuffer(nullptr)
	, pos(Vector3::Zero)
	, scale(1.0f)
	, rotation(0.0f)
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)
	, forward(Vector3::Zero)
{
}

RenderObj::RenderObj(Mesh* mesh)
	: mMesh(mesh)
	, pos(Vector3::Zero)
	, scale(1.0f)
	, rotation(0.0f)
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)
	, forward(Vector3::Zero)
{
	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

RenderObj::RenderObj(const std::vector<Mesh*>& meshes)
	: mMesh(nullptr)
	, mMeshes(meshes)
	, pos(Vector3::Zero)
	, scale(1.0f)
	, rotation(0.0f)
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)
	, forward(Vector3::Zero)
{
	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

RenderObj::~RenderObj()
{
	if (mConstBuffer)
	{
		mConstBuffer->Release();
	}
}

Vector3 RenderObj::GetForward()
{
	return Vector3(cosf(rotation + Math::PiOver2), sinf(rotation + Math::PiOver2), 0.0f);
}

void RenderObj::Update(float deltaTime)
{
	Matrix4 mat = Matrix4::CreateScale(scale)
		* Matrix4::CreateYawPitchRoll(yaw, pitch, roll)
		* Matrix4::CreateTranslation(pos);

	mObjConsts.modelToWorld = mat;
}

void RenderObj::Draw()
{
	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(Graphics::ConstantBuffer::CONSTANT_BUFFER_RENDEROBJ, 1, &mConstBuffer);

	if (mMeshes.empty() && mMesh)
	{
		mMesh->Draw();
	}
	else
	{
		for (auto m : mMeshes)
		{
			m->Draw();
		}
	}
}