#include "stdafx.h"
#include "ModelImporter.h"

ModelImporter::ModelImporter()
{
}

ModelImporter::~ModelImporter()
{
}

void ModelImporter::CreateModel(const std::string& fileName)
{
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	
	if (!scene)
	{

	}

}
