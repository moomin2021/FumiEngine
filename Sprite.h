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

// --���_�f�[�^-- //
struct Vertices2D {
	// --XYZ���W-- //
	XMFLOAT3 pos;

	// --uv���W-- //
	XMFLOAT2 uv;
};

// --�萔�o�b�t�@�p�f�[�^�\���́i�s��ƐF�j-- //
struct ConstBufferData {
	// --�F�iRBGA�j-- //
	XMFLOAT4 color;

	// --�s��-- //
	XMMATRIX mat;
};

class Sprite {
	/// --�����o�ϐ�-- ///
public:
	// --Z�����̉�]�p-- //
	float rotation;

	// --���W-- //
	XMFLOAT2 position;

	// --���[���h�s��-- //
	XMMATRIX matWorld;

	// --�ˉe�ϊ��s��-- //
	XMMATRIX matProjection;

	// --�F�iRBGA�j-- //
	XMFLOAT4 color;

	// --�X�P�[��-- //
	XMFLOAT2 scale;

private:
	// --���_�f�[�^-- //
	Vertices2D vertices[4];

	// --�C���f�b�N�X�f�[�^-- //
	uint16_t indices[6];

	// --���_�o�b�t�@�r���[-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --���_�o�b�t�@-- //
	ComPtr<ID3D12Resource> vertBuff;

	// --���_�o�b�t�@�}�b�s���O�����p-- //
	Vertices2D* vertMap;

	// --�C���f�b�N�X�o�b�t�@-- //
	ComPtr<ID3D12Resource> indexBuff;

	// --�C���f�b�N�X�o�b�t�@�r���[-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

	// --�萔�o�b�t�@-- //
	ComPtr<ID3D12Resource> constBuff;

	// --�萔�o�b�t�@�}�b�s���O�����p-- //
	ConstBufferData* constMap;

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Sprite();

	// --�X�V����-- //
	void Update();

	// --�`�揈��-- //
	void Draw(int textureHandle = 0);

	// --�`��O����-- //
	static void PreDraw();

private:

	/// --�����o�֐�END-- ///
};