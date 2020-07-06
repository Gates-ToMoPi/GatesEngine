#pragma once
#include<d3d12.h>
#include"SafeRelease.h"
enum BlendMode
{
	BLENDMODE_ADD,
	BLENDMODE_SUB,
	BLENDMODE_INV,
	BLENDMODE_ALPHA
};
enum ShaderType
{
	SHADER_TYPE_VS,
	SHADER_TYPE_PS
};
class Dx12_Pipeline
{
private:
	ID3DBlob* m_vsBlob;
	ID3DBlob* m_psBlob;
	ID3DBlob* m_rootBlob;
	ID3D12DescriptorHeap* m_cbv_srvHeap;
	ID3D12RootSignature* m_rootSignature;
	ID3D12PipelineState* m_pso_solid;
	ID3D12PipelineState* m_pso_wire;
	ID3D12Device** pDevice;
	ID3D12GraphicsCommandList** pCmdList;
private:
	bool Create_DescriptorHeap();
	bool Create_RootSignature();
	void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* blendDesc, BlendMode mode);
	bool Create_PipelineState();
public:
	Dx12_Pipeline(ID3D12Device ** device, ID3D12GraphicsCommandList ** cmdList);
	~Dx12_Pipeline();
	bool LoadShaderFile(LPCWSTR filename, LPCSTR entrypoint, LPCSTR version, ShaderType type);
	bool Create_Pipeline();
	void Set(D3D12_FILL_MODE fillMode);
};
