#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera()
{
	Graphics* g = Graphics::Get();

	cameraForward = Vector3::UnitZ;

	mCamConsts.position = Vector3(0.0f, 0.0f, -5.0f);
	deltaPos = Vector3(0.0f, 0.0f, -5.0f);

	mWorldToCamera = Matrix4::CreateTranslation(mCamConsts.position);
	//Matrix4::CreateLookAt(mCamConsts.position, Vector3::Zero, Vector3::UnitZ);

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


	mCamConsts.position = Transform(deltaPos, Matrix4::CreateYawPitchRoll(yRot, xRot, 0.0f)) + 
		Vector3(0.0f, 0.0f, 1.0f);

	// Set new camera position
	//mWorldToCamera = Matrix4::CreateTranslation(mCamConsts.position);
	cameraForward = Normalize(Vector3(0.0f, 0.0f, 1.0f) - mCamConsts.position);
	mWorldToCamera = Matrix4::CreateLookAt(mCamConsts.position, Vector3(0.0f, 0.0f, 1.0f), Vector3::UnitY);
		//Matrix4::CreateTranslation(mCamConsts.position)
		//* Matrix4::CreateRotationX(xRot)
		//* Matrix4::CreateRotationY(yRot);
		

	//*Matrix4::CreateYawPitchRoll(yRot, xRot, 0.0f);
	
		//;///////////////////////////////////////////////////
	//mCamConsts.position = Transform(mCamConsts.position, mWorldToCamera);
	//mCamConsts.position = mWorldToCamera.GetTranslation();

	Matrix4 pitchRot = Matrix4::CreateRotationX(-pitch);
	Matrix4 yawRot = Matrix4::CreateRotationY(-yaw);
	Matrix4 newRot = pitchRot * yawRot;
	//cameraForward += Transform(Vector3(0.0f, 0.0f, 1.0f), newRot);
	//cameraForward.Normalize();


	mProjection = //Matrix4::CreateYawPitchRoll(yaw, pitch, roll) 
		 Matrix4::CreateRotationZ(roll) * 
		 Matrix4::CreateRotationY(yaw) * Matrix4::CreateRotationX(pitch) * 
		 Matrix4::CreatePerspectiveFOV(Math::ToRadians(90.0f), g->GetScreenWidth(), g->GetScreenHeight(), 0.5f, 10000.0f);

	//Matrix4 newRot = Matrix4::CreateRotationY(yaw) * Matrix4::CreateRotationX(pitch);//Matrix4::CreateYawPitchRoll(yaw, pitch, roll);
	//newRot.Invert();
	//cameraForward = Transform(Vector3(0.0f, 0.0f, 1.0f), newRot);

	//mCamConsts.position = mWorldToCamera.GetTranslation();

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
