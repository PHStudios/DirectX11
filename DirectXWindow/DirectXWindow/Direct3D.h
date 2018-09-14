#pragma once

#include "d3d11.h"
#include "DXAdapter.h"
#include "DXUtil.h"
#include <wrl/client.h>

class Direct3D
{
public:
	Direct3D(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed);
	~Direct3D();

	void ClearRenderTarget(DXUtil::Color background);
	void Present();

protected:
	void UpdateRefreshRateFromAdapter();
	bool CreateSwapChain();
	bool CreateRenderTarget();
	void CreateViewport();

	HINSTANCE hInstance;
	HWND hWnd;
	DXUtil::RefreshRate refreshRate{};
	unsigned int width;
	unsigned int height;
	bool windowed;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferTarget;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3DContext;
	Microsoft::WRL::ComPtr<ID3D11Device> d3DDevice;

	std::unique_ptr<DXAdapter> adapter;

private:
	bool Initialize();
};

