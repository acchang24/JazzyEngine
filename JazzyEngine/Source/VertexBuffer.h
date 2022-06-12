#pragma once

// VertexBuffer class takes in all the vertex and index information of an object
// and creates its respective buffers. Binds the buffers when Draw() is called

class VertexBuffer
{
public:
	VertexBuffer(
		const void* vertexData, UINT sizeOfVertices, UINT vertexStride,
		const void* indexData, UINT sizeOfIndices, UINT indexStride);
	~VertexBuffer();
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void Draw() const;

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	DXGI_FORMAT mFormat;

	UINT mVertexStride;
	UINT mVertexCount;
	UINT mIndexCount;
};

