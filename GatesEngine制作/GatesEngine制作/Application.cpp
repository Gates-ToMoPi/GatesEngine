#include "Application.h"

Application::Application()
{
	window = new Window(1280, 720, "ƒGƒ“ƒWƒ“§ì");
	dxDevice = new Dx12_Device();
	dxSimplePso = new Dx12_Pipeline(dxDevice->GetDevice(), dxDevice->GetCmdList());
	keyboard = new Keyboard(window->GetHWND(), window->GetHINSTANCE());
}

Application::~Application()
{
}

bool Application::Initialize()
{
	window->DisplayWindow();
	if (!dxDevice->Init(window->GetHWND(), window->GetWindowWidth(), window->GetWindowHeight()))return false;
	if (!dxSimplePso->LoadShaderFile(L"BasicVertexShader.hlsl", "VSmain", "vs_5_0", ShaderType::SHADER_TYPE_VS))return false;
	if (!dxSimplePso->LoadShaderFile(L"BasicPixelShader.hlsl", "PSmain", "ps_5_0", ShaderType::SHADER_TYPE_PS))return false;
	if (!dxSimplePso->Create_Pipeline())return false;
	return true;
}

void Application::Run()
{
	while (!keyboard->CheakHitKey(Key::ESCAPE))
	{
		dxDevice->ClearScreen({ 0,0,1,0 });
		keyboard->Update();
		dxSimplePso->Set(D3D12_FILL_MODE_SOLID);
		dxDevice->Present();
		if (!window->ProcessMessage()) break;
	}
}

void Application::Terminate()
{
	delete keyboard;
	delete dxSimplePso;
	delete dxDevice;
	delete window;
}
