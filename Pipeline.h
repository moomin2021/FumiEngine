#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --�V�F�[�_�̓ǂݍ��݂ƃR���p�C���p-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --������-- //
#include <string>

// --�����^�ł��邱�Ƃ𔻒肷��@�\�̒ǉ�-- //
#include <cassert>

// --�p�C�v���C���Z�b�g-- //
struct PipelineSet {
	// --�p�C�v���C���X�e�[�g�I�u�W�F�N�g-- //
	ComPtr<ID3D12PipelineState> pipelineState;

	// --���[�g�V�O�l�`��-- //
	ComPtr<ID3D12RootSignature> rootSignature;
};

// --�X�v���C�g�p�p�C�v���C������-- //
PipelineSet CreateSpritePipeline();

// --�I�u�W�F�N�g3D�̃p�C�v���C������-- //
PipelineSet CreateObject3DPipeline();

// --�r���{�[�h�p�̃p�C�v���C������-- //
PipelineSet CreateBillBoardPipeline();