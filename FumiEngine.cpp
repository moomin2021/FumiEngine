#include "FumiEngine.h"
#include "DX12Cmd.h"
#include "Texture.h"

int LoadTexture(const wchar_t* szFile) {

	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// --WIC�e�N�X�`���̃��[�h-- //
	result = LoadFromWICFile(
		szFile,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	// --�~�b�v�}�b�v����-- //
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	// --�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���-- //
	metadata.format = MakeSRGB(metadata.format);

	// --�q�[�v�ݒ�-- //
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// --���\�[�X�ݒ�-- //
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// --�e�N�X�`���o�b�t�@�̐���-- //
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --�S�~�b�v�}�b�v�ɂ���-- //
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,// --------------> �S�̈�փR�s�[
			img->pixels,// ----------> ���f�[�^�A�h���X
			(UINT)img->rowPitch,// --> 1���C���T�C�Y
			(UINT)img->slicePitch// -> 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// --�摜�J�E���^�C���N�������g-- //
	Texture::imageCount_++;

	// �ݒ��ۑ�
	Texture::texBuff_[Texture::imageCount_] = texBuff;

	// --�V�F�[�_���\�[�X�r���[�ݒ�-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// --CBV, SRV, UAV��1���̃T�C�Y���擾-- //
	UINT descriptorSize = DX12Cmd::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// --�n���h����1�i�߂�-- //
	Texture::srvHandle_.ptr += descriptorSize;

	// --�n���h���̎w���@�ɃV�F�[�_�[���\�[�X�r���[�쐬-- //
	DX12Cmd::GetDevice()->CreateShaderResourceView(Texture::texBuff_[Texture::imageCount_].Get(), &srvDesc, Texture::srvHandle_);

	// --�n���h����Ԃ�-- //
	return descriptorSize * Texture::imageCount_;
}