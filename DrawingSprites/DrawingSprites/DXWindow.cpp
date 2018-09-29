#include "DXWindow.h"
#include "SimpleMath.h"


DXWindow::DXWindow(HINSTANCE hInstance, HWND hWnd, DXUtil::Canvas canvas, bool windowed)
{
	direct3D.reset(new Direct3D(hInstance, hWnd, canvas, windowed));
	background = { 125 / 255.0f, 108 / 255.0f, 76 / 255.0f, 1.0f };
}

bool DXWindow::LoadContent()
{
	direct3D->GetTexture(L"PlayerPaper.png", &texture);

	return texture.Get() != nullptr;
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

	direct3D->BeginSpriteBatch();

	direct3D->DrawTexture(texture.Get(), DirectX::SimpleMath::Vector2(200,200));

	direct3D->EndSpriteBatch();

	direct3D->Present();
}

