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
		HRESULT hr = S_FALSE;
		DbgAssert(hr == S_OK, "Could not find the loaded shader\n Check the shader name");
		return nullptr;
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