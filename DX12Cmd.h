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
#pragma region メンバ変数
private:
	ComPtr<ID3D12Device> device_ = nullptr;			// デバイス
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;	// DXGIファクトリー

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_ = {};		// スワップチェーン設定
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;	// スワップチェーン

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_ = {};	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;// レンダーターゲットビュー

	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;	// コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> cmdList_ = nullptr;	// コマンドリスト
	ComPtr<ID3D12CommandQueue> cmdQueue_ = nullptr;			// コマンドキュー

	std::vector<ComPtr<ID3D12Resource>> backBuffers_ = {};// バックバッファ

	UINT64 fenceVal_ = 0;					// フェンス値
	ComPtr<ID3D12Fence> fence_ = nullptr;	// フェンス

	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter_ = nullptr;

	// 深度テスト
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;	// リソース設定
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;// 深度ステンシルビュー

	D3D12_RESOURCE_BARRIER barrierDesc_;// リソースバリア

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	// パイプライン
	PipelineSet pipelineObj3D_;			// オブジェクト3D用
	PipelineSet pipelineSprite_;		// スプライト用
	PipelineSet pieplinePostEffectTest_;// テストポストエフェクト用
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns> インスタンス </returns>
	static DX12Cmd* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

#pragma region ゲッター関数
	/// <summary>
	/// デバイスを返す
	/// </summary>
	/// <returns> デバイス </returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// コマンドリストを返す
	/// </summary>
	/// <returns> コマンドリスト </returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }

	/// <summary>
	/// Object3D用パイプラインを返す
	/// </summary>
	/// <returns> パイプライン </returns>
	PipelineSet GetPipelineObject3D() { return pipelineObj3D_; }

	/// <summary>
	/// スプライト用パイプラインを返す
	/// </summary>
	/// <returns> パイプライン </returns>
	PipelineSet GetPipelineSprite() { return pipelineSprite_; }

	/// <summary>
	/// テストポストエフェクト用パイプラインを返す
	/// </summary>
	/// <returns> パイプライン </returns>
	PipelineSet GetPipelinePostEffectTest() { return pieplinePostEffectTest_; }
#pragma endregion

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DX12Cmd();

	/// <summary>
	/// FPS固定更新処理
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// デバックレイヤー初期設定
	/// </summary>
	void InitSetDebugLayer();

	/// <summary>
	/// アダプタ初期設定
	/// </summary>
	void InitSetAdapter();

	/// <summary>
	/// デバイス生成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// エラー時の設定
	/// </summary>
	void ErrorSet();

	/// <summary>
	/// コマンドリスト生成
	/// </summary>
	void CreateCmdList();

	/// <summary>
	/// コマンドキュー生成
	/// </summary>
	void CreateCmdQueue();

	/// <summary>
	/// スワップチェーン生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();
#pragma endregion

#pragma region 特殊関数
	// 禁止
	DX12Cmd(const DX12Cmd&) = delete;				// コピーコンストラクタ禁止
	DX12Cmd& operator = (const DX12Cmd&) = delete;	// コピー代入演算子禁止
#pragma endregion
};