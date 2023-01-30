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

#include "fMath.h"

// �V�[�P���X�R���e�i�̈��
#include <vector>

// �J�����N���X
#include "Camera.h"

// ���f���N���X
#include "Model.h"

// �萔�o�b�t�@�\����(�I�u�W�F�N�g)
struct ObjectBuff {
	// �s��
	Matrix4 mat;
};

class Object3D {
private:// �����o�ϐ�
	Float3 position_;// -> ���W
	Float3 rotation_;// -> ��]�p
	Float3 scale_;// ----> �X�P�[��

	// �萔�o�b�t�@
	static D3D12_HEAP_PROPERTIES heapProp_;
	static D3D12_RESOURCE_DESC resdesc_;
	ComPtr<ID3D12Resource> constBuff_;

	// ���[���h���W
	Matrix4 matWorld_;

	// �J����
	static Camera* camera_;

	// ���f��
	Model* model_;

	static ID3D12Device* device_;// -> �f�o�C�X
	static ID3D12GraphicsCommandList* cmdList_;// -> �R�}���h���X�g

	// �_�[�e�B�t���O
	bool dirty;

public:// �����o�֐�
	// [Object3D]�C���X�^���X�쐬
	static Object3D* CreateObject3D();

	// �I�u�W�F�N�g3D�S�̂̏�����
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

	// �`�揈��
	void Draw();

	// �`��O����
	static void PreDraw();

	// �J�����Z�b�g
	static inline void SetCamera(Camera* camera) { camera_ = camera; }

#pragma region �Z�b�^�[
	// ���f����ݒ�
	void SetModel(Model* model) { model_ = model; };

	// �e�v�f�ύX
	void SetPos(const Float3& position);
	void SetRot(const Float3& rotation);
	void SetScale(const Float3& scale);
#pragma endregion

#pragma region �Q�b�^�[
	// �e�v�f�擾
	inline const Float3& GetPos() { return position_; }
	inline const Float3& GetRot() { return rotation_; }
	inline const Float3& GetScale() { return scale_; }
#pragma endregion

private:// �����o�֐�
	// �R���X�g���N�^
	Object3D();

	// �萔�o�b�t�@����
	void GenerateConstBuffer();

	// �萔�o�b�t�@�X�V
	void TransferConstBuffer();
};