#include "stdafx.h"
#include "AssetManager.h"

AssetManager* AssetManager::sManager = nullptr;

AssetManager::AssetManager()
{
	sManager = this;
	shaderCache = new Cache<Shader>(this);
	textureCache = new Cache<Texture>(this);
	materialCache = new Cache<Material>(this);
	meshCache = new Cache<Mesh>(this);
}

AssetManager::~AssetManager()
{
	sManager = nullptr;
	Clear();
}

void AssetManager::Clear()
{
	shaderCache->Clear();
	textureCache->Clear();
	materialCache->Clear();
	meshCache->Clear();
}

void AssetManager::SaveShader(const std::string& fileName, Shader* shader)
{
	shaderCache->StoreCache(fileName, shader);
}

Shader* AssetManager::GetShader(const std::string& shaderName)
{
	return shaderCache->Get(shaderName);
}

void AssetManager::SaveMaterial(const std::string& fileName, Material* material)
{
	materialCache->StoreCache(fileName, material);
}

Material* AssetManager::GetMaterial(const std::string& materialName)
{
	return materialCache->Get(materialName);
}

void AssetManager::SaveMesh(const std::string& fileName, Mesh* mesh)
{
	meshCache->StoreCache(fileName, mesh);
}

Mesh* AssetManager::GetMesh(const std::string& meshName)
{
	return meshCache->Get(meshName);
}

Texture* AssetManager::LoadTexture(const std::string& fileName)
{
	return textureCache->Load(fileName);
}
