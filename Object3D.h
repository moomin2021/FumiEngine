#pragma once
// Direct3D 12 �p
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// ���w�֐�
#include <DirectXMath.h>
using namespace DirectX;

//�@ComPtr�p
#include <wrl.h>
using namespace Microsoft::WRL;

// �V�[�P���X�R���e�i�̈��
#include <vector>

// �J�����N���X
#include "Camera.h"

// ���f���N���X
#include "Model.h"

// �萔�o�b�t�@�\����(�I�u�W�F�N�g)
struct ObjectBuff {
	// �s��
	XMMATRIX mat;
};

class Object3D {
public:// �����o�ϐ�
	XMFLOAT3 position_;// -> ���W
	XMFLOAT3 rotation_;// -> ��]�p
	XMFLOAT3 scale_;// ----> �X�P�[��

private:// �����o�ϐ�
	// ���_�f�[�^�֘A
	D3D12_VERTEX_BUFFER_VIEW vbView_;// -----> ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_;// ---> ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^�֘A
	D3D12_INDEX_BUFFER_VIEW ibView_;// ---> �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;// -> �C���f�b�N�X�o�b�t�@

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> objectBuff_;// ---> �I�u�W�F�N�g
	ComPtr<ID3D12Resource> materialBuff_;// -> �}�e���A��

	// �������e�s��
	XMMATRIX matProjection_;

	// �J����
	Camera* camera_;

	// ���f��
	Model* model_;

public:// �����o�֐�
	// [Object3D]�C���X�^���X�쐬
	static Object3D* CreateObject3D();

	// �`�揈��
	void Draw();

	// �`��O����
	static void PreDraw();

#pragma region �Z�b�^�[
	// �J������ݒ�
	void SetCamera(Camera* camera) { camera_ = camera; }

	// ���f����ݒ�
	void SetModel(Model* model) { model_ = model; };
#pragma endregion

	Object3D();
private:// �����o�֐�
	// �R���X�g���N�^

	// ����������
	void Initialize();
};