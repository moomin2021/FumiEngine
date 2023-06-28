#pragma once
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "Matrix4.h"
#include "Key.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

//class PostEffect {
//	// ���_�f�[�^
//	struct Vertex {
//		float2 pos;	// ���W(XYZ)
//		float2 uv;	// UV���W(XY)
//	};
//
//	// �萔�o�b�t�@�p�f�[�^�\����
//	struct ConstBufferData {
//		Matrix4 mat;	// �s��
//		float4 color;	// �F(RGBA)
//	};
//
//#pragma region �����o�ϐ�
//private:
//	// �L�[�{�[�h�C���X�^���X
//	Key* key_;
//
//	// �X�v���C�g�f�[�^
//	float2 position_;	// ���W(XY)
//	float rotation_;	// ��](Z)
//	float2 size_;		// �T�C�Y(XY)
//	float4 color_;		// �F(RGBA)
//	float2 anchorPoint_;// �A���J�[�|�C���g���W(XY)
//	bool isFlipX_;		// ���E���]
//	bool isFlipY_;		// �㉺���]
//
//	// �X�v���C�g�f�[�^��ύX�������ǂ���
//	bool hasChanget_;
//
//	// �s��
//	Matrix4 matWorld_;		// ���[���h�s��
//	Matrix4 matProjection_;	// �ˉe�ϊ��s��
//
//	// �萔�o�b�t�@
//	ComPtr<ID3D12Resource> constBuff_;	// �萔�o�b�t�@
//	ConstBufferData* constMap_;			// �}�b�s���O�����p
//
//	// ���_�f�[�^
//	std::vector<Vertex> vertex_;		// ���_�f�[�^
//	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@
//	Vertex* vertexMap_;						// �}�b�s���O�����p
//	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�r���[
//
//	// �C���f�b�N�X�f�[�^
//	std::vector<uint16_t> index_;	// �C���f�b�N�X�f�[�^
//	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@
//	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
//
//	// �e�N�X�`���o�b�t�@
//	std::vector<ComPtr<ID3D12Resource>> texBuff_;
//
//	// SRV�p�f�X�N���v�^�q�[�v
//	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
//
//	// �[�x�o�b�t�@
//	ComPtr<ID3D12Resource> depthBuff_;
//
//	// RTV�p�f�X�N���v�^�q�[�v
//	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
//
//	// DSV�p�f�X�N���v�^�q�[�v
//	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
//
//	// ��ʃN���A�J���[
//	static const std::vector<float> clearColor_;
//#pragma endregion
//
//#pragma region �����o�֐�
//public:
//	/// <summary>
//	/// �R���X�g���N�^
//	/// </summary>
//	PostEffect();
//
//	/// <summary>
//	/// �`�揈��
//	/// </summary>
//	void Draw();
//
//	/// <summary>
//	/// �`��O����
//	/// </summary>
//	void PreDraw();
//
//	/// <summary>
//	/// �`��㏈��
//	/// </summary>
//	void PostDraw();
//
//private:
//	/// <summary>
//	/// �X�v���C�g�f�[�^�̍X�V
//	/// </summary>
//	void UpdateData();
//
//	/// <summary>
//	/// ���_�o�b�t�@�쐬
//	/// </summary>
//	void CreateVertexBuff();
//
//	/// <summary>
//	/// �C���f�b�N�X�o�b�t�@�쐬
//	/// </summary>
//	void CreateIndexBuff();
//
//	/// <summary>
//	/// �萔�o�b�t�@�쐬
//	/// </summary>
//	void CreateConstBuff();
//#pragma endregion
//};

class PostEffect {
#pragma region �\����
private:
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
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �y���|���S���f�[�^
	float2 scale_;// �g�k(��ʂ̑傫���ɑ΂���)

	// �y���|���S���f�[�^��ύX�������ǂ���
	bool hasChanget_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;	// �萔�o�b�t�@
	ConstBufferData* constMap_;			// �}�b�s���O�����p

	// ���_�f�[�^
	std::vector<Vertex> vertex_;			// ���_�f�[�^
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@
	Vertex* vertexMap_;						// �}�b�s���O�����p
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�r���[

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> index_;		// �C���f�b�N�X�f�[�^
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[

	// �e�N�X�`���o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> texBuff_;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff_;

	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;

	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	// ��ʃN���A�J���[
	static const std::vector<float> clearColor_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();

private:
	/// <summary>
	/// �y���|���S���f�[�^�̍X�V
	/// </summary>
	void UpdateData();

	/// <summary>
	/// ���_�o�b�t�@�쐬
	/// </summary>
	void CreateVertexBuff();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�쐬
	/// </summary>
	void CreateIndexBuff();

	/// <summary>
	/// �萔�o�b�t�@�쐬
	/// </summary>
	void CreateConstBuff();

	/// <summary>
	/// �e�N�X�`���o�b�t�@�쐬
	/// </summary>
	void CreateTextureBuff();

	/// <summary>
	/// SRV�p�f�X�N���v�^�q�[�v�쐬
	/// </summary>
	void CreateSRVDescHeap();

	/// <summary>
	/// RTV�p�f�X�N���v�^�q�[�v�쐬
	/// </summary>
	void CreateRTVDescHeap();

	/// <summary>
	/// �[�x�o�b�t�@�쐬
	/// </summary>
	void CreateDepthBuff();

	/// <summary>
	/// DSV�p�f�X�N���v�^�q�[�v�쐬
	/// </summary>
	void CreateDSVDescHeap();
#pragma endregion
};