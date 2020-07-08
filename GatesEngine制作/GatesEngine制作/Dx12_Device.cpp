#include "Dx12_Device.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void Dx12_Device::Create_Viewport(int window_width, int window_height)
{
	m_viewport.Width = (float)window_width;
	m_viewport.Height = (float)window_height;
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.MinDepth = 0.0f;
}

void Dx12_Device::Create_Scissorrect(int window_width, int window_height)
{
	m_scissorrect.left = 0;
	m_scissorrect.right = m_scissorrect.left + window_width;
	m_scissorrect.top = 0;
	m_scissorrect.bottom = m_scissorrect.top + window_height;
}

bool Dx12_Device::Create_DXGIfactory()
{
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory))))
	{
		return false;
	}
	return true;
}

bool Dx12_Device::Create_Device()
{
	/* デバイスの列挙 */
	for (int i = 0; m_dxgiFactory->EnumAdapters(i, &m_useAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		m_enumAdapter.push_back(m_useAdapter);
	}
	/* GPUを検索 */
	DXGI_ADAPTER_DESC adapterDesc;
	for (int i = 0; i < m_enumAdapter.size(); i++)
	{
		m_enumAdapter[i]->GetDesc(&adapterDesc);
		std::wstring name = adapterDesc.Description;
		if (name.find(L"Microsoft") == std::wstring::npos && name.find(L"Intel") == std::wstring::npos)
		{
			m_useAdapter = m_enumAdapter[i];
			break;
		}
	}
	/* 機能レベルの列挙と設定 */
	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	for (int i = 0; i < _countof(featureLevel); i++)
	{
		if (SUCCEEDED(D3D12CreateDevice(m_useAdapter, featureLevel[i], IID_PPV_ARGS(&m_device))))
		{
			break;
		}
	}
	return true;
}

bool Dx12_Device::Create_CommandList()
{
	/* コマンドアロケータの生成 */
	if (FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAlloc))))
	{
		return false;
	}
	/* コマンドリストの生成 */
	if (FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAlloc, nullptr, IID_PPV_ARGS(&m_cmdList))))
	{
		return false;
	}
	/* コマンドキューの生成 */
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	if (FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_cmdQueue))))
	{
		return false;
	}
	return true;
}

bool Dx12_Device::Create_SwapChain(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = (UINT)m_viewport.Width;
	swapChainDesc.Height = (UINT)m_viewport.Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(m_dxgiFactory->CreateSwapChainForHwnd(m_cmdQueue, hwnd, &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)&m_swapChain)))
	{
		return false;
	}
	return true;
}

bool Dx12_Device::Create_RTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.NumDescriptors = 2;
	if (FAILED(m_device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_rtvHeap))))return false;
	m_frameBuffer.resize(2);
	for (int i = 0; i < m_frameBuffer.size(); i++)
	{
		if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_frameBuffer[i]))))return false;
		D3D12_CPU_DESCRIPTOR_HANDLE descHandle = {};
		descHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		descHandle.ptr += i * m_device->GetDescriptorHandleIncrementSize(descHeapDesc.Type);
		m_device->CreateRenderTargetView(m_frameBuffer[i], nullptr, descHandle);
	}
	return true;
}

bool Dx12_Device::Create_Fence()
{
	if (FAILED(m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
	{
		return false;
	}
	return true;
}

void Dx12_Device::SetResourceBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = m_frameBuffer[m_swapChain->GetCurrentBackBufferIndex()];
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	m_cmdList->ResourceBarrier(1, &barrier);
}

Dx12_Device::Dx12_Device()
{
#ifdef _DEBUG
	/* DebugLayerのオン */
	ID3D12Debug* debuglayer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debuglayer))))
	{
		debuglayer->EnableDebugLayer();
	}
	SAFE_RELEASE(debuglayer);
#endif
}

Dx12_Device::~Dx12_Device()
{
	SAFE_RELEASE(m_dxgiFactory);
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_cmdAlloc);
	SAFE_RELEASE(m_cmdList);
	SAFE_RELEASE(m_cmdQueue);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_rtvHeap);
	for (int i = 0; i < m_frameBuffer.size(); ++i)
	{
		SAFE_RELEASE(m_frameBuffer[i]);
	}
	SAFE_RELEASE(m_fence);
}

bool Dx12_Device::Init(HWND hwnd, int window_width, int window_height)
{
	Create_Viewport(window_width, window_height);
	Create_Scissorrect(window_width, window_height);
	/* DirectXの初期化 */
	if (!Create_DXGIfactory()) return false;
	if (!Create_Device()) return false;
	if (!Create_CommandList()) return false;
	if (!Create_SwapChain(hwnd)) return false;
	if (!Create_RTV()) return false;
	if (!Create_Fence()) return false;
	return true;
}

void Dx12_Device::ClearScreen(DirectX::XMFLOAT4 color)
{
	SetResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	/* 画面クリアコマンド */
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += m_swapChain->GetCurrentBackBufferIndex() * m_device->GetDescriptorHandleIncrementSize(m_rtvHeap->GetDesc().Type);
	m_cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	float rgba[] = { color.x,color.y,color.z,color.w };
	m_cmdList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);
	m_cmdList->RSSetViewports(1, &m_viewport);
	m_cmdList->RSSetScissorRects(1, &m_scissorrect);
	m_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Dx12_Device::Present()
{
	SetResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	/* コマンドのクローズ */
	m_cmdList->Close();
	/* コマンドの実行 */
	ID3D12CommandList* cmdLists[] = { m_cmdList };
	m_cmdQueue->ExecuteCommandLists(1, cmdLists);
	m_cmdQueue->Signal(m_fence, ++m_fenceValue);
	if (m_fence->GetCompletedValue() != m_fenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	m_cmdAlloc->Reset();
	m_cmdList->Reset(m_cmdAlloc, nullptr);
	m_swapChain->Present(1, 0);
}

ID3D12Device ** Dx12_Device::GetDeviceAddress()
{
	return &m_device;
}

ID3D12Device * Dx12_Device::GetDevice()
{
	return m_device;
}

ID3D12GraphicsCommandList ** Dx12_Device::GetCmdListAddress()
{
	return &m_cmdList;
}

ID3D12GraphicsCommandList * Dx12_Device::GetCmdList()
{
	return m_cmdList;
}
