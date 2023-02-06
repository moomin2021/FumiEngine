#include "FumiEngine.h"
#include "DX12Cmd.h"
#include "Texture.h"

int LoadTexture(const std::string fileName) {
	// 既に読み込んだ物だったら
	if (Texture::texBuff_.find(fileName) != Texture::texBuff_.end()) {
		return Texture::texHandle_[fileName];
	}

	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	// --WICテクスチャのロード-- //
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	// --ミップマップ生成-- //
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	// --読み込んだディフューズテクスチャをSRGBとして扱う-- //
	metadata.format = MakeSRGB(metadata.format);

	// --ヒープ設定-- //
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// --リソース設定-- //
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// --テクスチャバッファの生成-- //
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --全ミップマップについて-- //
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,// --------------> 全領域へコピー
			img->pixels,// ----------> 元データアドレス
			(UINT)img->rowPitch,// --> 1ラインサイズ
			(UINT)img->slicePitch// -> 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// --CBV, SRV, UAVの1個分のサイズを取得-- //
	UINT descriptorSize = DX12Cmd::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// --ハンドルを1つ進める-- //
	Texture::srvHandle_.ptr += descriptorSize;

	// --画像カウンタインクリメント-- //
	Texture::imageCount_++;

	// 設定を保存
	Texture::texBuff_.emplace(fileName, texBuff);
	Texture::texHandle_.emplace(fileName, descriptorSize * Texture::imageCount_);

	// --シェーダリソースビュー設定-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
	DX12Cmd::GetDevice()->CreateShaderResourceView(Texture::texBuff_[fileName].Get(), &srvDesc, Texture::srvHandle_);

	// --ハンドルを返す-- //
	return descriptorSize * Texture::imageCount_;
}
