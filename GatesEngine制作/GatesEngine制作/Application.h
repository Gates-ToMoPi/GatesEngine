#pragma once
#include"Window.h"
#include"Dx12_Device.h"
#include"Dx12_Pipeline.h"
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

	ID3D12Resource* vb;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	ID3D12Resource* ib;
	D3D12_INDEX_BUFFER_VIEW ibView;
	ID3D12Resource* cb;
public:
	Application();
	~Application();
	bool Initialize();
	void Run();
	void Terminate();
};

