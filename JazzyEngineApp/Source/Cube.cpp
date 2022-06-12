#include "stdafx.h"
#include "Cube.h"
#include "Graphics.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <random>

Cube::Cube() : RenderObj()
{
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


	mVertexBuffer = new VertexBuffer(pps, sizeof(pps), sizeof(Vector3), indices, sizeof(indices), sizeof(uint16_t));

	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mObjConsts.modelToWorld = Matrix4::Identity;

	mShader = new Shader();

	const D3D11_INPUT_ELEMENT_DESC colorIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mShader->Load(L"Shaders/CubeVS.hlsl", ShaderType::Vertex, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));
	mShader->Load(L"Shaders/CubePS.hlsl", ShaderType::Pixel, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));
	mConstColorBuffer = Graphics::Get()->CreateGraphicsBuffer(&cb2, sizeof(cb2), 0, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Cube::~Cube()
{
	mConstColorBuffer->Release();
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
	Matrix4 mat = Matrix4::CreateYawPitchRoll(yaw, pitch, roll)
		* Matrix4::CreateTranslation(Vector3(r, 0.0f, 0.0f))
		* Matrix4::CreateYawPitchRoll(theta, phi, chi)
		* Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 20.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(90.0f), 1.0f, 9.0f / 16.0f, 1.0f, 10000.0f);
	mat.Transpose();

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
