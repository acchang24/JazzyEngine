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
#include "Mesh.h"

ModelImporter::ModelImporter()
{
}

ModelImporter::~ModelImporter()
{
}

std::vector<Mesh*> ModelImporter::CreateModel(const std::string& fileName)
{
	//Mesh* newMesh = nullptr;

	std::vector<Mesh*> meshArray;
	std::vector<Texture*> textureArray;

	std::ifstream fin(fileName);
	if (!fin.fail())
	{
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(fileName,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);

		std::string fstring(fileName);

		std::string::size_type index = fileName.find_last_of("/");

		std::string dir = fileName.substr(0, index + 1);

		std::string name = fileName.substr(14);

		index = name.find_last_of("/");

		name = name.substr(0, index);

		//VertexLayout vLayout;
		//if (mesh->HasPositions())
		//{
		//	// Add Vector3 to the dynamic vertex layout
		//	vLayout.Append<VertexLayout::ElementType::Pos3D>();
		//}
		//if (mesh->HasNormals())
		//{
		//	// Add Vector3 to the dynamic vertex layout
		//	vLayout.Append<VertexLayout::ElementType::Normal>();
		//}

		//for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		//{
		//	const aiMaterial* material = scene->mMaterials[i];

		//	aiString path;
		//	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		//	{
		//		// Add Vector2 to the dynamic vertex layout
		//		//vLayout.Append<VertexLayout::ElementType::Texture2D>();

		//		if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
		//		{
		//			std::string p(path.data);

		//			// create a texture
		//			Texture* texture = AssetManager::Get()->LoadTexture(dir + p);
		//			textureArray.push_back(texture);
		//		}
		//	}
		//}
		//VBuffer vBuffer(std::move(vLayout));

		for (int index = 0; index < scene->mNumMeshes; index++)
		{
			const aiMesh* mesh = scene->mMeshes[index];

			aiString meshName = mesh->mName;

			std::vector<VertexPosNormUV> vertices;
			vertices.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				const auto textureVec = &mesh->mTextureCoords[0][i];

				Vector3 pos = *reinterpret_cast<Vector3*>(&mesh->mVertices[i]);
				Vector3 norm = *reinterpret_cast<Vector3*>(&mesh->mNormals[i]);

				/*Vector3 pos = Vector3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z);
				Vector3 norm = Vector3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);*/

				Vector2 uv;

				if (mesh->HasTextureCoords(0))
				{
					uv = Vector2(textureVec->x, 1 - textureVec->y);
				}

				VertexPosNormUV v = {};
				v.pos = pos;
				v.normal = norm;
				v.uv = uv;

				vertices.push_back(v);

				//vBuffer.EmplaceBack(pos, norm, uv);
			}

			std::vector<uint16_t> indices;
			indices.reserve(size_t(mesh->mNumFaces * 3));
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				const auto& face = mesh->mFaces[i];

				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			// load diffuse textures
			Texture* texture = nullptr;

			if (mesh->mMaterialIndex >= 0)
			{
				const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				aiString path;
				if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					// Add Vector2 to the dynamic vertex layout
					//vLayout.Append<VertexLayout::ElementType::Texture2D>();

					if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
					{
						std::string p(path.data);

						// create a texture
						texture = AssetManager::Get()->LoadTexture(dir + p);
					}
				}
			}


			/*newMesh = new Mesh(new VertexBuffer(
				vBuffer.GetData(),
				vBuffer.SizeBytes(),
				vBuffer.GetLayout().Size(),
				indices.data(),
				indices.size() * sizeof(uint16_t),
				sizeof(uint16_t)),
				AssetManager::Get()->GetMaterial("Phong"));*/

			Material* newMat = new Material();
			newMat->SetShader(AssetManager::Get()->GetShader("Phong"));
			newMat->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
			newMat->SetSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
			newMat->SetSpecularPower(100.0f);
			AssetManager::Get()->SaveMaterial(name + std::to_string(index), newMat);

			Mesh* newMesh = new Mesh(new VertexBuffer(
				vertices.data(),
				vertices.size() * sizeof(VertexPosNormUV),
				sizeof(VertexPosNormUV),
				indices.data(),
				indices.size() * sizeof(uint16_t),
				sizeof(uint16_t)),
				newMat);

			newMesh->GetMaterial()->SetTexture(0, texture);

			AssetManager::Get()->SaveMesh(name + std::to_string(index), newMesh);

			/*obj = new RenderObj(new VertexBuffer(
				vBuffer.GetData(),
				vBuffer.SizeBytes(),
				vBuffer.GetLayout().Size(),
				indices.data(),
				indices.size() * sizeof(uint16_t),
				sizeof(uint16_t)));*/

			/*obj = new RenderObj(new VertexBuffer(
				vertices.data(),
				vertices.size() * sizeof(VertexPosNormUV),
				sizeof(VertexPosNormUV),
				indices.data(),
				indices.size() * sizeof(uint16_t),
				sizeof(uint16_t)));*/

			//obj->SetMaterial(AssetManager::Get()->GetMaterial("Phong"));
			//obj->GetMaterial()->SetTexture(0, texture);

			meshArray.push_back(newMesh);
		}
		fin.close();
	}
	return meshArray;
}
