#pragma once
#include "Cache.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

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

	void SaveMesh(const std::string& fileName, Mesh* mesh);
	Mesh* GetMesh(const std::string& meshName);

	Texture* LoadTexture(const std::string& fileName);

private:
	static AssetManager* sManager;

	Cache<Shader>* shaderCache;
	Cache<Texture>* textureCache;
	Cache<Material>* materialCache;
	Cache<Mesh>* meshCache;
};

