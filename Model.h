#pragma once
// Direct3D 12 �p
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// �V�[�P���X�R���e�i�̈��
#include <vector>

// ������N���X
#include <string>

// ���b�V����̃f�[�^�\����
struct Mesh {

};

class Model {
private:// �����o�ϐ�
	// �e���b�V���̃f�[�^
	std::vector<Mesh> meshs_;

	static ID3D12GraphicsCommandList* cmdList_;// -> �R�}���h���X�g

public:// �����o�֐�
	/// <summary>
	/// ���f���쐬
	/// </summary>
	/// <param name="cmdList"></param>
	/// <returns></returns>
	static Model* CreateModel(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
};