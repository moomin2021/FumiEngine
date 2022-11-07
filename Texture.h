#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --DirectXTex-- //
#include <DirectXteX.h>

 // --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

class Texture {
	///  --�����o�ϐ�-- ///
public:

private:
	// --�f�o�C�X-- //
	ID3D12Device* device_;

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	// --�ǂݍ��މ摜�������ڂ�-- //
	UINT imageCount_;

	// --SRV�p�f�X�N���v�^�q�[�v-- //
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	/// --�����o�ϐ�END-- ///
	///---------------- ///
	/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static Texture* GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="device_"> �f�o�C�X </param>
	void Initialize(ID3D12Device* device);

	// --�e�N�X�`���̓ǂݍ���-- //
	int LoadTexture(const wchar_t* szFile);

	// --SRV�q�[�v�Q��-- //
	static ID3D12DescriptorHeap* GetSRVHeap();

private:
	// --�R���X�g���N�^-- //
	Texture();

	// --�f�X�g���N�^-- //
	~Texture();

	/// --�����o�֐�END-- ///
};