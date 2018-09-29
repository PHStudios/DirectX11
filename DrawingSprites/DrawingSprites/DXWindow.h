#pragma once

#include "Direct3D.h"
#include <wrl/client.h>

class DXWindow
{
public:
	DXWindow(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed);

	void Loop();
	void Update(float deltaTime);
	void Render();

	bool LoadContent();
	void UnloadContent();

protected:
	DXUtil::Color background;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

private:
	std::unique_ptr<Direct3D> direct3D;
};

