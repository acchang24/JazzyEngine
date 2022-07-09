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
	RenderObj* obj = nullptr; 

	std::ifstream fin(fileName);
	if (!fin.fail())
	{
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(fileName,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);

		const auto mesh = scene->mMeshes[0];
		const auto material = scene->mMaterials[0];

		std::vector<VertexPosNorm> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vector3 pos = Vector3(
			pos.x = mesh->mVertices[i].x,
			pos.y = mesh->mVertices[i].y,
			pos.z = mesh->mVertices[i].z);

			Vector3 norm = Vector3(
			norm.x = mesh->mNormals[i].x,
			norm.y = mesh->mNormals[i].y,
			norm.z = mesh->mNormals[i].z);

			VertexPosNorm v = {};
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

		obj = new RenderObj(new VertexBuffer(
			vertices.data(),
			vertices.size() * sizeof(VertexPosNorm),
			sizeof(VertexPosNorm),
			indices.data(),
			indices.size() * sizeof(uint16_t),
			sizeof(uint16_t)));

		fin.close();
	}

	return obj;
}
