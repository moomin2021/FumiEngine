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

class Texture {
	///  --メンバ変数-- ///
public:

private:
	// --デバイス-- //
	ID3D12Device* device_;

	// --SRVヒープの先頭ハンドルを取得-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	// --読み込む画像が何枚目か-- //
	UINT imageCount_;

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

	// --テクスチャの読み込み-- //
	int LoadTexture(const wchar_t* szFile);

	// --SRVヒープ参照-- //
	static ID3D12DescriptorHeap* GetSRVHeap();

private:
	// --コンストラクタ-- //
	Texture();

	// --デストラクタ-- //
	~Texture();

	/// --メンバ関数END-- ///
};