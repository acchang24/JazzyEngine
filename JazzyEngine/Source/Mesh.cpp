#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexBuffer.h"

Mesh::Mesh(const VertexBuffer* vBuffer, Material* material) 
	: mVertexBuffer(vBuffer),mMaterial(material)
{
}

Mesh::~Mesh()
{
	delete mVertexBuffer;
}

void Mesh::Draw() const
{
	mMaterial->SetActive();

	mVertexBuffer->Draw();
}
