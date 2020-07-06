#pragma once
#include"Window.h"
#include"Keyboard.h"
class Application
{
private:
	Window* window;
	Keyboard* keyboard;
public:
	Application();
	~Application();
	bool Initialize();
	void Run();
	void Terminate();
};

