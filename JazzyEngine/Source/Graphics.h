#pragma once
#include <d3d11.h>
#include "VertexFormats.h"
#include "EngineMath.h"

class RenderObj;
class Shader;

class Graphics
{
public:
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

	enum TextureSlot
	{
		TEXTURE_SLOT_DIFFUSE,

		TEXTURE_SLOT_TOTAL
	};


	enum ConstantBuffer
	{
		CONSTANT_BUFFER_RENDEROBJ,
		CONSTANT_BUFFER_CAMERA,
		CONSTANT_BUFFER_CUBE_COLOR,
	};

	Graphics();
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	static Graphics* Get() { return sGraphics; }

	void InitD3D(HWND hWnd, float width, float height);

	void SetBuffer(ID3D11RenderTargetView* targetBuffer, ID3D11DepthStencilView* dsv);
	void ClearBuffer(const Color4& clearColor);
	void ClearBuffer(float red, float green, float blue);
	ID3D11RenderTargetView* GetBackBuffer() { return mBackBuffer; }

	IDXGISwapChain* GetSwapchain() { return mSwapChain; }

	void CreateRenderTargets(int width, int height);

	void EndFrame();

	void SetScreenWidth(float w) { screenWidth = w; }
	void SetScreenHeight(float h) { screenHeight = h; }
	// Getters for screen dimensions
	float GetScreenWidth() { return screenWidth; }
	float GetScreenHeight() { return screenHeight; }

	// Getters for D3D Device and Device Context
	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }

	// Create and upload a buffer
	ID3D11Buffer* CreateGraphicsBuffer(const void* initData, UINT byteWidth, UINT structByteStride, D3D11_BIND_FLAG inBindFlag, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage);
	void UploadBuffer(ID3D11Buffer* buffer, const void* data, UINT dataSize);

	// Viewport setter
	void SetViewport(float x, float y, float width, float height);

	// Create a depth-stencil state
	ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC inDepthComparisonFunction);

	// Create depth stencil
	bool CreateDepthStencil(int inWidth, int inHeight,
		ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView);

	// Getter for depth stencil view
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView; }

	// Clear z-buffer
	void ClearDepthBuffer(ID3D11DepthStencilView* depthView, float depth);

	void SetActiveTexture(int slot, ID3D11ShaderResourceView* pView);

	void SetActiveSampler(int slot, ID3D11SamplerState* pSampler);

private:
	static Graphics* sGraphics;

	float screenWidth;
	float screenHeight;

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;

	ID3D11RenderTargetView* mCurrentRenderTarget;
	ID3D11RenderTargetView* mBackBuffer;

	// Z-buffer
	ID3D11Texture2D* mDepthTexture;
	ID3D11DepthStencilView* mDepthStencilView;

	// Texture Sampler
	ID3D11SamplerState* mSamplerState;
};

