#pragma once
#include "Camera.h"
#include "Matrix4.h"
#include "float4.h"
#include "float2.h"

#include <wrl.h>
#include <d3d12.h>
#include <vector>
#include <forward_list>

enum BILLBOARD {
	NONE,
	ALL,
	X,
	Y,
	Z
};

class ParticleManager
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���_�f�[�^
	struct Vertex {
		float3 pos;
		float scale;
	};

	// �萔�o�b�t�@�p
	struct ConstBufferData {
		Matrix4 mat;
	};

	// �p�[�e�B�N��1��
	struct Particle {
		float3 position = {};

		// ���x
		float3 velocity = {};

		// �����x
		float3 accel = {};

		// ���݃t���[��
		uint16_t frame = 0;

		// �I���t���[��
		uint16_t num_frame = 0;

		// �X�P�[��
		float scale = 1.0f;

		// �����l
		float startScale = 1.0f;

		// �ŏI�l
		float endScale = 0.0f;
	};

#pragma region �����o�ϐ�
private:
	// �p�[�e�B�N���z��
	std::forward_list<Particle> particles_;

	// �ő咸�_��
	const uint16_t vertexCount_ = 1024;

	// �I�u�W�F�N�g�f�[�^
	float3 position_ = { 0.0f, 0.0f, 0.0f };
	float3 rotation_ = { 0.0f, 0.0f, 0.0f };
	float3 scale_ = { 1.0f, 1.0f, 1.0f };
	float4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// ���_�f�[�^
	std::vector<Vertex> vertices_;			// ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> indexes_;		// �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	ConstBufferData* constMap_;

	bool isAlive_ = true;

	// �J����
	static Camera* sCamera_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	ParticleManager();

	// �f�X�g���N�^
	~ParticleManager();

	// �X�V����
	void Update(BILLBOARD billBoard = BILLBOARD::NONE);

	// �`�揈��
	void Draw(uint16_t handle = 0);

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life"> �������� </param>
	/// <param name="pos"> �������W </param>
	/// <param name="velocity"> ���x </param>
	/// <param name="accel"> �����x </param>
	void Add(uint16_t life, float3 pos, float3 velocity, float3 accel, float startScale, float endScale);

private:
	// ���_�o�b�t�@�쐬
	void CreateVertexBuff();

	// �C���f�b�N�X�o�b�t�@�쐬
	void CreateIndexBuff();
#pragma endregion

#pragma region �Z�b�^�[
public:
	// �J������ݒ�
	static void SetCamera(Camera* camera) { sCamera_ = camera; }

	void SetPos(const float3& pos) { position_ = pos; }
#pragma endregion

	bool GetIsAlive() { return isAlive_; }
};