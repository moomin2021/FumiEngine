#pragma once
#include "Pipeline.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <chrono>
#include <vector>

using namespace Microsoft::WRL;

class DX12Cmd {
#pragma region �����o�ϐ�
private:
	ComPtr<ID3D12Device> device_ = nullptr;			// �f�o�C�X
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;	// DXGI�t�@�N�g���[

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_ = {};		// �X���b�v�`�F�[���ݒ�
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;	// �X���b�v�`�F�[��

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_ = {};	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;// �����_�[�^�[�Q�b�g�r���[

	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;	// �R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> cmdList_ = nullptr;	// �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> cmdQueue_ = nullptr;			// �R�}���h�L���[

	std::vector<ComPtr<ID3D12Resource>> backBuffers_ = {};// �o�b�N�o�b�t�@

	UINT64 fenceVal_ = 0;					// �t�F���X�l
	ComPtr<ID3D12Fence> fence_ = nullptr;	// �t�F���X

	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter_ = nullptr;

	// �[�x�e�X�g
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;	// ���\�[�X�ݒ�
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;// �[�x�X�e���V���r���[

	D3D12_RESOURCE_BARRIER barrierDesc_;// ���\�[�X�o���A

	// �L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;

	// �p�C�v���C��
	PipelineSet pipelineObj3D_;			// �I�u�W�F�N�g3D�p
	PipelineSet pipelineSprite_;		// �X�v���C�g�p
	PipelineSet pieplinePostEffectTest_;// �e�X�g�|�X�g�G�t�F�N�g�p
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static DX12Cmd* GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �f�o�C�X��Ԃ�
	/// </summary>
	/// <returns> �f�o�C�X </returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// �R�}���h���X�g��Ԃ�
	/// </summary>
	/// <returns> �R�}���h���X�g </returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }

	/// <summary>
	/// Object3D�p�p�C�v���C����Ԃ�
	/// </summary>
	/// <returns> �p�C�v���C�� </returns>
	PipelineSet GetPipelineObject3D() { return pipelineObj3D_; }

	/// <summary>
	/// �X�v���C�g�p�p�C�v���C����Ԃ�
	/// </summary>
	/// <returns> �p�C�v���C�� </returns>
	PipelineSet GetPipelineSprite() { return pipelineSprite_; }

	/// <summary>
	/// �e�X�g�|�X�g�G�t�F�N�g�p�p�C�v���C����Ԃ�
	/// </summary>
	/// <returns> �p�C�v���C�� </returns>
	PipelineSet GetPipelinePostEffectTest() { return pieplinePostEffectTest_; }
#pragma endregion

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DX12Cmd();

	/// <summary>
	/// FPS�Œ�X�V����
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// �f�o�b�N���C���[�����ݒ�
	/// </summary>
	void InitSetDebugLayer();

	/// <summary>
	/// �A�_�v�^�����ݒ�
	/// </summary>
	void InitSetAdapter();

	/// <summary>
	/// �f�o�C�X����
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// �G���[���̐ݒ�
	/// </summary>
	void ErrorSet();

	/// <summary>
	/// �R�}���h���X�g����
	/// </summary>
	void CreateCmdList();

	/// <summary>
	/// �R�}���h�L���[����
	/// </summary>
	void CreateCmdQueue();

	/// <summary>
	/// �X���b�v�`�F�[������
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[����
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// �t�F���X����
	/// </summary>
	void CreateFence();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();
#pragma endregion

#pragma region ����֐�
	// �֎~
	DX12Cmd(const DX12Cmd&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	DX12Cmd& operator = (const DX12Cmd&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};