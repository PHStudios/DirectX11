#include <Windows.h>
#include "DXUtil.h"
#include "DXWindow.h"
#include <stdexcept>

HWND CreateApplicationWindow(HINSTANCE hInstance, LPCSTR className, LPCSTR windowTitle, DXUtil::Canvas canvas);
WNDCLASSEX CreateWindowProperties(HINSTANCE hInstance, LPCSTR className);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateDirectXWindow(HINSTANCE hInstance, HWND hWnd, std::unique_ptr<DXWindow>* dxWindow, bool windowed);
int ApplicationLoop(DXWindow* dxWindow);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	const DXUtil::Canvas canvas = { 1280, 720 };
	std::unique_ptr<DXWindow> dxWindow;
	bool windowed = true;
	int result = 0;

	const auto hWnd = CreateApplicationWindow(hInstance, "DirectX11Window", "First DirectX 11 Window", canvas);

	if (!hWnd) return -1;

	ShowWindow(hWnd, cmdShow);

	try
	{
		CreateDirectXWindow(hInstance, hWnd, &dxWindow, windowed);
		result = ApplicationLoop(dxWindow.get());
	}
	catch (std::runtime_error &ex)
	{
		MessageBox(hWnd, ex.what(), nullptr, NULL);
		result = -1;
	}

	return result;
}

HWND CreateApplicationWindow(HINSTANCE hInstance, LPCSTR className, LPCSTR windowTitle, DXUtil::Canvas canvas)
{
	RECT rc = { 0, 0, canvas.Width, canvas.Height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	auto wndClassex = CreateWindowProperties(hInstance, className);

	if (!RegisterClassEx(&wndClassex)) return nullptr;

	return CreateWindowA(className, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);
}

WNDCLASSEX CreateWindowProperties(HINSTANCE hInstance, LPCSTR className)
{
	WNDCLASSEX wndClassex = { 0 };
	wndClassex.cbSize = sizeof(WNDCLASSEX);
	wndClassex.style = CS_HREDRAW | CS_VREDRAW;
	wndClassex.lpfnWndProc = WndProc;
	wndClassex.hInstance = hInstance;
	wndClassex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClassex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wndClassex.lpszMenuName = nullptr;
	wndClassex.lpszClassName = className;

	return wndClassex;
}

void CreateDirectXWindow(HINSTANCE hInstance, HWND hWnd, std::unique_ptr<DXWindow>* dxWindow, bool windowed)
{
	RECT dimensions;
	GetClientRect(hWnd, &dimensions);

	const DXUtil::Canvas canvas = { dimensions.right - dimensions.left, dimensions.bottom - dimensions.top };

	(*dxWindow).reset(new DXWindow(hInstance, hWnd, canvas, windowed));
}

int ApplicationLoop(DXWindow* dxWindow)
{
	MSG msg = { nullptr };

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		dxWindow->Loop();
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}