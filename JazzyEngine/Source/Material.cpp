#include "stdafx.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material()
{
	mShader = nullptr;

	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mMatConsts,
		sizeof(mMatConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mTextures[0] = nullptr;
}

Material::~Material()
{
	if (mConstBuffer)
	{
		mConstBuffer->Release();
	}
}

void Material::SetActive()
{
	// Set the material's shader and textures to active
	mShader->SetActive();

	for (int i = 0; i < sizeof(mTextures) / sizeof(mTextures[0]); i++)
	{
		if (mTextures[i])
		{
			mTextures[i]->SetActive(i);
		}
	}

	Graphics* graphics = Graphics::Get();

	// Update the constant material buffer with current data and upload to GPU
	graphics->UploadBuffer(mConstBuffer, &mMatConsts, sizeof(mMatConsts));

	// Bind that const buffer to the pixel shader
	graphics->GetContext()->PSSetConstantBuffers(
		Graphics::ConstantBuffer::CONSTANT_BUFFER_MATERIAL,
		1,
		&mConstBuffer);
}
