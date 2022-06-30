#pragma once
#include "EngineMath.h"

#include "RenderObj.h"

class Sphere : public RenderObj
{
public:
	Sphere();
	~Sphere();
	Sphere(const Sphere&) = delete;
	Sphere& operator=(const Sphere&) = delete;

	void CreateSphere();


private:

};

