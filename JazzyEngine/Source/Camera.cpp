#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera()
{
	Graphics* g = Graphics::Get();

	mWorldToCamera = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 7.5f));

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

	mCamConsts.viewProjection = mWorldToCamera * mProjection;

	Matrix4 temp = mWorldToCamera;
	temp.Invert();

	mCamConsts.position = temp.GetTranslation();


	g->UploadBuffer(mConstBuffer, &mCamConsts, sizeof(mCamConsts));
	g->GetContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);
	g->GetContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &mConstBuffer);

}
