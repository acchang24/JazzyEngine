#pragma once
#include "EngineMath.h"
#include "Graphics.h"

class Shader;
class Texture;

struct MaterialConstants
{
	Vector3 diffuseColor;
	float padding;
	Vector3 specularColor;
	float specularPower;
};

class Material
{
public:
	Material();
	~Material();
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;

	void SetActive();

	void SetShader(const Shader* shader) { mShader = shader; }

	void SetTexture(int slot, const Texture* texture) { mTextures[slot] = texture; }

	void SetDiffuseColor(const Vector3& color) { mMatConsts.diffuseColor = color; }

	void SetSpecularColor(const Vector3& color) { mMatConsts.specularColor = color; }

	void SetSpecularPower(float power) { mMatConsts.specularPower = power; }

	MaterialConstants mMatConsts;

private:
	ID3D11Buffer* mConstBuffer;

	const Shader* mShader;

	const Texture* mTextures[Graphics::TEXTURE_SLOT_TOTAL];
};

