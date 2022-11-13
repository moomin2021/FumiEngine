#include "Texture.h"

#include "DX12Cmd.h"

// --SRV�q�[�v�̐擪�n���h�����擾-- //
D3D12_CPU_DESCRIPTOR_HANDLE Texture::srvHandle_ = {};

// --�ǂݍ��މ摜�������ڂ�-- //
UINT Texture::imageCount_ = 0;

// --SRV�p�f�X�N���v�^�q�[�v-- //
ComPtr<ID3D12DescriptorHeap> Texture::srvHeap_ = nullptr;

// �e�N�X�`���o�b�t�@
std::array<ComPtr<ID3D12Resource>, 2056> Texture::texBuff_ = {};

// --�C���X�^���X�ǂݍ���-- //
Texture* Texture::GetInstance() {
	// --�C���X�^���X����-- //
	static Texture instance;

	// --�C���X�^���X��Ԃ�-- //
	return &instance;
}

// --�R���X�g���N�^-- //
Texture::Texture() 
#pragma region ���������X�g
	//device_(nullptr)
#pragma endregion
{

}

// --�f�X�g���N�^-- //
Texture::~Texture() {}

// --����������-- //
void Texture::Initialize(ID3D12Device* device) {
	// --DirectX�N���X�̃f�o�C�X�擾-- //
	//this->device_ = device;

	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	/// --�����̐^�����ȃe�N�X�`�����쐬-- ///
#pragma region

	// --����摜�f�[�^-- //
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;
	const size_t imageDataCount = textureWidth * textureHeight;
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];

	for (size_t i = 0; i < imageDataCount; i++) {
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;
	}

	// --�q�[�v�ݒ�-- //
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;
	textureResourceDesc.Height = textureHeight;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// --�e�N�X�`���o�b�t�@�̐���-- //
	ComPtr<ID3D12Resource> texBuff;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --�e�N�X�`���o�b�t�@�Ƀf�[�^�]��-- //
	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		imageData,
		sizeof(XMFLOAT4) * textureWidth,
		sizeof(XMFLOAT4) * imageDataCount
	);
	assert(SUCCEEDED(result));

	// �ݒ��ۑ�
	texBuff_[imageCount_] = texBuff;

#pragma endregion
	/// --END-- ///

	// --�f�X�N���v�^�q�[�v�̐ݒ�-- //
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2056;

	// --�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�-- //
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	// --�V�F�[�_���\�[�X�r���[�ݒ�-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// --�n���h���̎w���@�ɃV�F�[�_�[���\�[�X�r���[�쐬-- //
	device->CreateShaderResourceView(texBuff_[imageCount_].Get(), &srvDesc, srvHandle_);
}

// --SRV�q�[�v�Q��-- //
ID3D12DescriptorHeap* Texture::GetSRVHeap() { return srvHeap_.Get(); }