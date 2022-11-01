#pragma once
// --WinAPI-- //
#include "FMWinAPI.h"

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

class DX12Cmd
{
#pragma region �����o�ϐ�
public:

private:
	// --�C���X�^���X-- //
	static DX12Cmd* myInstance_;

	ComPtr<ID3D12Device> device;// -> �f�o�C�X
	ComPtr<IDXGIFactory7> dxgiFactory;// -> DXGI�t�@�N�g���[
	ComPtr<IDXGISwapChain4> swapChain;// -> �X���b�v�`�F�[��
	ComPtr<ID3D12CommandAllocator> cmdAllocator;// -> �R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> commandList;// -> �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> commandQueue;// -> �R�}���h�L���[
	ComPtr<ID3D12DescriptorHeap> rtvHeap;// -> �����_�[�^�[�Q�b�g�r���[
	std::vector<ComPtr<ID3D12Resource>> backBuffers;// -> �o�b�N�o�b�t�@
	D3D12_RESOURCE_BARRIER barrierDesc;// -> ���\�[�X�o���A
	ComPtr<ID3D12Fence> fence;// -> �t�F���X
	UINT64 fenceVal;// -> �t�F���X�l

#pragma endregion

#pragma region �����o�֐�
public:
	// --�C���X�^���X�ǂݍ���-- //
	static DX12Cmd* GetInstance();

	// --�C���X�^���X���-- //
	void Release();

	// --����������-- //
	void Initialize(WinAPI * win);

	// --�`��O����-- //
	void PreDraw();

	// --�`��㏈��-- //
	void PostDraw();

#pragma region �Q�b�^�[
	
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

