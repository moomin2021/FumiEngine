#pragma once


#include <vector>
#include <d3d12.h>
#include <wrl.h>

class Model {
#pragma region �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

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
	uint16_t textureHandle_;
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