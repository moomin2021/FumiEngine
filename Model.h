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

// ������N���X
#include <string>

// ���_�f�[�^
struct Vertex3D {
	XMFLOAT3 pos;// ----> ���W
	XMFLOAT3 normal;// -> �@��
	XMFLOAT2 uv;// -----> UV���W
};

// �萔�o�b�t�@�\����(�}�e���A��)
struct MaterialBuff {
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
public:// �����o�ϐ�
	std::vector<Vertex3D> vertexes_;// ----> ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vbView_;// ---> ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_;// -> ���_�o�b�t�@

	std::vector<uint16_t> indexes_;// ----> �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW ibView_;// ---> �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;// -> �C���f�b�N�X�o�b�t�@

	Material material_;// -------------------> �}�e���A���f�[�^
	ComPtr<ID3D12Resource> materialBuff_;// -> �}�e���A���o�b�t�@

	// �e�N�X�`���n���h��
	int textureHandle_;

	static ID3D12GraphicsCommandList* cmdList_;// -> �R�}���h���X�g

public:// �����o�֐�
	// [Model]�C���X�^���X�쐬
	static Model* CreateModel(std::string fileName);

	// ����������
	static void Initialize(ID3D12GraphicsCommandList* cmdList);

	// �`�揈��
	void Draw();

private:// �����o�֐�
	// ���f���ǂݍ���
	void LoadModel(std::string name);

	// �}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

	// ���_�o�b�t�@���쐬
	void CreateVertexBuff();

	// �C���f�b�N�X�o�b�t�@���쐬
	void CreateIndexBuff();

	// �}�e���A���o�b�t�@�쐬
	void CreateMaterialBuff();
};