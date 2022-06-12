#pragma once

class Graphics;

// Different shader types
enum class ShaderType
{
	Vertex,
	Pixel,
};

class Shader
{
public:
	Shader();
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	bool Load(const WCHAR* fileName, ShaderType type, const D3D11_INPUT_ELEMENT_DESC* layoutArray, int numLayoutElements);

	void SetActive() const;

private:
	ID3D11VertexShader* mVS;
	ID3D11PixelShader* mPS;
	ID3D11InputLayout* mInputLayout;
	std::unordered_map<ShaderType, const char*> shaderModels;
};

