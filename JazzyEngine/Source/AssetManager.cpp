#include "stdafx.h"
#include "AssetManager.h"

AssetManager* AssetManager::sManager = nullptr;

AssetManager::AssetManager()
{
	sManager = this;
	shaderCache = new Cache<Shader>(this);
}

AssetManager::~AssetManager()
{
	sManager = nullptr;
	Clear();
}

void AssetManager::Clear()
{
	shaderCache->Clear();
}

void AssetManager::SaveShader(const std::string& fileName, Shader* shader)
{
	shaderCache->StoreCache(fileName, shader);
}

Shader* AssetManager::GetShader(const std::string& shaderName)
{
	return shaderCache->Get(shaderName);
}