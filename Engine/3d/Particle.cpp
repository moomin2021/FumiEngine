#include "Particle.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "Util.h"

#include <cassert>

// �ÓI�����o�ϐ��̎���
Camera* Particle::sCamera_ = nullptr;

Particle::Particle()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

#pragma region ���_�ݒ�
	vertices_.resize(4);
	vertices_[0] = { {-5.0f, -5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };
	vertices_[1] = { {-5.0f, +5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
	vertices_[2] = { {+5.0f, -5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };
	vertices_[3] = { {+5.0f, +5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} };
#pragma endregion

#pragma region �C���f�b�N�X�ݒ�
	indexes_.resize(6);
	indexes_[0] = 0;
	indexes_[1] = 1;
	indexes_[2] = 2;
	indexes_[3] = 2;
	indexes_[4] = 1;
	indexes_[5] = 3;
#pragma endregion

	// ���_�o�b�t�@�쐬
	CreateVertexBuff();

	// �C���f�b�N�X�o�b�t�@�쐬
	CreateIndexBuff();

#pragma region �萔�o�b�t�@����
	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
#pragma endregion
}

void Particle::Update()
{
#pragma region ���[���h�s��v�Z
	// �s�񏉊���
	Matrix4 matWorld = Matrix4Identity();

	// ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= Matrix4Scale(scale_);

	// ���[���h�s��ɉ�]�𔽉f
	matWorld *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
	matWorld *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
	matWorld *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));

	// ���[���h�s��ɕ��s�ړ��𔽉f
	matWorld *= Matrix4Translate(position_);
#pragma endregion

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	// �s��
	constMap_->mat = matWorld * sCamera_->GetMatView() * sCamera_->GetMatProjection();
#pragma endregion
}

void Particle::Draw()
{
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �C���X�^���X�擾
	Texture* tex = Texture::GetInstance();

	// SRV�q�[�v�̃n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&indexView_);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(static_cast<UINT>(indexes_.size()), 1, 0, 0, 0);
}

void Particle::CreateVertexBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();// GPU���z�A�h���X
	vertexView_.SizeInBytes = sizeVB;				// ���_�o�b�t�@�̃T�C�Y
	vertexView_.StrideInBytes = sizeof(vertices_[0]);	// ���_1���̃f�[�^�T�C�Y

	// Map�����Ń��C����������GPU�̃�������R�Â���
	Vertex* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (size_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}

	// �q���������
	vertexBuff_->Unmap(0, nullptr);
}

void Particle::CreateIndexBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexes_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// �C���f�b�N�X�o�b�t�@�r���[�쐬
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexView_.Format = DXGI_FORMAT_R16_UINT;
	indexView_.SizeInBytes = sizeIB;

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// �S�C���f�b�N�X�ɑ΂���
	for (size_t i = 0; i < indexes_.size(); i++)
	{
		indexMap[i] = indexes_[i];
	}

	// �}�b�s���O����
	indexBuff_->Unmap(0, nullptr);
}