#include "Texture.h"
#include "DX12Cmd.h"
#include "float4.h"
#include "Util.h"

#include <vector>

using namespace DirectX;

Texture* Texture::GetInstance() {
	// �C���X�^���X����
	static Texture instance;

	// �C���X�^���X��Ԃ�
	return &instance;
}

void Texture::Initialize() {
	// SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	CreateDescriptorHeap();

	// �f�o�b�N�p�̃e�N�X�`����ǂݍ���
	LoadTexture("Resources/debug.png");
}

int Texture::LoadTexture(const std::string fileName)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// ���ɓǂݍ��񂾕���������
	if (texBuff_.find(fileName) != texBuff_.end()) {
		return texHandle_[fileName];
	}

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		Util::StringToWideChar(fileName).data(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = static_cast<uint32_t>(metadata.height);
	textureResourceDesc.DepthOrArraySize = static_cast<uint32_t>(metadata.arraySize);
	textureResourceDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			static_cast<uint32_t>(i),
			nullptr,								// �S�̈�փR�s�[
			img->pixels,							// ���f�[�^�A�h���X
			static_cast<uint32_t>(img->rowPitch),	// 1���C���T�C�Y
			static_cast<uint32_t>(img->slicePitch)	// 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// CBV, SRV, UAV��1���̃T�C�Y���擾
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �ǂݍ��񂾉摜�̃n���h��
	uint16_t handle = descriptorSize * loadCounter_;

	// �ݒ��ۑ�
	texBuff_.emplace(fileName, texBuff);
	texHandle_.emplace(fileName, handle);

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// �n���h���̎w���@�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff_[fileName].Get(), &srvDesc, srvHandle_);

	// �n���h����1�i�߂�
	srvHandle_.ptr += descriptorSize;

	// �J�E���^�[��i�߂�
	loadCounter_++;

	// �n���h����Ԃ�
	return handle;
}

Texture::Texture() {}

void Texture::CreateDescriptorHeap()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2056;

	// �ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = DX12Cmd::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// SRV�q�[�v�̐擪�n���h�����擾
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();
}

int LoadTexture(const std::string fileName) {
	// �摜�ǂݍ���
	return Texture::GetInstance()->LoadTexture(fileName);
}
