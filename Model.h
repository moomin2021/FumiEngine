#pragma once
#include "float3.h"
#include "float2.h"

#include <string>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12.lib")

class Model {
	// ���_�f�[�^
	struct Vertex {
		float3 pos;		// ���W(XYZ)
		float3 normal;	// �@��(XYZ)
		float2 uv;		// UV���W(XY)
	};

	// �}�e���A���f�[�^
	struct Material {
		std::string name;	// �}�e���A����
		float3 ambient;		// �A���r�G���g�e���x
		float3 diffuse;		// �f�B�t���[�Y�e���x
		float3 specular;	// �X�y�L�����[�e���x
		float alpha;		// �A���t�@

		// �e�N�X�`���t�@�C����
		std::string textureFilename;

		// �R���X�g���N�^
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	// �萔�o�b�t�@�p�}�e���A���f�[�^
	struct MaterialBuffer {
		float3 ambient;	// �A���r�G���g�W��
		float pad1;		// �p�f�B���O
		float3 diffuse;	// �f�B�t�F�[�Y�W��
		float pad2;		// �p�f�B���O
		float3 specular;// �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};

#pragma region �����o�ϐ�
private:
	// ���_�f�[�^
	std::vector<Vertex> vertex_;			// ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> index_;		// �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@

	// �}�e���A���f�[�^
	Material material_;						// �}�e���A���f�[�^
	ComPtr<ID3D12Resource> materialBuff_;	// �}�e���A���o�b�t�@

	// �e�N�X�`���n���h��
	int textureHandle_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName"> ���f���t�@�C���� </param>
	Model(std::string fileName);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	/// <param name="name"> �t�@�C���� </param>
	void LoadModel(std::string name);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"> �t�@�C���� </param>
	/// <param name="fileName"> �t�@�C���� </param>
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// ���_�o�b�t�@�쐬
	/// </summary>
	void CreateVertexBuff();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�쐬
	/// </summary>
	void CreateIndexBuff();

	/// <summary>
	/// �}�e���A���o�b�t�@�쐬
	/// </summary>
	void CreateMaterialBuff();
#pragma endregion
};