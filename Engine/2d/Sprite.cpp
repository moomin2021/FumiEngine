#include "Sprite.h"
#include "DX12Cmd.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Util.h"

#include <DirectXMath.h>

using namespace DirectX;

Sprite::Sprite() :
#pragma region 初期化リスト
	// スプライトデータ
	position_{ 0.0f, 0.0f },			// 座標
	rotation_(0.0f),					// 回転
	size_{ 100.0f, 100.0f },			// サイズ
	color_{ 1.0f, 1.0f, 1.0f, 1.0f },	// 色(RGBA)
	anchorPoint_{0.0f, 0.0f},			// アンカーポイント(XY)
	isFlipX_(false),					// 左右反転
	isFlipY_(false),					// 上下反転

	// スプライトデータを変更したかどうか
	hasChanget_(true),

	// 行列
	matWorld_{},		// ワールド行列
	matProjection_{},	// 射影変換行列

	// 定数バッファ
	constBuff_(nullptr),// 定数バッファ
	constMap_(nullptr),	// マッピング処理用

	// 頂点データ
	vertex_(4),
	vertexBuff_(nullptr),	// 頂点バッファ
	vertexMap_(nullptr),	// マッピング処理用
	vertexView_{},			// 頂点バッファビュー

	// インデックスデータ
	index_(6),
	indexBuff_(nullptr),// インデックスバッファ
	indexView_{}		// インデックスバッファビュー
#pragma endregion
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// インスタンス取得
	WinAPI* winAPI = WinAPI::GetInstance();

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// 頂点データ
	vertex_[0] = { { 0.0f, 0.0f}, {0.0f, 1.0f} };// 左下
	vertex_[1] = { { 0.0f, 0.0f}, {0.0f, 0.0f} };// 左上
	vertex_[2] = { { 0.0f, 0.0f}, {1.0f, 1.0f} };// 右下
	vertex_[3] = { { 0.0f, 0.0f}, {1.0f, 0.0f} };// 右上

	// インデックスデータ
	index_[0] = 0;
	index_[1] = 1;
	index_[2] = 2;
	index_[3] = 1;
	index_[4] = 2;
	index_[5] = 3;

#pragma region 頂点バッファ確保
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertex_[0]) * vertex_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapVSProp{};			// ヒープ設定
	heapVSProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

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
		&heapVSProp,// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region 頂点バッファへのデータ伝送
	// Map処理でメインメモリとGPUのメモリを紐づける
	result = vertexBuff_->Map(0, nullptr, (void**)&vertexMap_);
	assert(SUCCEEDED(result));

	// 全頂点に対して
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		// 座標をコピー
		vertexMap_[i] = vertex_[i];
	}
#pragma endregion

#pragma region 頂点バッファビュー作成

	// GPU仮想アドレス
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();

	// 頂点バッファのサイズ
	vertexView_.SizeInBytes = sizeVB;

	// 頂点1つ分のデータサイズ
	vertexView_.StrideInBytes = sizeof(vertex_[0]);
#pragma endregion

#pragma region インデックスバッファ確保
	// インデックスデータ全体のサイズ = インデックスデータ一つ分のサイズ * インデックスデータの要素数
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * index_.size());

	// リリース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapVSProp,// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);
#pragma endregion

#pragma region インデックスバッファへのデータ転送
	// マッピング用
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);

	// 全インデックスに対して
	for (int i = 0; i < index_.size(); i++) {
		indexMap[i] = index_[i];
	}

	// マッピング解除
	indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファビューの作成
	// GPU仮想アドレス
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();

	// フォーマット指定
	indexView_.Format = DXGI_FORMAT_R16_UINT;

	// インデックスデータ1つ分のサイズ
	indexView_.SizeInBytes = sizeIB;
#pragma endregion

#pragma region 定数バッファ生成
	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region 定数バッファへのデータ転送
	// 定数バッファのマッピング
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
#pragma endregion

	// 射影変換行列初期化
	XMMATRIX mat = XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(winAPI->GetWidth()),
		static_cast<float>(winAPI->GetHeight()),
		0.0f, 0.0f, 1.0f);

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			matProjection_.m[i][j] = mat.r[i].m128_f32[j];
		}
	}
}

void Sprite::Draw(int textureHandle) {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// インスタンス取得
	Texture* tex = Texture::GetInstance();

	// SRVヒープのハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// スプライトデータの更新処理
	UpdateData();

	// ハンドルを指定された分まで進める
	srvGpuHandle.ptr += textureHandle;

	// 指定されたSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// 定数バッファビュー（CBV）の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&indexView_);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(static_cast<UINT>(index_.size()), 1, 0, 0, 0);
}

void Sprite::PreDraw()
{
	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// パイプライン取得
	PipelineSet pipeline = DX12Cmd::GetInstance()->GetPipelineSprite();

	// SRVヒープ取得
	ID3D12DescriptorHeap* srvHeap = Texture::GetInstance()->GetSRVHeap();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// デスクリプタヒープの配列をセットするコマンド
	std::vector<ID3D12DescriptorHeap*> ppHeaps = { srvHeap };
	cmdList->SetDescriptorHeaps(1, ppHeaps.data());
}

void Sprite::UpdateData()
{
	// スプライトデータの変更がされていなかったら処理を飛ばす
	if (hasChanget_ == false) return;

#pragma region ワールド行列計算
	// 行列初期化
	matWorld_ = Matrix4Identity();

	// Z軸回転
	matWorld_ *= Matrix4RotateZ(Util::Degree2Radian(rotation_));

	// 平行移動
	matWorld_ *= Matrix4Translate({position_.x, position_.y, 0.0f});
#pragma endregion

#pragma region 定数バッファの転送
	// 行列計算
	constMap_->mat = matWorld_ * matProjection_;

	// 色(RGBA)
	constMap_->color = color_;
#pragma endregion

#pragma region 頂点座標変更(画像のサイズを変更)
	// 頂点データ
	float left		= (0.0f - anchorPoint_.x);
	float right		= (1.0f - anchorPoint_.x);
	float top		= (0.0f - anchorPoint_.y);
	float bottom	= (1.0f - anchorPoint_.y);

	// 左右反転
	if (isFlipX_) left = -left, right = -right;

	// 上下反転
	if (isFlipY_) top = -top, bottom = -bottom;

	vertex_[0] = { { left	* size_.x, bottom	* size_.y }, {0.0f, 1.0f} };// 左下
	vertex_[1] = { { left	* size_.x, top		* size_.y }, {0.0f, 0.0f} };// 左上
	vertex_[2] = { { right	* size_.x, bottom	* size_.y }, {1.0f, 1.0f} };// 右下
	vertex_[3] = { { right	* size_.x, top		* size_.y }, {1.0f, 0.0f} };// 右上

	// 全頂点に対して
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		// 座標をコピー
		vertexMap_[i] = vertex_[i];
	}
#pragma endregion

	// 変更したのでフラグを[OFF]にする
	hasChanget_ = false;
}