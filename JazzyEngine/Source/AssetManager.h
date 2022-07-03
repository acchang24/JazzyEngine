#pragma once
#include "Cache.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	static AssetManager* Get() { return sManager; }

	void Clear();

	void SaveShader(const std::string& fileName, Shader* shader);
	Shader* GetShader(const std::string& shaderName);

	void SaveMaterial(const std::string& fileName, Material* material);
	Material* GetMaterial(const std::string& materialName);

	Texture* LoadTexture(const std::string& fileName);

private:
	static AssetManager* sManager;

	Cache<Shader>* shaderCache;
	Cache<Texture>* textureCache;
	Cache<Material>* materialCache;
};

