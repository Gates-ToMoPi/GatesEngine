#pragma once
#include"Window.h"
#include"Dx12_Device.h"
#include"Dx12_Pipeline.h"
#include"Dx12_Object.h"
#include"Dx12_ConstBufferData.h"
#include"Dx12_VertexData.h"
#include"Keyboard.h"
class Application
{
private:
	Window* window;
	Dx12_Device* dxDevice;
	Dx12_Pipeline* dxSimplePso;
	Keyboard* keyboard;
	Dx12_Object* obj;
public:
	Application();
	~Application();
	bool Initialize();
	void Run();
	void Terminate();
};

