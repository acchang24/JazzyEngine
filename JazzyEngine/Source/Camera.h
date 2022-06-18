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


private:
	ID3D11Buffer* mConstBuffer;


};

