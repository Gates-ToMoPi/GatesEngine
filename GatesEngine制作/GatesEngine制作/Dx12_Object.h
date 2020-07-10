#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include"Dx12_VertexData.h"
#include"Dx12_ConstBufferData.h"
#include"SafeRelease.h"
class Dx12_Object
{
private:
	ID3D12Resource* vtxBuffer;
	D3D12_VERTEX_BUFFER_VIEW vtxBufferView;
	ID3D12Resource* idxBuffer;
	D3D12_INDEX_BUFFER_VIEW idxBufferView;
	ID3D12Resource* cstBuffer;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	ConstantBufferData cbData;
	DirectX::XMMATRIX rotaMatrix;
	DirectX::XMMATRIX transMatrix;
private:
	void CreateVtxBuffer(ID3D12Device** ppDevice,const D3D12_HEAP_PROPERTIES& heapProp,D3D12_RESOURCE_DESC& resDesc);
	void CreateIdxBuffer(ID3D12Device** ppDevice,const D3D12_HEAP_PROPERTIES& heapProp,D3D12_RESOURCE_DESC& resDesc);
	void CreateCstBuffer(ID3D12Device** ppDevice,const D3D12_HEAP_PROPERTIES& heapProp,D3D12_RESOURCE_DESC& resDesc);
public:
	Dx12_Object(ID3D12Device* pDevice,const std::vector<Vertex>& argVertices,std::vector<unsigned short>& argIndices);
	~Dx12_Object();
	void Mapping3D(const TransformMatrix& transform,const DirectX::XMMATRIX& view,const DirectX::XMMATRIX projection3D,DirectX::XMFLOAT4 color);
	void Mapping2D(const DirectX::XMMATRIX& transform, const DirectX::XMMATRIX& projection2D, DirectX::XMFLOAT4 color);
	void Draw(ID3D12GraphicsCommandList** ppCmdList);
};

