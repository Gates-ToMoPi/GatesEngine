#pragma once
#include"SafeRelease.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
class Dx12_Device
{
private:
	IDXGIFactory6*               m_dxgiFactory;
	ID3D12Device*                m_device;
	std::vector<IDXGIAdapter*>   m_enumAdapter;
	IDXGIAdapter*                m_useAdapter;
	ID3D12CommandAllocator*      m_cmdAlloc;
	ID3D12GraphicsCommandList*   m_cmdList;
	ID3D12CommandQueue*          m_cmdQueue;
	IDXGISwapChain4*             m_swapChain;
	ID3D12DescriptorHeap*        m_rtvHeap;
	std::vector<ID3D12Resource*> m_frameBuffer;
	ID3D12Fence*                 m_fence;
	UINT64                       m_fenceValue;
	D3D12_VIEWPORT               m_viewport;
	D3D12_RECT                   m_scissorrect;
private:
	/* 各オブジェクトの生成処理 */
	void Create_Viewport(int window_width, int window_height);
	void Create_Scissorrect(int window_width, int window_height);
	bool Create_DXGIfactory();
	bool Create_Device();
	bool Create_CommandList();
	bool Create_SwapChain(HWND hwnd);
	bool Create_RTV();
	bool Create_Fence();
	/* リソースバリアの設定 */
	void SetResourceBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
public:
	Dx12_Device();
	~Dx12_Device();
	bool Init(HWND hwnd, int window_width, int window_height); // 初期化
	void ClearScreen(DirectX::XMFLOAT4 color); // 画面クリア
	void Present(); // コマンドの実行と画面フリップ
	ID3D12Device** GetDeviceAddress();
	ID3D12Device* GetDevice();
	ID3D12GraphicsCommandList** GetCmdListAddress();
	ID3D12GraphicsCommandList* GetCmdList();
};