#include "DXWindow.h"


DXWindow::DXWindow(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed)
{
	direct3D.reset(new Direct3D(hInstance, hWnd, canvas, windowed));
	background = { 125 / 255.0f, 108 / 255.0f, 76 / 255.0f, 1.0f };
}

bool DXWindow::LoadContent()
{
	return true;
}

void DXWindow::UnloadContent()
{
}

void DXWindow::Loop()
{
	Update(0.0);
	Render();
}

void DXWindow::Update(float dt)
{
}

void DXWindow::Render()
{
	direct3D->ClearRenderTarget(background);

	direct3D->Present();
}

