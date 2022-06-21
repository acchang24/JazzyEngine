#pragma once
#include "EngineMath.h"

struct CameraConstants
{
	Matrix4 viewProjection;
	Vector3 position;
	float padding;
};

class Camera
{
public:
	Camera();
	~Camera();
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void SetActive();

	CameraConstants mCamConsts;
	Matrix4 mWorldToCamera;
	Matrix4 mProjection;

	Matrix4 mRotation = Matrix4::Identity;

	Vector3 cameraForward;

	Vector3 deltaPos;

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	float xRot = 0.0f;
	float yRot = 0.0f;

	float rotSpeed = 5.0f;

private:
	ID3D11Buffer* mConstBuffer;

	
};

