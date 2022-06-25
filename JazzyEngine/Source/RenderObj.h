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

	void SetShader(Shader* shader) { mShader = shader; }

	float GetScale() { return scale; }
	float GetRotation() { return rotation; }
	Vector3 GetPos() { return pos; }

	void SetScale(float s) { scale = s; }
	void SetPos(Vector3 p) { pos = p; }
	void SetYaw(float y) { yaw = y; }
	void SetPitch(float p) { pitch = p; }
	void SetRoll(float r) { roll = r; }

	Vector3 GetForward();

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

	Vector3 forward;

	float pitch = 0.0f; // Rotation x
	float yaw = 0.0f; // Rotation y
	float roll = 0.0f; // Rotation z
};

