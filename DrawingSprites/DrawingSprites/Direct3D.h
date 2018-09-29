#pragma once

#include "d3d11.h"
#include "DXAdapter.h"
#include "DXUtil.h"
#include <wrl/client.h>
#include <SpriteBatch.h>
#include "WICTextureLoader.h"
#include "SimpleMath.h"
#include "CommonStates.h"

class Direct3D
{
public:
	Direct3D(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed);
	~Direct3D();

	void GetTexture(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* texture);

	void ClearRenderTarget(DXUtil::Color background);
	void BeginSpriteBatch();
	void DrawTexture(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 position);
	void EndSpriteBatch();
	void Present();

protected:
	void UpdateRefreshRateFromAdapter();
	bool CreateSwapChain();
	bool CreateRenderTarget();
	void CreateViewport();
	void GetCommonStates();

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
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::CommonStates> commonStates;

private:
	bool Initialize();
};

