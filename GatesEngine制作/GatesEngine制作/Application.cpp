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
	DirectX::XMMATRIX projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)window->GetWindowWidth() / window->GetWindowHeight(), 0.1f, 1000);
	DirectX::XMFLOAT3 eyepos = { 0,0,-100 };
	DirectX::XMFLOAT3 target = { 0,0,1};
	DirectX::XMFLOAT3 up = { 0,1,0 };
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));


	Vertex vertices[] =
	{
		{{  0.0f,   0.0f, 500.0f}},
		{{  50.0f,   0.0f,500.0f}},
		{{  50.0f,  50.0f,500.0f}},
	};
	unsigned short indices[] =
	{
		0,1,2,
	};
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resDesc.Width = sizeof(vertices);
	dxDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vb));
	vbView.BufferLocation = vb->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
	DirectX::XMFLOAT3* vtxMap = nullptr;
	vb->Map(0, nullptr, (void**)&vtxMap);
	for (int i = 0; i < _countof(vertices); i++)
	{
		vtxMap[i] = vertices[i].pos;
	}
	vb->Unmap(0, nullptr);

	resDesc.Width = sizeof(indices);
	dxDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ib));
	ibView.BufferLocation = ib->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
	unsigned short* idxMap = nullptr;
	ib->Map(0, nullptr, (void**)&idxMap);
	for (int i = 0; i < _countof(indices); i++)
	{
		idxMap[i] = indices[i];
	}

	resDesc.Width = (sizeof(ConstantBufferData) + 0xff)&~0xff;
	dxDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cb));
	ConstantBufferData* cstMap = nullptr;
	cb->Map(0, nullptr, (void**)&cstMap);
	cstMap->color = DirectX::XMFLOAT4(1, 0, 0, 1);
	cstMap->matrix = view * projection3D;
	cb->Unmap(0, nullptr);

	while (!keyboard->CheakHitKey(Key::ESCAPE))
	{
		dxDevice->ClearScreen({ 0,0,1,0 });
		keyboard->Update();
		dxSimplePso->Set(D3D12_FILL_MODE_SOLID);
		dxDevice->GetCmdList()->SetGraphicsRootConstantBufferView(0, cb->GetGPUVirtualAddress());
		dxDevice->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
		dxDevice->GetCmdList()->DrawInstanced(3, 1, 0, 0);
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