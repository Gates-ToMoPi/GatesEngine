#include "Dx12_Object.h"

void Dx12_Object::CreateVtxBuffer(ID3D12Device ** ppDevice, const D3D12_HEAP_PROPERTIES & heapProp, D3D12_RESOURCE_DESC & resDesc)
{
	resDesc.Width = sizeof(vertices[0])*vertices.size();
	(*ppDevice)->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vtxBuffer));
	vtxBufferView.BufferLocation = vtxBuffer->GetGPUVirtualAddress();
	vtxBufferView.SizeInBytes = sizeof(vertices[0])*vertices.size();
	vtxBufferView.StrideInBytes = sizeof(vertices[0]);
	DirectX::XMFLOAT3* vtxMap = nullptr;
	vtxBuffer->Map(0, nullptr, (void**)&vtxMap);
	for (int i = 0; i < vertices.size(); i++)
	{
		vtxMap[i] = vertices[i].pos;
	}
	vtxBuffer->Unmap(0, nullptr);
}

void Dx12_Object::CreateIdxBuffer(ID3D12Device ** ppDevice, const D3D12_HEAP_PROPERTIES & heapProp, D3D12_RESOURCE_DESC & resDesc)
{
	resDesc.Width = sizeof(indices);
	(*ppDevice)->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&idxBuffer));
	idxBufferView.BufferLocation = idxBuffer->GetGPUVirtualAddress();
	idxBufferView.Format = DXGI_FORMAT_R16_UINT;
	idxBufferView.SizeInBytes = sizeof(indices);
	unsigned short* idxMap = nullptr;
	idxBuffer->Map(0, nullptr, (void**)&idxMap);
	for (int i = 0; i < indices.size(); i++)
	{
		idxMap[i] = indices[i];
	}
	idxBuffer->Unmap(0, nullptr);
}

void Dx12_Object::CreateCstBuffer(ID3D12Device ** ppDevice, const D3D12_HEAP_PROPERTIES & heapProp, D3D12_RESOURCE_DESC & resDesc)
{
	resDesc.Width = (sizeof(cbData) + 0xff)&~0xff;
	(*ppDevice)->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cstBuffer));
}

Dx12_Object::Dx12_Object(ID3D12Device* pDevice, const std::vector<Vertex>& argVertices, std::vector<unsigned short>& argIndices)
{
	vertices.resize(argVertices.size());
	indices.resize(argIndices.size());
	vertices = argVertices;
	indices = argIndices;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	CreateVtxBuffer(&pDevice, heapProp, resDesc);
	CreateIdxBuffer(&pDevice, heapProp, resDesc);
	CreateCstBuffer(&pDevice, heapProp, resDesc);
}

Dx12_Object::~Dx12_Object()
{
	SAFE_RELEASE(vtxBuffer);
	SAFE_RELEASE(idxBuffer);
	SAFE_RELEASE(cstBuffer);
}

void Dx12_Object::Mapping3D(const TransformMatrix& transform, const DirectX::XMMATRIX & view, const DirectX::XMMATRIX projection3D, DirectX::XMFLOAT4 color)
{
	cbData.matrix = DirectX::XMMatrixIdentity();
	cbData.color = color;
	cbData.matrix *= transform.scale;
	cbData.matrix *= transform.rotation;
	cbData.matrix *= transform.translation;
	ConstantBufferData* cbMap = nullptr;
	cstBuffer->Map(0, nullptr, (void**)&cbMap);
	cbMap->color = cbData.color;
	cbMap->matrix = cbData.matrix * view * projection3D;
	cstBuffer->Unmap(0, nullptr);
}

void Dx12_Object::Draw(ID3D12GraphicsCommandList ** ppCmdList)
{
	(*ppCmdList)->SetGraphicsRootConstantBufferView(0, cstBuffer->GetGPUVirtualAddress());
	(*ppCmdList)->IASetIndexBuffer(&idxBufferView);
	(*ppCmdList)->IASetVertexBuffers(0, 1, &vtxBufferView);
	(*ppCmdList)->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
}
