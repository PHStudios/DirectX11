#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

class DXAdapter
{
public:
	DXAdapter();

	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;

	UINT numDisplayModes{};

	std::unique_ptr<DXGI_MODE_DESC> modes;

private:
	bool Initialize();
};

