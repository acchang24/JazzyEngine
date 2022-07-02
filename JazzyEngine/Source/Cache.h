#pragma once
#include <string>
#include <unordered_map>

class AssetManager;

template<class T> class Cache
{
public:
	Cache(AssetManager* pManager) 
		: mManager(pManager)
	{
	}

	~Cache()
	{
		Clear();
	}

	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;

	void StoreCache(const std::string& key, T* asset)
	{
		mAssetMap[key] = asset;
	}

	T* Get(const std::string name)
	{
		if (mAssetMap.find(name) != mAssetMap.end())
		{
			return mAssetMap[name];
		}
		DbgAssert(false, "Could not find the loaded shader\n Check the shader name");
		return nullptr;
	}

	T* Load(const std::string& fileName)
	{
		if (mAssetMap.find(fileName) != mAssetMap.end())
		{
			return mAssetMap[fileName];
		}

		T* asset = T::StaticLoad(fileName, mManager);
		if (asset)
		{
			StoreCache(fileName, asset);
		}
		return asset;
	}


	void Clear()
	{
		for (auto a : mAssetMap)
		{
			delete a.second;
		}
		mAssetMap.clear();
	}

private:
	AssetManager* mManager;
	std::unordered_map<std::string, T*> mAssetMap;
};