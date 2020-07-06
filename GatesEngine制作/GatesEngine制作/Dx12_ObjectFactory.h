#pragma once
#include<d3d12.h>
//いろいろな図形の頂点やらインデックスやら行列やらを生成して
//引数で渡したオブジェクトに参照して書き換える
class Dx12_ObjectFactory
{
private:
	ID3D12Device** pDevice;
public:
	Dx12_ObjectFactory(ID3D12Device**,ID3D12GraphicsCommandList**);
	~Dx12_ObjectFactory();
	//2Dor3D
	bool CreateTriangle();
	bool CreateBox();
	bool CreateCircle();
	//3D
	bool CreateCube();
	bool CreateSphere();
};

