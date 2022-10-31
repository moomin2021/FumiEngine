#include "FMDX12Cmd.h"

// --インスタンスの初期化-- //
DX12Cmd* DX12Cmd::myInstance_ = nullptr;

// --インスタンス読み込み-- //
DX12Cmd* DX12Cmd::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance_ == nullptr) myInstance_ = new DX12Cmd();

	// --インスタンスを返す-- //
	return myInstance_;
}

// --インスタンス解放-- //
void DX12Cmd::Release() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance_ == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance_;
	myInstance_ = nullptr;
}

// --コンストラクタ-- //
DX12Cmd::DX12Cmd()
#pragma region 定数初期化

#pragma endregion
{

}

// --デストラクタ-- //
DX12Cmd::~DX12Cmd() {

}

void DX12Cmd::Initialize(WinAPI* win) {
	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	/// --アダプタの列挙-- ///
	/// ※PCにあるグラフィックボードを、仮想的なデバイスを含めて全部リストアップする ///
#pragma region

	// --DXGIファクトリーの生成-- //
	// DXGI = グラフィックスインフラストラクチャ
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// --アダプターの列挙用-- //
	std::vector<IDXGIAdapter4*> adapters;

	// --ここに特定の名前を持つアダプターオブジェクトが入る-- //
	IDXGIAdapter4* tmpAdapter = nullptr;

	// --パフォーマンスが高いものから順に、全てのアダプターを列挙する-- //
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

#pragma endregion
	/// --END-- ///

	/// --アダプタの選別-- //
	/// ※検出されたグラフィックスデバイスの中で性能の低いもの除外して、専用デバイスを採用する ///
#pragma region

	// --妥当なアダプタを選別する-- //
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --デバイスの生成-- ///
	/// ※採用したグラフィックスデバイスを操作するためにD3D12Deviceオブジェクトを生成 ///
	/// ※これは普通１ゲームに1つしか作らない ///
#pragma region

	// --対応レベルの配列-- //
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	// --グラフィックスデバイスを操作する為のオブジェクト生成-- //
	// ※これは普通、１ゲームに１つしか作らない
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --コマンドリスト-- ///
	/// ※GPUに、まとめて命令を送るためのコマンドリストを生成する //
#pragma region

	// --コマンドアロケータを生成-- //
	// ※コマンドリストはコマンドアロケータから生成するので、先にコマンドアロケータを作る //
	// ※コマンドリストに格納する命令の為のメモリを管理するオブジェクト //
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,// -> コマンドアロケータの種類
		IID_PPV_ARGS(&cmdAllocator));// -> 各インターフェイス固有のGUID
	assert(SUCCEEDED(result));// -> ID3D12CommandAllocatorインターフェイスのポインタを格納する変数アドレス

	// --コマンドリストを生成-- //
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --コマンドキュー-- ///
	/// ※コマンドリストをGPUに順に実行させていく為の仕組み ///
#pragma region

	// --コマンドキューの設定-- //
	// ※{}をつけることで構造体の中身を0でクリアしている。
	// ※値0が標準値になるようにMicrosoftが決めているので、この場合コマンドキューの設定を標準にしている //
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// --標準設定でコマンドキューを生成-- //
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --スワップチェーン-- ///
	/// ※スワップチェーンは、ダブルバッファリングやトリプルバッファリングを簡単に実装するための仕組み ///
	/// ※表示中の画面（フロントバッファ）・描画中の画面（バックバッファ）
#pragma region

	// --スワップチェーンの設定-- //
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// --画面の幅を指定する
	swapChainDesc.Width = win->GetWidth();;

	// --画面の高さを指定する
	swapChainDesc.Height = win->GetHeight();

	// --色情報の書式（表示形式）
	//※DXGI_FORMAT_R8G8B8A8_UNORMはアルファを含むチャンネルあたり8ビットをサポート
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// --ピクセルあたりのマルチサンプルの数を指定する
	swapChainDesc.SampleDesc.Count = 1;

	// --リソースの使用方法を指定
	// ※DXGI_USAGE_BACK_BUFFERはリソースをバックバッファとして使用する
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	// --スワップチェーン内のバッファーの数を指定する
	swapChainDesc.BufferCount = 2;

	// --画面をスワップした後のリソースの処理方法を指定
	// ※DXGI_SWAP_EFFECT_FLIP_DISCARDはスワップした後バックバッファーの内容を破棄する設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// --スワップチェーン動作のオプションを指定
	// ※DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCHはウィンドウとフルスクリーンの切り替え時に>>
	// >>解像度がウィンドウサイズに一致するように変更する
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// --IDXGISwapChain1のComPtrを用意-- //
	ComPtr<IDXGISwapChain1> swapChain1;

	// --スワップチェーンの生成-- //
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), win->GetHWND(), &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain1);
	assert(SUCCEEDED(result));

	// --生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する-- //
	swapChain1.As(&swapChain);

#pragma endregion
	/// --END-- ///
}

// --アダプタ初期化処理-- //
void DX12Cmd::InitializeAdapter() {

}