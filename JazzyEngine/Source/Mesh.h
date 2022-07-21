#pragma once

class VertexBuffer;
class Material;

class Mesh
{
public:
	Mesh(const VertexBuffer* vBuffer, Material* material);
	~Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void Draw() const;

	Material* GetMaterial() { return mMaterial; }

private:
	const VertexBuffer* mVertexBuffer;
	Material* mMaterial;
};

