#pragma once
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <wrl.h>
using namespace Microsoft::WRL;
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "fMath.h"
#include "LightGroup.h"

class Object3D {
public:// -----�T�u�N���X----- //
	// �萔�o�b�t�@�\����(�I�u�W�F�N�g)
	struct ObjectBuff {
		Matrix4 viewProj;// -> �r���[�v���W�F�N�V����
		Matrix4 world;// ----> ���[���h�s��
		Float3 cameraPos;// -> �J�������W(���[���h���W)
		float pad1;// -------> �p�f�B���O
		Float4 color;// -----> �F(RGBA)
	};

private:// -----�����o�ϐ�----- //
	Float3 position_;// ------------------> ���W
	Float3 rotation_;// ------------------> ��]�p
	Float3 scale_;// ---------------------> �X�P�[��
	Float4 color_;// ---------------------> �F(RGBA)
	Matrix4 matWorld_;// -----------------> ���[���h���W
	ComPtr<ID3D12Resource> constBuff_;// -> �萔�o�b�t�@
	Model* model_;// ---------------------> ���f��
	bool dirty;// ------------------------> �_�[�e�B�t���O

private:// -----�ÓI�����o�ϐ�----- //
	static ID3D12Device* device_;// ---------------> �f�o�C�X
	static ID3D12GraphicsCommandList* cmdList_;// -> �R�}���h���X�g
	static D3D12_HEAP_PROPERTIES heapProp_;// -----> �q�[�v�ݒ�
	static D3D12_RESOURCE_DESC resdesc_;// --------> ���\�[�X�ݒ�
	static Camera* camera_;// ---------------------> �J����
	static LightGroup* lightGroup_;// -------------> ���C�g

private:// -----�����o�֐�----- //
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3D();

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void GenerateConstBuffer();

	/// <summary>
	/// �萔�o�b�t�@�X�V
	/// </summary>
	void TransferConstBuffer();

public:// -----�ÓI�����o�֐�----- //
	/// <summary>
	/// [Object3D]�C���X�^���X�쐬
	/// </summary>
	static Object3D* CreateObject3D(Model* model = nullptr);

	/// <summary>
	/// �I�u�W�F�N�g3D�S�̂̏�����
	/// </summary>
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �J�����ݒ�
	/// </summary>
	static void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// ���C�g�ݒ�
	/// </summary>
	static void SetLightGroup(LightGroup* lightGroup) { lightGroup_ = lightGroup; }

public:// -----�����o�֐�----- //
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	void SetPos(const Float3& position);

	/// <summary>
	/// ��]�p�ݒ�[�x���@]
	/// </summary>
	void SetRot(const Float3& rotation);

	/// <summary>
	/// �g�k�ݒ�
	/// </summary>
	void SetScale(const Float3& scale);

	/// <summary>
	/// �F(RGBA)�ݒ�
	/// </summary>
	void SetColor(const Float4& color);

	/// <summary>
	/// ���f���ݒ�
	/// </summary>
	void SetModel(Model* model) { model_ = model; };

	/// <summary>
	/// ���W�擾
	/// </summary>
	inline const Float3& GetPos() { return position_; }

	/// <summary>
	/// ��]�p�擾[�x���@]
	/// </summary>
	inline const Float3& GetRot() { return rotation_; }

	/// <summary>
	/// �g�k�擾
	/// </summary>
	inline const Float3& GetScale() { return scale_; }
};