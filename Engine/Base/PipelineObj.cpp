#include "PipelineObj.h"
#include "DX12Cmd.h"
#include "Util.h"

#include <d3dcompiler.h>
#include <d3d12.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

PipelineObj::PipelineObj()
{
}

void PipelineObj::LoadShader(std::string fileName, ShaderType shaderType)
{
	switch (shaderType)
	{
	// ���_�V�F�[�_�[
	case VS : ShaderCompileFromFile(fileName, "vs_5_0", &vsBlob_);
		break;
	// �s�N�Z���V�F�[�_�[
	case PS : ShaderCompileFromFile(fileName, "ps_5_0", &psBlob_);
		break;
	}
}

void PipelineObj::AddInputLayout(const char* semanticName, DXGI_FORMAT format, uint16_t index)
{
	// �C���v�b�g���C�A�E�g��ݒ聕�ǉ�
	inputLayout_.emplace_back(D3D12_INPUT_ELEMENT_DESC{
		semanticName,								// �Z�}���e�B�b�N��
		index,										// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
		format,										// �v�f���ƃr�b�g����\��
		0,											// ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
		D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	// ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
		0											// ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
	});
}

void PipelineObj::CreateRootParams(uint16_t texRegisterNum, uint16_t constBuffNum)
{
	// ���[�g�p�����[�^�̐����J�E���^�[
	uint16_t counter = 0;

	rootParams_.resize(texRegisterNum + constBuffNum);
	descRangeSRV_.resize(texRegisterNum);

	for (size_t i = 0; i < texRegisterNum; i++) {
		descRangeSRV_[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, static_cast<UINT>(i));// t i ���W�X�^

		// ���[�g�p�����[�^�ݒ�
		rootParams_[counter].InitAsDescriptorTable(1, &descRangeSRV_[i], D3D12_SHADER_VISIBILITY_ALL);

		// ���[�g�p�����[�^�𐶐����ݒ肵�I��������߃C���N�������g
		counter++;
	}

	for (size_t i = 0; i < constBuffNum; i++) {
		// ���[�g�p�����[�^�ݒ�
		rootParams_[counter].InitAsConstantBufferView(static_cast<UINT>(i), 0, D3D12_SHADER_VISIBILITY_ALL);

		// ���[�g�p�����[�^�𐶐����ݒ肵�I��������߃C���N�������g
		counter++;
	}
}

void PipelineObj::CreatePipeline(uint16_t renderTargetNum) {
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob_->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// �[�x�N���b�s���O��L����

	// �f�v�X�X�e���V���X�e�[�g
	pipelineDesc.DepthStencilState.DepthEnable = true;							// �[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;								// �[�x�l�t�H�[�}�b�g

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RGBA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	for (size_t i = 0; i < renderTargetNum; i++) {
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout_.data();
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout_.size());

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = renderTargetNum;					// �`��Ώۂ�2��
	for (size_t i = 0; i < renderTargetNum; i++) {
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0~255�w���RGBA
	}
	pipelineDesc.SampleDesc.Count = 1;									// 1�s�N�Z���ɂ�1��T���v�����O

	// �e�N�X�`���T���v���[�̐ݒ�
	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		static_cast<UINT>(rootParams_.size()), rootParams_.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

void PipelineObj::ShaderCompileFromFile(std::string fileName, LPCSTR target, ID3D10Blob** shaderBlob)
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		Util::StringToWideChar(fileName).data(),		// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				// �C���N���[�h�\�ɂ���
		"main", target,									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,// �f�o�b�O�p�ݒ�
		0,
		shaderBlob, errorBlob_.GetAddressOf());

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}