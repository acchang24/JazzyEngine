#include "stdafx.h"
#include "Sphere.h"
#include "VertexBuffer.h"
#include "Graphics.h"
#include "AssetManager.h"

Sphere::Sphere() : RenderObj()
{
	scale = 0.3f;
	CreateSphere();
}

Sphere::~Sphere()
{
}

void Sphere::CreateSphere()
{
	int stacks = 20;
	int slices = 20;

	std::vector<uint16_t> indices;
	std::vector<Vertex> positions;

	for (int i = 0; i <= stacks; ++i)
	{
		float v = (float)i / (float)stacks;
		float phi = v * Math::Pi;

		float x;
		float y;
		float z;

		for (int j = 0; j <= slices; ++j)
		{
			float u = (float)j / (float)slices;
			float theta = u * Math::TwoPi;

			x = cos(theta) * sin(phi);
			y = cos(phi);
			z = sin(theta) * sin(phi);

			Vertex v;
			v.color = Color4(1.0f,1.0f,1.0f,1.0f);
			v.pos = Vector3(x, y, z);

			positions.push_back(v);
		}
	}

	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {
		indices.push_back(uint16_t(i));
		indices.push_back(uint16_t(i + slices + 1));
		indices.push_back(uint16_t(i + slices));

		indices.push_back(uint16_t(i + slices + 1));
		indices.push_back(uint16_t(i));
		indices.push_back(uint16_t(i + 1));
	}

	mVertexBuffer = new VertexBuffer(positions.data(), sizeof(Vertex) * positions.size(), sizeof(Vertex), indices.data(), indices.size() * sizeof(uint16_t), sizeof(uint16_t));

	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mObjConsts.modelToWorld = Matrix4::Identity;
}
