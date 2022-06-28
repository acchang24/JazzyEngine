#pragma once
#include "EngineMath.h"

class Color4
{
public:
    Color4()
        : r(0), g(0), b(0), a(0)
    {}
    Color4(float _r, float _g, float _b, float _a = 1.0f)
        : r(_r), g(_g), b(_b), a(_a)
    {}
    float r, g, b, a;
};

struct Vertex
{
    Vector3 pos;
    Color4 color;
};

struct VertexTexture
{
    Vector3 pos;
    Vector2 uv;
};

struct VertexPosNormUV
{
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

struct VertexPosNormColorUV
{
    Vector3 pos;
    Vector3 normal;
    Color4 color;
    Vector2 uv;
};