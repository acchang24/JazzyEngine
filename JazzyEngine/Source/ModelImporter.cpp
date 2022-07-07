#include "stdafx.h"
#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include "EngineMath.h"
#include "VertexFormats.h"
#include "VertexBuffer.h"
#include "AssetManager.h"
#include "RenderObj.h"

ModelImporter::ModelImporter()
{
}

ModelImporter::~ModelImporter()
{
}

RenderObj* ModelImporter::CreateModel(const std::string& fileName)
{
	std::ifstream fin(fileName);
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		std::string vhat = "Vhat is happening";
	}


	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(fileName, 
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices);
	
	const auto mesh = scene->mMeshes[0];


	std::vector<VertexPosNorm> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vector3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;

		Vector3 norm;
		norm.x = mesh->mNormals[i].x;
		norm.y = mesh->mNormals[i].y;
		norm.z = mesh->mNormals[i].z;

		VertexPosNorm v;
		v.pos = pos;
		v.normal = norm;
		
		vertices.push_back(v);

	}
	std::vector<uint16_t> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);

	}

	

	

	int test = sizeof(vertices.data());


	RenderObj* obj = new RenderObj(new VertexBuffer(
		vertices.data(),
		vertices.size() * sizeof(VertexPosNorm),
		sizeof(VertexPosNorm),
		indices.data(), 
		indices.size() * sizeof(uint16_t),
		sizeof(uint16_t)));



	return obj;
}
