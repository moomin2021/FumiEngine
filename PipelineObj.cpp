#include "PipelineObj.h"
#include "DX12Cmd.h"
#include "Util.h"

#include <d3dcompiler.h>
#include <d3d12.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

// �f�X�N���v�^�����W�̐ݒ�
CD3DX12_DESCRIPTOR_RANGE PipelineObj::descRangeSRV_ = {};

PipelineObj::PipelineObj()
{
	// �f�X�N���v�^�����W�̐ݒ�
	descRangeSRV_.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);// t0 ���W�X�^
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

void PipelineObj::CreateRootParams(uint16_t addValue)
{
	for (size_t i = 0; i < addValue; i++) {
		// ���[�g�p�����[�^������₷
		rootParams_.emplace_back();

		if (i == 0) {
			rootParams_[i].InitAsDescriptorTable(1, &descRangeSRV_, D3D12_SHADER_VISIBILITY_ALL);
		}

		else {
			rootParams_[i].InitAsConstantBufferView(static_cast<UINT>(i - 1), 0, D3D12_SHADER_VISIBILITY_ALL);
		}
	}
}

void PipelineObj::CreatePipeline() {
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
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc = pipelineDesc.BlendState.RenderTarget[1];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��

	// �A���t�@�l���ʐݒ�
	blenddesc.BlendEnable = true;					// �u�����h��L������
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;		// �f�X�g�̒l��0%�g��

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;		//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout_.data();
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout_.size());

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 2;								// �`��Ώۂ�2��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0~255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;								// 1�s�N�Z���ɂ�1��T���v�����O

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// ���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// �c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// ���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams_.data();// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParams_.size());
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
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