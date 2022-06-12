#include "stdafx.h"
#include "Texture.h"
#include "Graphics.h"
#include "DirectXTK\Inc\DDSTextureLoader.h"
#include "DirectXTK\Inc\WICTextureLoader.h"

Texture::Texture()
	: mResource(nullptr)
	, mView(nullptr)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{
	Free();
}

void Texture::Free()
{
	if (mView)
	{
		mView->Release();
		mView = nullptr;
	}
	if (mResource)
	{
		mResource->Release();
		mResource = nullptr;
	}
	mWidth = 0;
	mHeight = 0;
}

bool Texture::Load(const WCHAR* filename)
{
	// Free Just in case this already has a texture loaded in
	Free();

	ID3D11Device* dev = Graphics::Get()->GetDevice();

	std::wstring fileStr(filename);
	// Check the file extention of the image
	std::wstring extention = fileStr.substr(fileStr.find_last_of('.'));
	HRESULT hr = E_FAIL;
	if (extention == L".dds" || extention == L".DDS")
		// Create from dds file
		hr = DirectX::CreateDDSTextureFromFile(dev, filename, &mResource, &mView);
	else
		hr = DirectX::CreateWICTextureFromFile(dev, filename, &mResource, &mView);
	DbgAssert(hr == S_OK, "Could not create a texture from file");
	if (hr != S_OK)
		return false;

	CD3D11_TEXTURE2D_DESC textureDesc = {};
	((ID3D11Texture2D*)mResource)->GetDesc(&textureDesc);
	mWidth = textureDesc.Width;
	mHeight = textureDesc.Height;

	return true;
}

void Texture::SetActive(int slot) const
{
	Graphics::Get()->SetActiveTexture(slot, mView);
}