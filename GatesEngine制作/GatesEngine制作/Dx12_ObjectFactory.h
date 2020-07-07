#pragma once
#include<d3d12.h>
#include<vector>
//���낢��Ȑ}�`�̒��_���C���f�b�N�X���s����𐶐�����
//�����œn�����I�u�W�F�N�g�ɎQ�Ƃ��ď���������
class Dx12_ObjectFactory
{
private:
	ID3D12Device** pDevice;
public:
	Dx12_ObjectFactory(ID3D12Device**);
	~Dx12_ObjectFactory();
	//2Dor3D
	bool CreateBox();
	bool CreateCircle();
	//3D
	bool CreateCube();
	bool CreateSphere();
};

