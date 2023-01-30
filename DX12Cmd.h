#pragma once
// --WinAPI-- //
#include "WinAPI.h"

// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DXGI�p-- //
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

// --�����^�ł��邱�Ƃ𔻒肷��@�\�̒ǉ�-- //
#include <cassert>

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --�V�[�P���X�R���e�i�̈��-- //
#include <vector>

// --������-- //
#include <string>

// --���Ԃ������ׂ̕W�����C�u����-- //
#include <chrono>

// --�p�C�v���C��-- //
#include "Pipeline.h"

class DX12Cmd
{
#pragma region �����o�ϐ�
public:

private:
	static ComPtr<ID3D12Device> device_;// -> �f�o�C�X
	ComPtr<IDXGIFactory7> dxgiFactory;// -> DXGI�t�@�N�g���[
	ComPtr<IDXGISwapChain4> swapChain;// -> �X���b�v�`�F�[��
	ComPtr<ID3D12CommandAllocator> cmdAllocator;// -> �R�}���h�A���P�[�^
	static ComPtr<ID3D12GraphicsCommandList> commandList;// -> �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> commandQueue;// -> �R�}���h�L���[
	ComPtr<ID3D12DescriptorHeap> rtvHeap;// -> �����_�[�^�[�Q�b�g�r���[
	std::vector<ComPtr<ID3D12Resource>> backBuffers;// -> �o�b�N�o�b�t�@
	D3D12_RESOURCE_BARRIER barrierDesc;// -> ���\�[�X�o���A
	ComPtr<ID3D12Fence> fence;// -> �t�F���X
	UINT64 fenceVal;// -> �t�F���X�l
	ComPtr<ID3D12DescriptorHeap> dsvHeap;// -> �[�x�X�e���V���r���[
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12Resource> depthBuff;// -> ���\�[�X�ݒ�

	// �L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;

	// --�X�v���C�g�p�̃p�C�v���C��-- //
	static PipelineSet spritePipeline_;

	// --�I�u�W�F�N�g3D�p�̃p�C�v���C��-- //
	static PipelineSet object3DPipeline_;

	// --�r���{�[�h�p�̃p�C�v���C��-- //
	//static PipelineSet billBoardPipeline_;

#pragma endregion

#pragma region �����o�֐�
public:
	// --�C���X�^���X�ǂݍ���-- //
	static DX12Cmd* GetInstance();

	// --����������-- //
	void Initialize(WinAPI * win);

	// FPS�Œ菉��������
	void InitializeFixFPS();

	// FPS�Œ�X�V����
	void UpdateFixFPS();

	// --�`��O����-- //
	void PreDraw();

	// --�`��㏈��-- //
	void PostDraw();

#pragma region �Q�b�^�[
	// --�f�o�C�X���擾-- //
	static ID3D12Device* GetDevice() { return device_.Get(); }

	// --�R�}���h���X�g���擾-- //
	static ID3D12GraphicsCommandList* GetCmdList() { return commandList.Get(); }

	// --�X�v���C�g�p�̃p�C�v���C�����擾-- //
	static PipelineSet GetSpritePipeline() { return spritePipeline_; }

	// --�I�u�W�F�N�g3D�p�̃p�C�v���C�����擾-- //
	static PipelineSet GetObject3DPipeline() { return object3DPipeline_; }

	// --�r���{�[�h�p�̃p�C�v���C�����擾
	//static PipelineSet GetBillBoardPipeline() { return billBoardPipeline_; }
#pragma endregion

private:
	// --�R���X�g���N�^-- //
	DX12Cmd();

	// --�f�X�g���N�^-- //
	~DX12Cmd();
#pragma endregion

	// --�֎~-- //
	DX12Cmd(const DX12Cmd&) = delete;// --------------> �R�s�[�R���X�g���N�^�֎~
	DX12Cmd& operator = (const DX12Cmd&) = delete;// -> �R�s�[������Z�q�֎~
};

