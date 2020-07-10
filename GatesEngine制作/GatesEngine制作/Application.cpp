#include "Application.h"
Application::Application()
{
	window = new Window(1280, 720, "ƒGƒ“ƒWƒ“§ì");
	dxDevice = new Dx12_Device();
	dxSimplePso = new Dx12_Pipeline(dxDevice->GetDeviceAddress(), dxDevice->GetCmdListAddress());
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
	DirectX::XMMATRIX projection2D = DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1);
	DirectX::XMMATRIX projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)window->GetWindowWidth() / window->GetWindowHeight(), 0.1f, 10000);
	DirectX::XMFLOAT3 eyepos = { 0,0,0 };
	DirectX::XMFLOAT3 target = { 0,0,1 };
	DirectX::XMFLOAT3 up = { 0,1,0 };
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	vertices.push_back({ { -50.0f,  50.0f, 0.0f } });
	vertices.push_back({ {  50.0f,  50.0f, 0.0f } });
	vertices.push_back({ {  50.0f, -50.0f, 0.0f } });
	vertices.push_back({ { -50.0f, -50.0f, 0.0f } });
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	TransformMatrix trans = { DirectX::XMMatrixIdentity(),DirectX::XMMatrixIdentity(),DirectX::XMMatrixIdentity() };
	obj = new Dx12_Object(dxDevice->GetDevice(),vertices, indices);
	obj->Mapping3D(trans, view, projection3D, {1,0,0,1});
	DirectX::XMFLOAT3 pos(0,0,500);

	while (!keyboard->CheakHitKey(Key::ESCAPE))
	{
		static DirectX::XMFLOAT3 angle = {};
		angle.y += 0.01f;;
		if (keyboard->CheakHitKey(Key::RIGHT))
		{
			pos.x++;
		}
		if (keyboard->CheakHitKey(Key::LEFT))
		{
			pos.x--;
		}
		if (keyboard->CheakHitKey(Key::UP))
		{
			pos.y++;
		}
		if (keyboard->CheakHitKey(Key::DOWN))
		{
			pos.y--;
		}
		trans.rotation = DirectX::XMMatrixIdentity();
		trans.rotation *= DirectX::XMMatrixRotationZ(0);
		trans.rotation *= DirectX::XMMatrixRotationX(0);
		trans.rotation *= DirectX::XMMatrixRotationY(angle.y);
		trans.translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
		obj->Mapping3D(trans, view, projection3D, { 1,0,0,1 });
		dxDevice->ClearScreen({ 0,0,1,0 });
		keyboard->Update();
		dxSimplePso->Set(D3D12_FILL_MODE_SOLID);
		obj->Draw(dxDevice->GetCmdListAddress());
		dxDevice->Present();
		if (!window->ProcessMessage()) break;
	}
}

void Application::Terminate()
{
	delete keyboard;
	delete obj;
	delete dxSimplePso;
	delete dxDevice;
	delete window;
}