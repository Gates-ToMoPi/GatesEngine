#include"Dx12_Pipeline.h"
#include <d3dcompiler.h>
#include<string>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool Dx12_Pipeline::Create_RootSignature()
{
	//�T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE srvRange = {};
	srvRange.NumDescriptors = 1;
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.BaseShaderRegister = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam[2] = {};
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &srvRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//���[�g�V�O�l�`���̐ݒ�Ɛ���
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = 2;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	ID3DBlob* rootBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob)))
	{
		return false;
	}
	if (FAILED((*pDevice)->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature))))
	{
		return false;
	}
	return true;
}

void Dx12_Pipeline::SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* blendDesc, BlendMode mode)
{
	(*blendDesc).RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	(*blendDesc).BlendEnable = true;
	(*blendDesc).BlendOpAlpha = D3D12_BLEND_OP_ADD;
	(*blendDesc).SrcBlendAlpha = D3D12_BLEND_ONE;
	(*blendDesc).DestBlendAlpha = D3D12_BLEND_ZERO;
	switch (mode)
	{
	case BLENDMODE_ADD:
		/* ���Z���� */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_SUB:
		/* ���Z���� */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_INV:
		/* �F���] */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		(*blendDesc).DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLENDMODE_ALPHA:
		/* ������ */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_SRC_ALPHA;
		(*blendDesc).DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	}
}
bool Dx12_Pipeline::Create_PipelineState()
{
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	SetBlendMode(&blendDesc, BlendMode::BLENDMODE_ALPHA);
	/* �V�F�[�_�[�ɓn����� */
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	/* PipelineStateDesc */
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.VS.pShaderBytecode = m_vsBlob->GetBufferPointer();
	psoDesc.VS.BytecodeLength = m_vsBlob->GetBufferSize();
	psoDesc.PS.pShaderBytecode = m_psBlob->GetBufferPointer();
	psoDesc.PS.BytecodeLength = m_psBlob->GetBufferSize();
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState.RenderTarget[0] = blendDesc;
	psoDesc.InputLayout.pInputElementDescs = inputLayout;
	psoDesc.InputLayout.NumElements = _countof(inputLayout);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.pRootSignature = m_rootSignature;
	//�h��Ԃ��p
	if (FAILED((*pDevice)->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso_solid))))
	{
		return false;
	}
	//���C���[�t���[���p
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	if (FAILED((*pDevice)->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso_wire))))
	{
		return false;
	}
	return true;
}

Dx12_Pipeline::Dx12_Pipeline(ID3D12Device ** device, ID3D12GraphicsCommandList ** cmdList)
{
	//�f�o�C�X�ƃR�}���h���X�g�̎Q��
	pDevice = device;
	pCmdList = cmdList;
}

Dx12_Pipeline::~Dx12_Pipeline()
{
	//�Z�[�t�����[�X�A�����[�X�֐����Ă񂾌�nullptr����
	SAFE_RELEASE(m_vsBlob);
	SAFE_RELEASE(m_psBlob);
	SAFE_RELEASE(m_rootBlob);
	SAFE_RELEASE(m_rootSignature);
	SAFE_RELEASE(m_pso_solid);
	SAFE_RELEASE(m_pso_wire);
	pDevice = nullptr;
	pCmdList = nullptr;
}

bool Dx12_Pipeline::LoadShaderFile(LPCWSTR filename, LPCSTR entrypoint, LPCSTR version, ShaderType type)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	if (type == SHADER_TYPE_VS)
	{
		result = D3DCompileFromFile(filename, nullptr, include, entrypoint, version, flag, 0, &m_vsBlob, &errorBlob);
	}
	else if (type == SHADER_TYPE_PS)
	{
		result = D3DCompileFromFile(filename, nullptr, include, entrypoint, version, flag, 0, &m_psBlob, &errorBlob);
	}

	if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
	{
		OutputDebugStringA("�V�F�[�_�[�t�@�C������������܂���\n");
		SAFE_RELEASE(errorBlob);
		return false;
	}
	else if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		OutputDebugStringA(errstr.c_str());
		SAFE_RELEASE(errorBlob);
		return false;
	}
	return true;
}

bool Dx12_Pipeline::Create_Pipeline()
{
	if (!Create_RootSignature())return false;
	if (!Create_PipelineState())return false;
	return true;
}

void Dx12_Pipeline::Set(D3D12_FILL_MODE fillMode)
{
	if (fillMode == D3D12_FILL_MODE_SOLID)
	{
		(*pCmdList)->SetPipelineState(m_pso_solid);
	}
	else
	{
		(*pCmdList)->SetPipelineState(m_pso_wire);
	}
	//(*pCmdList)->SetPipelineState(m_pso_wire);
	(*pCmdList)->SetGraphicsRootSignature(m_rootSignature);
}
