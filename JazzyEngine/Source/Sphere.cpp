#include "stdafx.h"
#include "Sphere.h"
#include "VertexFormats.h"
#include "VertexBuffer.h"
#include "Graphics.h"
#include "AssetManager.h"

Sphere::Sphere() : RenderObj()
{
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
	std::vector<float> positions;

	for (int i = 0; i <= stacks; ++i)
	{
		float v = (float)i / (float)stacks;
		float phi = v * Math::Pi;

		for (int j = 0; j <= slices; ++j)
		{
			float u = (float)j / (float)slices;
			float theta = u * Math::TwoPi;

			float x = cos(theta) * sin(phi);
			float y = cos(phi);
			float z = sin(theta) * sin(phi);


			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);
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

	int numVertices = (int)positions.size() / 3;
	const int t = (const int)indices.size();


	Vertex verts[441];

	

	for (int i = 0; i < positions.size(); i+=3)
	{
		std::vector<float> pos;
		for (int j = 0; j < 3; j++)
		{
			pos.push_back(positions[i + j]);
		}
		Vector3 p = Vector3(pos[0], pos[1], pos[2]);
		Color4 c = Color4(1.0f, 1.0f, 1.0f, 1.0f);

		int vhat = i / 3;

		Vertex v;
		v.pos = p;
		v.color = c;

		verts[vhat] = v;
	}

	verts;

	uint16_t test[2520];
	for (int i = 0; i < indices.size(); i++)
	{
		test[i] = indices[i];
	}


	mVertexBuffer = new VertexBuffer(verts, sizeof(verts), sizeof(Vertex), test, sizeof(test), sizeof(uint16_t));

	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mObjConsts.modelToWorld = Matrix4::Identity;

	mShader = AssetManager::Get()->GetShader("Simple");

	scale = 0.3f;
}
