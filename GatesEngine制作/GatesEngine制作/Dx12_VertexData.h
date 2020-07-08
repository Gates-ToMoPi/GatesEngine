#pragma once
#include<DirectXMath.h>
struct Vertex
{
	DirectX::XMFLOAT3 pos;
};

struct Vertex_UV
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct Vertex_UV_Normal
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};
