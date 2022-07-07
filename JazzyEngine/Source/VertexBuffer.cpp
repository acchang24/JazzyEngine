#include "stdafx.h"
#include "VertexBuffer.h"
#include "Graphics.h"

// VertexBuffer takes in vertex/index data, sizeof() of the vertex/index data, and sizeof() of the data type
VertexBuffer::VertexBuffer(const void* vertexData, UINT sizeOfVertices, UINT vertexStride, const void* indexData, UINT sizeOfIndices, UINT indexStride)
{
	mVertexCount = sizeOfVertices / vertexStride;
	mVertexStride = vertexStride;
	mIndexCount = sizeOfIndices / indexStride;

	if (indexStride == 2)
	{
		mFormat = DXGI_FORMAT_R16_UINT;
	}
	else
	{
		mFormat = DXGI_FORMAT_R32_UINT;
	}

	Graphics* graphics = Graphics::Get();

	// Create Vertex Buffer
	mVertexBuffer = graphics->CreateGraphicsBuffer(
		vertexData,
		sizeOfVertices,
		vertexStride,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	// Create Index Buffer
	mIndexBuffer = graphics->CreateGraphicsBuffer(
		indexData,
		sizeOfIndices,
		indexStride,
		D3D11_BIND_INDEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

VertexBuffer::~VertexBuffer()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
	}
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
	}
}

void VertexBuffer::Draw() const
{

	mVertexCount;

	ID3D11DeviceContext* devCon = Graphics::Get()->GetContext();

	UINT offset = 0;

	// Bind the vertex buffer
	devCon->IASetVertexBuffers(0u, 1u, &mVertexBuffer, &mVertexStride, &offset);

	// Bind the index buffer
	devCon->IASetIndexBuffer(mIndexBuffer, mFormat, offset);

	// Draw
	devCon->DrawIndexed(mIndexCount, 0u, 0u);
}
