#pragma once
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

class Sprite {
	// ���_�f�[�^
	struct Vertex {
		float2 pos;	// ���W(XYZ)
		float2 uv;	// UV���W(XY)
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Matrix4 mat;	// �s��
		float4 color;	// �F(RGBA)
	};

#pragma region �����o�ϐ�
private:
	// �X�v���C�g�f�[�^
	float2 position_;	// ���W
	float rotation_;	// ��]
	float2 scale_;		// �g�k
	float4 color_;		// �F(RGBA)

	// �X�v���C�g�f�[�^��ύX�������ǂ���
	bool hasChanget_;

	// �s��
	Matrix4 matWorld_;		// ���[���h�s��
	Matrix4 matProjection_;	// �ˉe�ϊ��s��

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;	// �萔�o�b�t�@
	ConstBufferData* constMap_;			// �}�b�s���O�����p

	// ���_�f�[�^
	std::vector<Vertex> vertex_;		// ���_�f�[�^
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@
	Vertex* vertexMap_;						// �}�b�s���O�����p
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�r���[

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> index_;	// �C���f�b�N�X�f�[�^
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(int textureHandle = 0);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

private:
	/// <summary>
	/// �X�v���C�g�f�[�^�̍X�V
	/// </summary>
	void UpdateData();

#pragma region �Z�b�^�[�֐�
public:

	/// <summary>
	/// ���W(XY)��ݒ�
	/// </summary>
	/// <param name="position"> ���W(XY) </param>
	inline void SetPosition(const float2& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// ��](Z)��ݒ�
	/// </summary>
	/// <param name="rotation"> ��](Z) </param>
	inline void SetRotation(float rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// �g�k(XY)��ݒ�
	/// </summary>
	/// <param name="scale"> �g�k(XY) </param>
	inline void SetScale(const float2& scale) { scale_ = scale, hasChanget_ = true; }

	/// <summary>
	/// �F(RGBA)��ݒ�
	/// </summary>
	/// <param name="color"> �F(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }
#pragma endregion

#pragma endregion
};