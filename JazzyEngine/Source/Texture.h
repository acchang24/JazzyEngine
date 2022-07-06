#pragma once
#include "../Tools/DirectXTK/Inc/DDSTextureLoader.h"
#include "../Tools/DirectXTK/Inc/WICTextureLoader.h"

class AssetManager;

class Texture
{
public:
	Texture();
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Free();

	bool Load(const WCHAR* fileName);

	static Texture* StaticLoad(std::string fileName, AssetManager* pManager);

	void SetActive(int slot) const;

private:
	ID3D11Resource* mResource;
	ID3D11ShaderResourceView* mView;

	int mWidth;
	int mHeight;
};

