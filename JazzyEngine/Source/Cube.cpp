#include "stdafx.h"
#include "Cube.h"
#include "Graphics.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <random>
#include "AssetManager.h"

Cube::Cube() : RenderObj()
{
	scale = 1.0f;
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	r = rdist(rng);
	droll = ddist(rng);
	dpitch = ddist(rng);
	dyaw = ddist(rng);
	dphi = odist(rng);
	dtheta = odist(rng);
	dchi = odist(rng);
	chi = adist(rng);
	theta = adist(rng);
	phi = adist(rng);


	mVertexBuffer = new VertexBuffer(vColor, sizeof(vColor), sizeof(VertexPosNormColor), indices, sizeof(indices), sizeof(uint16_t));

	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mObjConsts.modelToWorld = Matrix4::Identity;

	mShader = AssetManager::Get()->GetShader("Colored");

	mConstColorBuffer = nullptr;
	//mConstColorBuffer = Graphics::Get()->CreateGraphicsBuffer(&cb2, sizeof(cb2), 0, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Cube::~Cube()
{
	if (mConstColorBuffer)
	{
		mConstColorBuffer->Release();
	}
}

void Cube::Update(float deltaTime)
{
	roll += droll * deltaTime;
	pitch += dpitch * deltaTime;
	yaw += dyaw * deltaTime;
	theta += dtheta * deltaTime;
	phi += dphi * deltaTime;
	chi += dchi * deltaTime;

	// Calculate transform
	Matrix4 mat = Matrix4::CreateScale(scale) * Matrix4::CreateYawPitchRoll(yaw, pitch, roll)
		* Matrix4::CreateTranslation(Vector3(r, 0.0f, 0.0f))
		* Matrix4::CreateYawPitchRoll(chi, phi, theta);

	mObjConsts.modelToWorld = mat;
}

void Cube::Draw()
{
	// Set color const to pixel shader
	Graphics::Get()->GetContext()->PSSetConstantBuffers(Graphics::ConstantBuffer::CONSTANT_BUFFER_CUBE_COLOR, 1, &mConstColorBuffer);

	mShader->SetActive();

	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(Graphics::ConstantBuffer::CONSTANT_BUFFER_RENDEROBJ, 1, &mConstBuffer);

	mVertexBuffer->Draw();
}
