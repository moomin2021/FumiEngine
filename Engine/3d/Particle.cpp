#include "Particle.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "Util.h"

#include <cassert>

// 静的メンバ変数の実態
Camera* Particle::sCamera_ = nullptr;

Particle::Particle()
{
	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

#pragma region 頂点設定
	vertices_.resize(4);
	vertices_[0] = { {-5.0f, -5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };
	vertices_[1] = { {-5.0f, +5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
	vertices_[2] = { {+5.0f, -5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };
	vertices_[3] = { {+5.0f, +5.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} };
#pragma endregion

#pragma region インデックス設定
	indexes_.resize(6);
	indexes_[0] = 0;
	indexes_[1] = 1;
	indexes_[2] = 2;
	indexes_[3] = 2;
	indexes_[4] = 1;
	indexes_[5] = 3;
#pragma endregion

	// 頂点バッファ作成
	CreateVertexBuff();

	// インデックスバッファ作成
	CreateIndexBuff();

#pragma region 定数バッファ生成
	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};		// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region 定数バッファへのデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
#pragma endregion
}

void Particle::Update()
{
#pragma region ワールド行列計算
	// 行列初期化
	Matrix4 matWorld = Matrix4Identity();

	// ワールド行列にスケーリングを反映
	matWorld *= Matrix4Scale(scale_);

	// ワールド行列に回転を反映
	matWorld *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
	matWorld *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
	matWorld *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));

	// ワールド行列に平行移動を反映
	matWorld *= Matrix4Translate(position_);
#pragma endregion

#pragma region 定数バッファへのデータ転送
	// 行列
	constMap_->mat = matWorld * sCamera_->GetMatView() * sCamera_->GetMatProjection();
#pragma endregion
}

void Particle::Draw()
{
	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// インスタンス取得
	Texture* tex = Texture::GetInstance();

	// SRVヒープのハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// 指定されたSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// 定数バッファビュー（CBV）の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&indexView_);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(static_cast<UINT>(indexes_.size()), 1, 0, 0, 0);
}

void Particle::CreateVertexBuff()
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数実行の成否を判別用の変数
	HRESULT result;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();// GPU仮想アドレス
	vertexView_.SizeInBytes = sizeVB;				// 頂点バッファのサイズ
	vertexView_.StrideInBytes = sizeof(vertices_[0]);	// 頂点1つ分のデータサイズ

	// Map処理でメインメモリとGPUのメモリを紐づける
	Vertex* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 全頂点に対して
	for (size_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertexBuff_->Unmap(0, nullptr);
}

void Particle::CreateIndexBuff()
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数実行の成否を判別用の変数
	HRESULT result;

	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexes_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp,// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// インデックスバッファビュー作成
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexView_.Format = DXGI_FORMAT_R16_UINT;
	indexView_.SizeInBytes = sizeIB;

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// 全インデックスに対して
	for (size_t i = 0; i < indexes_.size(); i++)
	{
		indexMap[i] = indexes_[i];
	}

	// マッピング解除
	indexBuff_->Unmap(0, nullptr);
}