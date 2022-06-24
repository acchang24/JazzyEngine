#pragma once
#include "EngineMath.h"

struct CameraConstants
{
	Matrix4 viewProjection;
	Vector3 position;
	float padding = 0.0f;
};

enum class CameraType
{
	FirstPerson,
	ThirdPerson,
};

class Camera
{
public:
	Camera();
	~Camera();
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void SetActive();

	CameraType GetType() { return mType; }

	void SwitchCamera();
	void ResetCamera();

	CameraConstants mCamConsts;
	Matrix4 mWorldToCamera;
	Matrix4 mProjection;
	
	Vector3 cameraForward;

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	float rotSpeed = 5.0f;

private:
	ID3D11Buffer* mConstBuffer;

	Vector3 camTarget;

	Vector3 defaultFwd;

	CameraType mType = CameraType::ThirdPerson;
};

