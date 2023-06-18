#pragma once
#include <wrl.h>
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

// �p�C�v���C���Z�b�g
struct PipelineSet {
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState;

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
};

// �X�v���C�g�p�p�C�v���C������
PipelineSet CreateSpritePipeline();

// �I�u�W�F�N�g3D�̃p�C�v���C������
PipelineSet CreateObject3DPipeline();