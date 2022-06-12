#pragma once
#include "EngineMath.h"

class VertexBuffer;
class Shader;

// Struct that matches the constants set in vertex shader
struct ObjectConstants
{
	Matrix4 modelToWorld;
};

class RenderObj
{
public:
	RenderObj();
	RenderObj(const VertexBuffer* vBuffer, Shader* shader);
	virtual ~RenderObj();
	RenderObj(const RenderObj&) = delete;
	RenderObj& operator=(const RenderObj&) = delete;

	virtual void Update(float deltaTime);

	virtual void Draw();

	ObjectConstants mObjConsts;

protected:
	// Buffers
	const VertexBuffer* mVertexBuffer;
	ID3D11Buffer* mConstBuffer;

	// Shader
	Shader* mShader;

	// Object info
	Vector3 pos;
	float scale;
	float rotation;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	float phi = 0.0f;
	float chi = 0.0f;
	float theta = 0.0f;

	float r = 0.0f;

	float droll = 0.0f;
	float dpitch = 0.0f;
	float dyaw = 0.0f;
	float dphi = 0.0f;
	float dtheta = 0.0f;
	float dchi = 0.0f;

};

