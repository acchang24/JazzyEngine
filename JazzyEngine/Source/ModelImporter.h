#pragma once

#include "../assimp/include/assimp/Importer.hpp"
#include "../assimp/include/assimp/scene.h"
#include "../assimp/include/assimp/postprocess.h"

class ModelImporter
{
public:
	ModelImporter();
	~ModelImporter();

	void CreateModel(const std::string& fileName);
private:
};

