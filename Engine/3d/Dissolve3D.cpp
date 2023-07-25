#include "Dissolve3D.h"
#include "DX12Cmd.h"
#include "PipelineManager.h"

#include <cassert>

void Dissolve3D::Initialize()
{
	// ���f������
	model_ = std::make_unique<Model>("dissolve");

	// �I�u�W�F�N�g����
	object_ = std::make_unique<Object3D>(model_.get());

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

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

void Dissolve3D::Update()
{
	// �I�u�W�F�N�g�X�V����
	object_->Update();

	// �萔�o�b�t�@��]��
	constMap_->dissolveTime = time_;
}

void Dissolve3D::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Dissolve");

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(4, constBuff_->GetGPUVirtualAddress());

	// �I�u�W�F�N�g�`�揈��
	object_->Draw();
}