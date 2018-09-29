#include "DXAdapter.h"
#include <stdexcept>



DXAdapter::DXAdapter()
{
	if (!Initialize()) throw std::runtime_error("Unable to create the DXAdapter");
}

bool DXAdapter::Initialize()
{
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
	if(FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if(FAILED(result))
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numDisplayModes, nullptr);
	if(FAILED(result))
	{
		return false;
	}

	modes.reset(new DXGI_MODE_DESC[numDisplayModes]);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numDisplayModes, modes.get());

	return !FAILED(result);

}

