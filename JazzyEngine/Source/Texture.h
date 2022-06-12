#pragma once

class Texture
{
public:
	Texture();
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Free();

	bool Load(const WCHAR* fileName);

	void SetActive(int slot) const;

private:
	ID3D11Resource* mResource;
	ID3D11ShaderResourceView* mView;

	int mWidth;
	int mHeight;
};

