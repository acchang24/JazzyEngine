#pragma once
#include "RenderObj.h"
#include "VertexFormats.h"

class App;

const Vector3 pps[] =
{
	{Vector3(-1.0f, -1.0f, -1.0f)},
	{Vector3(1.0f, -1.0f, -1.0f)},
	{Vector3(-1.0f, 1.0f, -1.0f)},
	{Vector3(1.0f, 1.0f, -1.0f)},
	{Vector3(-1.0f, -1.0f, 1.0f)},
	{Vector3(1.0f, -1.0f, 1.0f)},
	{Vector3(-1.0f, 1.0f, 1.0f)},
	{Vector3(1.0f, 1.0f, 1.0f)},
};

const uint16_t indices[] =
{
	0, 2, 1,
	2, 3, 1,
	1, 3, 5,
	3, 7, 5,
	2, 6, 3,
	3, 6, 7,
	4, 5, 7,
	4, 7, 6,
	0, 4, 2,
	2, 4, 6,
	0, 1, 4,
	1, 5, 4,
};

const Color4 cb2[] =
{
	{Color4(1.0f, 0.0f, 1.0f, 1.0f)},
	{Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	{Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	{Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	{Color4(1.0f, 1.0f, 0.0f, 1.0f)},
	{Color4(0.0f, 1.0f, 1.0f, 1.0f)},
};

class Cube : public RenderObj
{
public:
	Cube();
	~Cube();

	void Update(float deltaTime) override;
	void Draw() override;

private:
	ID3D11Buffer* mConstColorBuffer;

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

