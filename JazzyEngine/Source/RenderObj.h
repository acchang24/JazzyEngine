#pragma once
#include "EngineMath.h"

class VertexBuffer;
class Shader;
class Mesh;

// Struct that matches the constants set in vertex shader
struct ObjectConstants
{
	Matrix4 modelToWorld;
};

class RenderObj
{
public:
	RenderObj();
	RenderObj(Mesh* mesh);
	virtual ~RenderObj();
	RenderObj(const RenderObj&) = delete;
	RenderObj& operator=(const RenderObj&) = delete;

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
	Mesh* mMesh;

	// Constant object buffer
	ID3D11Buffer* mConstBuffer;

	// Object info
	Vector3 pos;
	float scale;
	float rotation;

	Vector3 forward;

	float pitch; // Rotation x
	float yaw; // Rotation y
	float roll; // Rotation z
};

