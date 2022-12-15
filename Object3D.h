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

// --���_�f�[�^-- //
struct Vertices3D {
	// --XYZ���W-- //
	XMFLOAT3 pos;

	// --�@���f�[�^-- //
	XMFLOAT3 normal;

	// --uv���W-- //
	XMFLOAT2 uv;
};

// --�萔�o�b�t�@�p�f�[�^�\���́i�s��ƐF�j-- //
struct ConstBufferData3D {
	// --�F�iRBGA�j-- //
	XMFLOAT4 color;

	// --�s��-- //
	XMMATRIX mat;
};

class Object3D {
	/// �����o�ϐ�
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

	// --�萔�o�b�t�@-- //
	ComPtr<ID3D12Resource> constBuff_;

	// --�������e�s��̌v�Z-- //
	XMMATRIX matProjection_;

	/// �����o�֐�
public:
	// --�R���X�g���N�^-- //
	Object3D();

	// --�����̂̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬-- //
	void CreateCube();

	// --���f���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬-- //
	void CreateModel();

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
	void CreateBuffer();

	// --�X�V����-- //
	void Update(Camera* camera);

	// --�`�揈��-- //
	void Draw(int textureHandle = 0);

	// --�`��O����-- //
	static void PreDraw();
};