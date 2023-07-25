#pragma once
#include "Object3D.h"

#include <memory>

class Dissolve3D {
#pragma region �\����
private:
	struct ConstBufferData {
		float dissolveTime;
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource>	constBuff_ = nullptr;	// �萔�o�b�t�@
	ConstBufferData* constMap_;	// �}�b�s���O�����p

	// �f�B�]���u�W��
	float time_ = 0.0f;
#pragma endregion

#pragma region �����o�֐�
public:
	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �f�B�]���u�W����ݒ�
	void SetDissolveTime(float t) { time_ = t; }
#pragma endregion
};