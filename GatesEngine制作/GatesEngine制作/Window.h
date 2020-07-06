#pragma once
#include<Windows.h>
class Window
{
private:
	HWND m_hwnd;
	WNDCLASSEX m_wnd;
	MSG m_msg;
	int m_width;
	int m_height;
	const char* m_title;
private:
	void SetWindowSize(int width, int height);
	void SetWindowTitle(const char* title);
	void SetWindow();
public:
	Window(int width, int height, const char* title);
	~Window();
	HWND GetHWND();
	HINSTANCE GetHINSTANCE();
	int GetWindowWidth();
	int GetWindowHeight();
	void DisplayWindow();
	bool ProcessMessage();
};