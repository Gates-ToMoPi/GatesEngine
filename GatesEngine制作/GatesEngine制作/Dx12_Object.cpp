#include "Dx12_Object.h"

Dx12_Object::~Dx12_Object()
{
	SAFE_RELEASE(vtxBuffer);
	SAFE_RELEASE(idxBuffer);
	SAFE_RELEASE(cstBuffer);
}

void Dx12_Object::Draw(ID3D12GraphicsCommandList ** ppCmdList)
{
	(*ppCmdList)->SetGraphicsRootConstantBufferView(0, cstBuffer->GetGPUVirtualAddress());
	(*ppCmdList)->IASetIndexBuffer(&idxBufferView);
	(*ppCmdList)->IASetVertexBuffers(0, 1, &vtxBufferView);
}
