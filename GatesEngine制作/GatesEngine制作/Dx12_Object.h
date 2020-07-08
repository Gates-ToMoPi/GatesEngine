#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include"SafeRelease.h"
class Dx12_Object
{
private:
	ID3D12Resource* vtxBuffer;
	D3D12_VERTEX_BUFFER_VIEW vtxBufferView;
	ID3D12Resource* idxBuffer;
	D3D12_INDEX_BUFFER_VIEW idxBufferView;
	ID3D12Resource* cstBuffer;
public:
	Dx12_Object();
	~Dx12_Object();
	void Mapping3D(const DirectX::XMMATRIX& world,const DirectX::XMMATRIX& view,const DirectX::XMMATRIX projection);
	void Mapping2D(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& projection);
	void Draw(ID3D12GraphicsCommandList** ppCmdList);
};

