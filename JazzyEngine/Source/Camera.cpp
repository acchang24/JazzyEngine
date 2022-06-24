#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera()
{
	Graphics* g = Graphics::Get();

	cameraForward = Vector3::UnitZ;

	mCamConsts.position = Vector3(0.0f, 0.0f, -5.0f);

	camTarget = Vector3(0.0f,0.0f, 1.0f);

	defaultFwd = Vector3(0.0f, 0.0f, 1.0f);

	mWorldToCamera = Matrix4::CreateTranslation(mCamConsts.position);

	mProjection = Matrix4::CreateYawPitchRoll(0.0, 0.0f, 0.0f)
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(90.0f), g->GetScreenWidth(), g->GetScreenHeight(), 0.5f, 10000.0f);

	mConstBuffer = g->CreateGraphicsBuffer(&mCamConsts, sizeof(mCamConsts), 0, 
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Camera::~Camera()
{
	if (mConstBuffer)
	{
		mConstBuffer->Release();
	}
}

void Camera::SetActive()
{
	Graphics* g = Graphics::Get();

	switch (mType)
	{
	case CameraType::FirstPerson:
		{
			Matrix4 newRot = Matrix4::CreateYawPitchRoll(-yaw, pitch, 0.0f);
			cameraForward += Transform(defaultFwd, newRot);
			cameraForward.Normalize();

			mWorldToCamera = Matrix4::CreateTranslation(mCamConsts.position);

			mProjection = Matrix4::CreateRotationZ(roll) *
				Matrix4::CreateRotationY(yaw) * Matrix4::CreateRotationX(-pitch) *
				Matrix4::CreatePerspectiveFOV(Math::ToRadians(90.0f), g->GetScreenWidth(), g->GetScreenHeight(), 0.5f, 10000.0f);
		}
		break;
	case CameraType::ThirdPerson:
		{
			Vector3 camTargetDist = camTarget - mCamConsts.position;
			float ctd = camTargetDist.Length();

			Matrix4 camRotMat = Matrix4::CreateYawPitchRoll(-yaw, pitch, 0.0f);

			cameraForward = Transform(defaultFwd, camRotMat);
			cameraForward.Normalize();
			mCamConsts.position = cameraForward * -ctd + camTarget;

			cameraForward = Normalize(camTarget - mCamConsts.position);
			mWorldToCamera = Matrix4::CreateLookAt(mCamConsts.position, camTarget, Vector3::UnitY);

			mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(90.0f), g->GetScreenWidth(), g->GetScreenHeight(), 0.5f, 10000.0f);
		}
		break;
	}
	
	// Invert temp world to camera matrix
	Matrix4 temp = mWorldToCamera;
	temp.Invert();

	// Set view projection matrix
	mCamConsts.viewProjection = temp * mProjection;

	// Upload new buffer to gpu
	g->UploadBuffer(mConstBuffer, &mCamConsts, sizeof(mCamConsts));
	g->GetContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);
	g->GetContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);
}

void Camera::SwitchCamera() 
{
	if (mType == CameraType::FirstPerson)
	{
		mType = CameraType::ThirdPerson;
	}
	else
	{
		mType = CameraType::FirstPerson;
	}
}

void Camera::ResetCamera()
{
	mType = CameraType::ThirdPerson;
	mCamConsts.position = Vector3(0.0f, 0.0f, -5.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}