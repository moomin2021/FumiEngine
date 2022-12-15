#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --�V�[�P���X�R���e�i�̈��-- //
#include <vector>

// --�J�����N���X-- //
#include "Camera.h"

#include "Object3D.h"

#include <string>

// --�萔�o�b�t�@�p�f�[�^�\���́i�s��ƐF�j-- //
struct ModelBufferData {
	// --�s��-- //
	XMMATRIX mat;
};

// �}�e���A���o�b�t�@�p�f�[�^�\����
struct MaterialBufferData {
	XMFLOAT3 ambient;// -> �A���r�G���g�W��
	float pad1;// -> �p�f�B���O
	XMFLOAT3 diffuse;// -> �f�B�t�F�[�Y�W��
	float pad2;// -> �p�f�B���O
	XMFLOAT3 specular;// -> �X�y�L�����[�W��
	float alpha;// -> �A���t�@
};

// �}�e���A���\����
struct Material {
	std::string name;// -> �}�e���A����
	XMFLOAT3 ambient;// -> �A���r�G���g�e���x
	XMFLOAT3 diffuse;// -> �f�B�t���[�Y�e���x
	XMFLOAT3 specular;// -> �X�y�L�����[�e���x
	float alpha;// -> �A���t�@
	std::string textureFilename;// -> �e�N�X�`���t�@�C����

	// �R���X�g���N�^
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}
};

class Model {
	// --�����o�ϐ�-- //
public:
	// ���W�A��]�p�A�X�P�[��
	XMFLOAT3 position_;
	XMFLOAT3 rotation_;
	XMFLOAT3 scale_;

	// �F
	XMFLOAT4 color_;

private:
	// --���_�f�[�^-- //
	std::vector<Vertices3D> vertices_;// -> ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView_;// -> ���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> vertBuff_;// -> ���_�o�b�t�@

	// --�C���f�b�N�X�f�[�^-- //
	std::vector<uint16_t> indices_;// -> �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW ibView_;// -> �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;// -> �C���f�b�N�X�o�b�t�@

	// --���f���萔�o�b�t�@-- //
	ComPtr<ID3D12Resource> constBuff_;

	// �}�e���A���萔�o�b�t�@
	ComPtr<ID3D12Resource> materialBuff_;

	// --�������e�s��̌v�Z-- //
	XMMATRIX matProjection_;

	// �}�e���A��
	Material material_;

	// �e�N�X�`���n���h��
	int textureHandle_;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Model();

	// ���f���ǂݍ���
	void CreateModel(std::string name);

	// �X�V����
	void Update(Camera * camera);

	// �`�揈��
	void Draw();

	// �`��O����
	static void PreDraw();

private:
	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
	void CreateBuffer();

	// �}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);
};