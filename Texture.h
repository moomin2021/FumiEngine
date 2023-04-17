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

#include <array>
#include <map>
#include <string>

class Texture {
	///  --�����o�ϐ�-- ///
public:
	// --�ǂݍ��މ摜�������ڂ�-- //
	static UINT imageCount_;

	// �e�N�X�`���o�b�t�@
	static std::map<const std::string, ComPtr<ID3D12Resource>> texBuff_;

	// �e�N�X�`���n���h��
	static std::map<const std::string, UINT> texHandle_;

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	static D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

private:
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

	// --SRV�q�[�v�Q��-- //
	static ID3D12DescriptorHeap* GetSRVHeap();

private:
	// --�R���X�g���N�^-- //
	Texture();

	// --�f�X�g���N�^-- //
	~Texture();

	/// --�����o�֐�END-- ///
};

int LoadTexture(const std::string fileName);