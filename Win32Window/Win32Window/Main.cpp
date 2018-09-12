#include <Windows.h>

HWND CreateApplicationWindow(HINSTANCE hInstance, LPCSTR className, LPCSTR windowTitle, LONG width, LONG height);
WNDCLASSEX CreateWindowProperties(HINSTANCE hInstance, LPCSTR className);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WPARAM ApplicationLoop();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	auto hWnd = CreateApplicationWindow(hInstance, "Win32Window", "First Win32 Window in C++", 1280, 720);

	if (!hWnd) return -1;

	ShowWindow(hWnd, cmdShow);

	const auto wParam = ApplicationLoop();

	return static_cast<int>(wParam);
}

HWND CreateApplicationWindow(HINSTANCE hInstance, LPCSTR className, LPCSTR windowTitle, LONG width, LONG height)
{
	RECT rc = { 0, 0, width, height };
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

WPARAM ApplicationLoop()
{
	MSG msg = { nullptr };

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
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