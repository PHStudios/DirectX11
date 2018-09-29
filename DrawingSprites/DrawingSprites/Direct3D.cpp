#include "Direct3D.h"
#include "DXAdapter.h"
#include <stdexcept>




Direct3D::Direct3D(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed)
{
	this->hInstance = hInstance;
	this->hWnd = hWnd;
	this->width = canvas.Width;
	this->height = canvas.Height;
	this->windowed = windowed;
	this->refreshRate.Numerator = 60;
	this->refreshRate.Denominator = 1;

	driverType = D3D_DRIVER_TYPE_NULL;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (!Initialize()) throw std::runtime_error("Unable to initialize Direct3D object");
}


Direct3D::~Direct3D()
{
	hInstance = nullptr;
	hWnd = nullptr;
}

bool Direct3D::Initialize()
{
	adapter.reset(new DXAdapter());

	UpdateRefreshRateFromAdapter();

	if(!CreateSwapChain())
	{
		return false;
	}

	if(!CreateRenderTarget())
	{
		return false;
	}

	CreateViewport();
	GetCommonStates();

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(d3DContext.Get());

	return true;
}

void Direct3D::UpdateRefreshRateFromAdapter()
{
	if (windowed) return;

	for(UINT i = 0; i < adapter->numDisplayModes; i++)
	{
		const auto currentMode = adapter->modes.get()[i];
		if(currentMode.Width == width && currentMode.Height == height
			&& (currentMode.RefreshRate.Numerator / currentMode.RefreshRate.Denominator) > (refreshRate.Numerator / refreshRate.Denominator))
		{
			refreshRate.Numerator = currentMode.RefreshRate.Numerator;
			refreshRate.Denominator = currentMode.RefreshRate.Denominator;
		}
	}
}

bool Direct3D::CreateSwapChain()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP
	};

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	const unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRate.Numerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRate.Denominator;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = windowed;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	for(auto& driverType : driverTypes)
	{
		const HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, 0, featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, swapChain.ReleaseAndGetAddressOf(), d3DDevice.ReleaseAndGetAddressOf(), &featureLevel,
			d3DContext.ReleaseAndGetAddressOf());

		if(SUCCEEDED(result))
		{
			this->driverType = driverType;
			return true;
		}
	}

	return false;
}

bool Direct3D::CreateRenderTarget()
{
	ID3D11Texture2D* backBufferTexture = nullptr;
	HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferTexture));

	if(FAILED(result))
	{
		return false;
	}

	result = d3DDevice->CreateRenderTargetView(backBufferTexture, nullptr, backBufferTarget.ReleaseAndGetAddressOf());
	backBufferTexture->Release();
	backBufferTexture = nullptr;

	if(FAILED(result))
	{
		return false;
	}

	d3DContext->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);
	return true;
}

void Direct3D::CreateViewport()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	d3DContext->RSSetViewports(1, &viewport);
}

void Direct3D::GetCommonStates()
{
	commonStates.reset(new DirectX::CommonStates(d3DDevice.Get()));
}

void Direct3D::GetTexture(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* texture)
{
	DirectX::CreateWICTextureFromFile(d3DDevice.Get(), fileName, nullptr, (*texture).ReleaseAndGetAddressOf());
}



void Direct3D::ClearRenderTarget(DXUtil::Color background)
{
	if (d3DContext == nullptr) return;

	float color[4] = { background.Red, background.Green, background.Blue, background.Alpha };

	d3DContext->ClearRenderTargetView(backBufferTarget.Get(), color);
}

void Direct3D::BeginSpriteBatch()
{
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, commonStates->NonPremultiplied());
}

void Direct3D::DrawTexture(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 position)
{
	spriteBatch->Draw(texture, position);
}

void Direct3D::EndSpriteBatch()
{
	spriteBatch->End();
}




void Direct3D::Present()
{
	swapChain->Present(0, 0);
}







