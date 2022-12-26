#pragma once

// ���w�֐�
#include <DirectXMath.h>
using namespace DirectX;

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
private:// �����o�ϐ�
	std::vector<Vertex3D> vertexes_;// -> ���_�f�[�^
	std::vector<uint16_t> indexes_;// --> �C���f�b�N�X�f�[�^

	// �}�e���A��
	Material material_;

	// �e�N�X�`���n���h��
	int textureHandle_;

public:// �����o�֐�
	// [Model]�C���X�^���X�쐬
	Model* CreateModel(std::string fileName);

#pragma region �Q�b�^�[
	// ���_�f�[�^���擾
	std::vector<Vertex3D> GetVertexes() { return vertexes_; }

	// �C���f�b�N�X�f�[�^���擾
	std::vector<uint16_t> GetIndexes() { return indexes_; }

	// �}�e���A���擾
	Material GetMaterial() { return material_; }

	// �e�N�X�`���n���h�����擾
	int GetTextureHandle() { return textureHandle_; }
#pragma endregion

private:// �����o�֐�
	// ���f���ǂݍ���
	void LoadModel(std::string name);

	// �}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);
};