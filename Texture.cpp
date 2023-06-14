#include "Texture.h"
#include "DX12Cmd.h"
#include "float4.h"
#include "Util.h"

#include <vector>

using namespace DirectX;

Texture* Texture::GetInstance() {
	// インスタンス生成
	static Texture instance;

	// インスタンスを返す
	return &instance;
}

void Texture::Initialize() {
	// SRV用でスクリプタヒープを生成
	CreateDescriptorHeap();

	// デバック用のテクスチャを読み込む
	LoadTexture("Resources/debug.png");
}

int Texture::LoadTexture(const std::string fileName)
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 既に読み込んだ物だったら
	if (texBuff_.find(fileName) != texBuff_.end()) {
		return texHandle_[fileName];
	}

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WICテクスチャのロード
	result = LoadFromWICFile(
		Util::StringToWideChar(fileName).data(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = static_cast<uint32_t>(metadata.height);
	textureResourceDesc.DepthOrArraySize = static_cast<uint32_t>(metadata.arraySize);
	textureResourceDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			static_cast<uint32_t>(i),
			nullptr,								// 全領域へコピー
			img->pixels,							// 元データアドレス
			static_cast<uint32_t>(img->rowPitch),	// 1ラインサイズ
			static_cast<uint32_t>(img->slicePitch)	// 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// CBV, SRV, UAVの1個分のサイズを取得
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 読み込んだ画像のハンドル
	uint16_t handle = descriptorSize * loadCounter_;

	// 設定を保存
	texBuff_.emplace(fileName, texBuff);
	texHandle_.emplace(fileName, handle);

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// ハンドルの指す①にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff_[fileName].Get(), &srvDesc, srvHandle_);

	// ハンドルを1つ進める
	srvHandle_.ptr += descriptorSize;

	// カウンターを進める
	loadCounter_++;

	// ハンドルを返す
	return handle;
}

Texture::Texture() {}

void Texture::CreateDescriptorHeap()
{
	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2056;

	// 設定をもとにSRV用デスクリプタヒープを生成
	result = DX12Cmd::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// SRVヒープの先頭ハンドルを取得
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();
}

int LoadTexture(const std::string fileName) {
	// 画像読み込み
	return Texture::GetInstance()->LoadTexture(fileName);
}
