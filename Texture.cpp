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
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

#pragma region 真っ白な画像を作成

	// 画像のサイズ
	const uint32_t textureWidth = 256;	// 横幅
	const uint32_t textureHeight = 256;	// 縦幅
	const uint32_t imageDataCount = textureWidth * textureHeight;// 全体のピクセル

	// 1ピクセルの色
	std::vector<float4> imageData;
	imageData.resize(imageDataCount);

	// 画像を真っ白に
	for (size_t i = 0; i < imageDataCount; i++) {
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;
	}

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;
	textureResourceDesc.Height = textureHeight;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	// --テクスチャバッファの生成-- //
	ComPtr<ID3D12Resource> texBuff;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --テクスチャバッファにデータ転送-- //
	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		imageData.data(),
		sizeof(float4) * textureWidth,
		sizeof(float4) * imageDataCount
	);
	assert(SUCCEEDED(result));

#pragma endregion

	// --デスクリプタヒープの設定-- //
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2056;

	// --設定をもとにSRV用デスクリプタヒープを生成-- //
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// --SRVヒープの先頭ハンドルを取得-- //
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	// --シェーダリソースビュー設定-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// 設定を保存
	texBuff_.emplace("white", texBuff);
	texHandle_.emplace("white", 0);

	// ハンドルの指す①にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff_["white"].Get(), &srvDesc, srvHandle_);
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

	// ハンドルを1つ進める
	srvHandle_.ptr += descriptorSize;

	// カウンターを進める
	loadCounter_++;

	// 設定を保存
	texBuff_.emplace(fileName, texBuff);
	texHandle_.emplace(fileName, descriptorSize * loadCounter_);

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
	device->CreateShaderResourceView(texBuff_[fileName].Get(), &srvDesc, srvHandle_);

	// --ハンドルを返す-- //
	return descriptorSize * loadCounter_;
}

Texture::Texture() {}

int LoadTexture(const std::string fileName) {
	// 画像読み込み
	return Texture::GetInstance()->LoadTexture(fileName);
}
