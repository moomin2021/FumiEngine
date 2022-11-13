#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --DirectXTex-- //
#include <DirectXteX.h>

 // --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

#include <array>

class Texture {
	///  --メンバ変数-- ///
public:
	// --読み込む画像が何枚目か-- //
	static UINT imageCount_;

	// テクスチャバッファ
	static std::array<ComPtr<ID3D12Resource>, 2056> texBuff_;

	// --SRVヒープの先頭ハンドルを取得-- //
	static D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

private:
	// --SRV用デスクリプタヒープ-- //
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	/// --メンバ変数END-- ///
	///---------------- ///
	/// --メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static Texture* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device_"> デバイス </param>
	void Initialize(ID3D12Device* device);

	// --SRVヒープ参照-- //
	static ID3D12DescriptorHeap* GetSRVHeap();

private:
	// --コンストラクタ-- //
	Texture();

	// --デストラクタ-- //
	~Texture();

	/// --メンバ関数END-- ///
};