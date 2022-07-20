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
#include "Texture.h"
#include "AssetManager.h"

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

		std::string fstring(fileName);

		std::string::size_type index = fileName.find_last_of("/");

		std::string dir = fileName.substr(0, index + 1);

		std::string name = fileName.substr(14);

		index = name.find_last_of("/");

		name = name.substr(0, index);



		Texture* texture = nullptr;


		VertexLayout vLayout;


		if (mesh->HasPositions())
		{
			// Add Vector3 to the dynamic vertex layout
			vLayout.Append<VertexLayout::ElementType::Position3D>();
		}
		if (mesh->HasNormals())
		{
			// Add Vector3 to the dynamic vertex layout
			vLayout.Append<VertexLayout::ElementType::Normal>();
		}

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial* material = scene->mMaterials[i];

			aiString path;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				// Add Vector2 to the dynamic vertex layout
				vLayout.Append<VertexLayout::ElementType::Texture2D>();

				if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
				{
					std::string p(path.data);

					// create a texture
					texture = AssetManager::Get()->LoadTexture(dir + p);
				}
			}
		}
		VBuffer vBuffer(std::move(vLayout));

		//std::vector<VertexPosNormUV> vertices;
		//vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const auto textureVec = &mesh->mTextureCoords[0][i];

			Vector3 pos = Vector3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z);

			Vector3 norm = Vector3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z);

			Vector2 uv;

			if (mesh->HasTextureCoords(0))
			{
				uv = Vector2(textureVec->x, 1 - textureVec->y);
			}
			

			VertexPosNormUV v = {};
			v.pos = pos;
			v.normal = norm;
			v.uv = uv;

			//vertices.push_back(v);

			vBuffer.EmplaceBack(pos, norm, uv);
		}
		/*int testaa = vBuffer.Size();
		int testii = vBuffer.SizeBytes();
		int testinn = vertices.size() * sizeof(VertexPosNormUV);
		int taa = vBuffer.GetLayout().Size();*/

		std::vector<uint16_t> indices;
		indices.reserve(size_t(mesh->mNumFaces * 3));
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const auto& face = mesh->mFaces[i];

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		obj = new RenderObj(new VertexBuffer(
			vBuffer.GetData(),
			vBuffer.SizeBytes(),
			vBuffer.GetLayout().Size(),
			indices.data(),
			indices.size() * sizeof(uint16_t),
			sizeof(uint16_t)));

		/*obj = new RenderObj(new VertexBuffer(
			vertices.data(),
			vertices.size() * sizeof(VertexPosNormUV),
			sizeof(VertexPosNormUV),
			indices.data(),
			indices.size() * sizeof(uint16_t),
			sizeof(uint16_t)));*/

		obj->SetMaterial(AssetManager::Get()->GetMaterial("Phong"));

		obj->GetMaterial()->SetTexture(0, texture);

		fin.close();
	}

	return obj;
}
