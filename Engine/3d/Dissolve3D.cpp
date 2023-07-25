#include "Dissolve3D.h"
#include "DX12Cmd.h"
#include "PipelineManager.h"

#include <cassert>

void Dissolve3D::Initialize()
{
	// モデル生成
	model_ = std::make_unique<Model>("dissolve");

	// オブジェクト生成
	object_ = std::make_unique<Object3D>(model_.get());

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

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

void Dissolve3D::Update()
{
	// オブジェクト更新処理
	object_->Update();

	// 定数バッファを転送
	constMap_->dissolveTime = time_;
}

void Dissolve3D::Draw()
{
	PipelineManager::GetInstance()->PreDraw("Dissolve");

	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// 定数バッファビュー（CBV）の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(4, constBuff_->GetGPUVirtualAddress());

	// オブジェクト描画処理
	object_->Draw();
}