#include "stdafx.h"
#include <d3dcompiler.h>
#include <cmath>
#include "Graphics.h"
#include "Shader.h"
#include "RenderObj.h"
#include "VertexBuffer.h"

#pragma comment (lib, "d3d11.lib") 
#pragma comment(lib, "d3dCompiler.lib")

Graphics* Graphics::sGraphics = nullptr;

Graphics::Graphics()
	: mSwapChain(nullptr)
	, mDevice(nullptr)
	, mContext(nullptr)
	, mCurrentRenderTarget(nullptr)
	, mBackBuffer(nullptr)
	, mDepthTexture(nullptr)
	, mDepthStencilView(nullptr)
	, mSamplerState(nullptr)
	, screenWidth(0.0f)
	, screenHeight(0.0f)
{
	sGraphics = this;
}

Graphics::~Graphics()
{
	sGraphics = nullptr;
#ifdef _DEBUG
	ID3D11Debug* pDbg = nullptr;
	HRESULT hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDbg));
	DbgAssert(S_OK == hr, "Unable to create debug device");
#endif

	if (mBackBuffer)
	{
		mBackBuffer->Release();
	}
	if (mContext)
	{
		mContext->Release();
	}
	if (mSwapChain)
	{
		mSwapChain->Release();
	}
	if (mDevice)
	{
		mDevice->Release();
	}
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
	}
	if (mDepthTexture)
	{
		mDepthTexture->Release();
	}
	if (mSamplerState)
	{
		mSamplerState->Release();
	}

#ifdef _DEBUG
	pDbg->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
	pDbg->Release();
#endif
}

void Graphics::InitD3D(HWND hWnd, float width, float height)
{
	screenWidth = width;
	screenHeight = height;

	HRESULT hr = S_OK;

	// Setup device and swap chain
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = static_cast<UINT>(width);
		sd.BufferDesc.Height = static_cast<UINT>(height);
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = 0;

		// Create device, front and back buffers, swap chain, and rendering context
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&mSwapChain,
			&mDevice,
			nullptr,
			&mContext);
		DbgAssert(hr == S_OK, "Failed to create device");
	}

	// Set the viewport
	SetViewport(0.0f, 0.0f, width, height);

	// Grab the back buffer (access texture subresource in swap chain)
	{
		ID3D11Resource* pBackBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&pBackBuffer);
		DbgAssert(hr == S_OK, "Something wrong with back buffer");
		// Create a render target view with that resource
		hr = mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mBackBuffer);
		DbgAssert(hr == S_OK, "Something went wrong while creating a render target view");
		// Release the temporary resource
		pBackBuffer->Release();
	}

	// Draw triangle lists(groups of 3 vertices)
	mContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Z Buffer
	{
		// Create Depth Stencil State
		ID3D11DepthStencilState* depthState = CreateDepthStencilState(D3D11_COMPARISON_LESS);
		// Set Depth Stencil State
		mContext->OMSetDepthStencilState(depthState, 0);
		// Release depth state
		depthState->Release();

		// Create Depth Stencil
		CreateDepthStencil((int)width, (int)height, &mDepthTexture, &mDepthStencilView);
	}

	// Sampler State
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Initialize Sampler State
		mDevice->CreateSamplerState(&sampDesc, &mSamplerState);
		// Set Active Sampler
		SetActiveSampler(0, mSamplerState);
	}
}


void Graphics::SetBuffer(ID3D11RenderTargetView* targetBuffer, ID3D11DepthStencilView* dsv)
{
	mCurrentRenderTarget = targetBuffer;
	mContext->OMSetRenderTargets(1, &mCurrentRenderTarget, dsv);
}

void Graphics::ClearBuffer(const Color4& clearColor)
{
	// Clears screen to a certain color
	mContext->ClearRenderTargetView(mCurrentRenderTarget, reinterpret_cast<const float*>(&clearColor));
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	mContext->ClearRenderTargetView(mCurrentRenderTarget, color);
}

void Graphics::EndFrame()
{
	// Rendering with v-sync
	//mSwapChain->Present(1, 0);
	
	// Rendering without v-sync
	mSwapChain->Present(0, 0);
}

ID3D11Buffer* Graphics::CreateGraphicsBuffer(const void* initData, UINT byteWidth, UINT structByteStride, D3D11_BIND_FLAG inBindFlag, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage)
{
	UINT test = sizeof(initData);
	ID3D11Buffer* buffer = nullptr;

	// Setup buffer description
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = inBindFlag;
	bd.Usage = inUsage;
	bd.ByteWidth = byteWidth;
	bd.CPUAccessFlags = inCPUAccessFlags;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = structByteStride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = initData;

	HRESULT hr = mDevice->CreateBuffer(&bd, &sd, &buffer);
	DbgAssert(hr == S_OK, "Unable to create a graphics buffer");

	if (initData != nullptr)
	{
		// upload buffer info to gpu
		UploadBuffer(buffer, initData, byteWidth);
	}

	return buffer;
}

void Graphics::UploadBuffer(ID3D11Buffer* buffer, const void* data, UINT dataSize)
{
	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	DbgAssert(hr == S_OK, "Map failed");
	memcpy(map.pData, data, dataSize);
	mContext->Unmap(buffer, 0);
}

void Graphics::SetViewport(float x, float y, float width, float height)
{
	// Set Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	mContext->RSSetViewports(1, &vp);
}

ID3D11DepthStencilState* Graphics::CreateDepthStencilState(D3D11_COMPARISON_FUNC inDepthComparisonFunction)
{
	// Creates a depth-stencil state
	// Depth parameters
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = inDepthComparisonFunction;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	HRESULT hr = mDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	DbgAssert(hr == S_OK, "Could not create depth stencil state");


	return pDSState;
}

bool Graphics::CreateDepthStencil(int inWidth, int inHeight, ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView)
{
	// Create Texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = inWidth;
	descDepth.Height = inHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = mDevice->CreateTexture2D(&descDepth, NULL, pDepthTexture);
	DbgAssert(hr == S_OK, "Could not create texture 2d");

	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = mDevice->CreateDepthStencilView(*pDepthTexture,	// Depth stencil texture
		&descDSV,											// Depth stencil desc
		pDepthView);										// [out] Depth stencil view
	DbgAssert(hr == S_OK, "Could not create depth stencil view");


	// Bind the depth stencil view
	mContext->OMSetRenderTargets(1,							// One rendertarget view
		&mCurrentRenderTarget,								// Render target view, created earlier
		*pDepthView);										// Depth stencil view for the render target

	return true;
}

void Graphics::ClearDepthBuffer(ID3D11DepthStencilView* depthView, float depth)
{
	mContext->ClearDepthStencilView(depthView, 1, depth, 1);
}

void Graphics::SetActiveTexture(int slot, ID3D11ShaderResourceView* pView)
{
	mContext->PSSetShaderResources(slot, 1, &pView);
}

void Graphics::SetActiveSampler(int slot, ID3D11SamplerState* pSampler)
{
	mContext->PSSetSamplers(slot, 1, &pSampler);
}
