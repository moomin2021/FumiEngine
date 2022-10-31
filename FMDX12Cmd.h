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
	ComPtr<ID3D12CommandAllocator> cmdAllocator;// -> コマンドアロケータ
	ID3D12GraphicsCommandList* commandList;// -> コマンドリスト
	ComPtr<ID3D12CommandQueue> commandQueue;// -> コマンドキュー
	ComPtr<IDXGISwapChain4> swapChain;// -> スワップチェーン
#pragma endregion

#pragma region メンバ関数
public:
	// --インスタンス読み込み-- //
	static DX12Cmd* GetInstance();

	// --インスタンス解放-- //
	void Release();

	// --初期化処理-- //
	void Initialize(WinAPI * win);

#pragma region ゲッター
	
#pragma endregion

private:
	// --コンストラクタ-- //
	DX12Cmd();

	// --デストラクタ-- //
	~DX12Cmd();

	// --アダプタ初期化処理-- //
	void InitializeAdapter();
#pragma endregion

	// --禁止-- //
	DX12Cmd(const DX12Cmd&) = delete;// --------------> コピーコンストラクタ禁止
	DX12Cmd& operator = (const DX12Cmd&) = delete;// -> コピー代入演算子禁止
};

