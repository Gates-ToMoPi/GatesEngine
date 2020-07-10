#pragma once
#include<DirectXMath.h>
struct ConstantBufferData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX matrix;
};

struct TransformMatrix
{
	DirectX::XMMATRIX scale;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
};
