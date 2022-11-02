#pragma once
// --WinAPI-- //
#include "FMWinAPI.h"

// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DXGI用-- //
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

// --式が真であることを判定する機能の追加-- //
#include <cassert>

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --シーケンスコンテナの一種-- //
#include <vector>

// --文字列-- //
#include <string>

class DX12Cmd
{
#pragma region メンバ変数
public:

private:
	// --インスタンス-- //
	static DX12Cmd* myInstance_;

	ComPtr<ID3D12Device> device;// -> デバイス
	ComPtr<IDXGIFactory7> dxgiFactory;// -> DXGIファクトリー
	ComPtr<IDXGISwapChain4> swapChain;// -> スワップチェーン
	ComPtr<ID3D12CommandAllocator> cmdAllocator;// -> コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> commandList;// -> コマンドリスト
	ComPtr<ID3D12CommandQueue> commandQueue;// -> コマンドキュー
	ComPtr<ID3D12DescriptorHeap> rtvHeap;// -> レンダーターゲットビュー
	std::vector<ComPtr<ID3D12Resource>> backBuffers;// -> バックバッファ
	D3D12_RESOURCE_BARRIER barrierDesc;// -> リソースバリア
	ComPtr<ID3D12Fence> fence;// -> フェンス
	UINT64 fenceVal;// -> フェンス値

#pragma endregion

#pragma region メンバ関数
public:
	// --インスタンス読み込み-- //
	static DX12Cmd* GetInstance();

	// --インスタンス解放-- //
	void Release();

	// --初期化処理-- //
	void Initialize(WinAPI * win);

	// --描画前処理-- //
	void PreDraw();

	// --描画後処理-- //
	void PostDraw();

#pragma region ゲッター
	
#pragma endregion

private:
	// --コンストラクタ-- //
	DX12Cmd();

	// --デストラクタ-- //
	~DX12Cmd();
#pragma endregion

	// --禁止-- //
	DX12Cmd(const DX12Cmd&) = delete;// --------------> コピーコンストラクタ禁止
	DX12Cmd& operator = (const DX12Cmd&) = delete;// -> コピー代入演算子禁止
};

