#pragma once
#include "Cache.h"
#include "Shader.h"

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

private:
	static AssetManager* sManager;

	Cache<Shader>* shaderCache;
};

