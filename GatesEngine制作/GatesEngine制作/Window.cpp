#include "window.h"

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::SetWindowSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Window::SetWindowTitle(const char * title)
{
	m_title = title;
}

void Window::SetWindow()
{
	/* ウィンドウの設定 */
	m_wnd.cbSize = sizeof(WNDCLASSEX);
	m_wnd.lpfnWndProc = (WNDPROC)WinProc;
	m_wnd.lpszClassName = m_title;
	m_wnd.hInstance = GetModuleHandle(nullptr);
	m_wnd.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* Windowの登録 */
	RegisterClassEx(&m_wnd);

	/* Windowの生成 */
	RECT l_rect = { 0,0,m_width,m_height };
	AdjustWindowRect(&l_rect, WS_OVERLAPPEDWINDOW, false);
	m_hwnd = CreateWindow(m_wnd.lpszClassName,
		m_wnd.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		l_rect.right - l_rect.left,
		l_rect.bottom - l_rect.top,
		nullptr,
		nullptr,
		m_wnd.hInstance,
		nullptr);
}

Window::Window(int width, int height, const char * title)
{
	SetWindowSize(width, height);
	SetWindowTitle(title);
	SetWindow();
}

Window::~Window()
{
	UnregisterClass(m_wnd.lpszClassName, m_wnd.hInstance);
}

HWND Window::GetHWND()
{
	return m_hwnd;
}

HINSTANCE Window::GetHINSTANCE()
{
	return m_wnd.hInstance;
}

int Window::GetWindowWidth()
{
	return m_width;
}

int Window::GetWindowHeight()
{
	return m_height;
}

void Window::DisplayWindow()
{
	ShowWindow(m_hwnd, SW_SHOW);
}

bool Window::ProcessMessage()
{
	if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
	if (m_msg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}

