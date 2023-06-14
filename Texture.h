#pragma once
#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>
#include <map>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

class Texture {
#pragma region メンバ変数
private:
	// 読み込んだ回数
	uint16_t loadCounter_;

	// テクスチャバッファ
	std::map<const std::string, ComPtr<ID3D12Resource>> texBuff_;

	// テクスチャハンドル
	std::map<const std::string, uint16_t> texHandle_;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	// SRVヒープのハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns> インスタンス </returns>
	static Texture* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// テクスチャを読み込み、ハンドルを取得する
	/// </summary>
	/// <param name="fileName"> 画像ファイル名 </param>
	/// <returns> 読み込んだ画像のハンドル </returns>
	int LoadTexture(const std::string fileName);

#pragma region ゲッター関数
	/// <summary>
	/// SRVデスクリプタヒープの取得
	/// </summary>
	/// <returns> SRVデスクリプタヒープ </returns>
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }
#pragma endregion

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture();

#pragma endregion

#pragma region 特殊関数
	// 禁止
	Texture(const Texture&) = delete;				// コピーコンストラクタ禁止
	Texture& operator = (const Texture&) = delete;	// コピー代入演算子禁止
#pragma endregion
};

/// <summary>
/// テクスチャを読み込み、ハンドルを取得する
/// </summary>
/// <param name="fileName"> 画像ファイル名 </param>
/// <returns> 読み込んだ画像のハンドル </returns>
int LoadTexture(const std::string fileName);