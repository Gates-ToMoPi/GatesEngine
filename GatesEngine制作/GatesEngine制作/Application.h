#pragma once
#include"Window.h"
#include"Dx12_Device.h"
#include"Dx12_Pipeline.h"
#include"Keyboard.h"
class Application
{
private:
	Window* window;
	Dx12_Device* dxDevice;
	Dx12_Pipeline* dxSimplePso;
	Keyboard* keyboard;
public:
	Application();
	~Application();
	bool Initialize();
	void Run();
	void Terminate();
};

