#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
class Dx12_Object
{
	struct ConstantBufferData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX local;
	};
private:
	ID3D12Resource* vb;
	ID3D12Resource* ib;
	ID3D12Resource* cb;
	DirectX::XMMATRIX world;
public:
	Dx12_Object();
	~Dx12_Object();
	void Draw();
	void Map(const DirectX::XMMATRIX*,const DirectX::XMMATRIX*);
};

