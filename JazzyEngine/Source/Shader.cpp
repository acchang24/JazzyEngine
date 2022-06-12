#include "stdafx.h"
#include "Shader.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dCompiler.lib")

Shader::Shader()
    : mVS(nullptr)
    , mPS(nullptr)
    , mInputLayout(nullptr)
{
    shaderModels[ShaderType::Vertex] = "vs_4_0";
    shaderModels[ShaderType::Pixel] = "ps_4_0";
}

Shader::~Shader()
{
    if (mVS != nullptr)
    {
        mVS->Release();
    }
    if (mPS != nullptr)
    {
        mPS->Release();
    }
    if (mInputLayout != nullptr)
    {
        mInputLayout->Release();
    }
}

static bool LoadShader(const WCHAR* filename, const char* entryPoint, const char* model, ID3DBlob*& pBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, model,
        dwShaderFlags, 0, &pBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            static wchar_t szBuffer[4096];
            _snwprintf_s(szBuffer, 4096, _TRUNCATE,
                L"%hs",
                (char*)pErrorBlob->GetBufferPointer());
            OutputDebugStringW(szBuffer);
#ifdef _WINDOWS
            MessageBox(nullptr, szBuffer, L"Error", MB_OK);
#endif
            pErrorBlob->Release();
        }
        return false;
    }
    if (pErrorBlob)
    {
        pErrorBlob->Release();
    }

    return true;
}

bool Shader::Load(const WCHAR* fileName, ShaderType type, const D3D11_INPUT_ELEMENT_DESC* layoutArray, int numLayoutElements)
{
    Graphics* g = Graphics::Get();

    HRESULT hr = S_OK;
    ID3DBlob* pBlob = nullptr;
    if (LoadShader(fileName, "main", shaderModels[type], pBlob))
    {
        switch (type)
        {
        case ShaderType::Vertex:
            hr = g->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &mVS);
            DbgAssert(hr == S_OK, "Failed to create vertex shader");
            break;
        case ShaderType::Pixel:
            hr = g->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &mPS);
            DbgAssert(hr == S_OK, "Failed to create pixel shader");
            break;
        }
        if (mInputLayout == nullptr)
        {
            hr = g->GetDevice()->CreateInputLayout(layoutArray, numLayoutElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &mInputLayout);
            DbgAssert(hr == S_OK, "Failed to create input layout");
        }
        if (S_OK == hr)
            return true;
    }
    hr = S_FALSE;
    DbgAssert(hr == S_OK, "Could not load the shader file");
    return false;
}

void Shader::SetActive() const
{
    Graphics* g = Graphics::Get();
    g->GetContext()->VSSetShader(mVS, nullptr, 0);
    g->GetContext()->PSSetShader(mPS, nullptr, 0);
    g->GetContext()->IASetInputLayout(mInputLayout);
}