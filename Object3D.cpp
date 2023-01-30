#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --便利系関数-- //
#include "Util.h"

D3D12_HEAP_PROPERTIES Object3D::heapProp_ = {};
D3D12_RESOURCE_DESC Object3D::resdesc_ = {};
ID3D12Device* Object3D::device_;
ID3D12GraphicsCommandList* Object3D::cmdList_= nullptr;
Camera* Object3D::camera_ = nullptr;

Object3D::Object3D() :
#pragma region 初期化リスト
	position_{ 0.0f, 0.0f, 0.0f },// -> 座標
	rotation_{ 0.0f, 0.0f, 0.0f },// -> 回転角
	scale_{ 1.0f, 1.0f, 1.0f },// ----> スケール

	matWorld_{},

	// 定数バッファ
	constBuff_(nullptr),// -> オブジェクト

	// モデル
	model_(nullptr),

	dirty(true)
#pragma endregion
{
	// 定数バッファ生成
	GenerateConstBuffer();
}

Object3D* Object3D::CreateObject3D(Model* model)
{
	// インスタンス生成
	Object3D* object = new Object3D();

	object->SetModel(model);

	// 生成したインスタンスを返す
	return object;
}

void Object3D::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
	device_ = device;// -> デバイス
	cmdList_ = cmdList;// -> コマンドリスト

	Model::Initialize(cmdList);

	// 定数バッファのヒープ設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファのリソース設定
	resdesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc_.Width = (sizeof(ObjectBuff) + 0xff) & ~0xff;
	resdesc_.Height = 1;
	resdesc_.DepthOrArraySize = 1;
	resdesc_.MipLevels = 1;
	resdesc_.SampleDesc.Count = 1;
	resdesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void Object3D::Draw()
{
	// オブジェクトの更新処理
	TransferConstBuffer();

	// オブジェクトの描画処理
	{
		// --定数バッファビュー（CBV）の設定コマンド-- //
		cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
		
		model_->Draw();
	}
}

void Object3D::PreDraw() {
	// パイプラインステートの設定
	cmdList_->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	// ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	// プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --デスクリプタヒープの配列をセットするコマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	cmdList_->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::SetPos(const Float3& position)
{
	position_ = position;
	dirty = true;
}

void Object3D::SetRot(const Float3& rotation)
{
	rotation_ = rotation;
	dirty = true;
}

void Object3D::SetScale(const Float3& scale)
{
	scale_ = scale;
	dirty = true;
}

void Object3D::GenerateConstBuffer() {
	// 関数実行の成否を判別用の変数
	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resdesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void Object3D::TransferConstBuffer() {
	if (dirty) {
		Matrix4 matScale, matRot, matTrans;
		matScale = Matrix4Scale(scale_);
		matRot = Matrix4Identity();
		matRot *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
		matRot *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
		matRot *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));
		matTrans = Matrix4Translate(position_);

		// --ワールド行列の合成-- //
		matWorld_ = Matrix4Identity();// -> 変形のリセット
		matWorld_ *= matScale;// -> ワールド行列にスケーリングを反映
		matWorld_ *= matRot;// -> ワールド行列に回転を反映
		matWorld_ *= matTrans;// -> ワールド行列に平行移動を反映

		dirty = false;
	}

	// 関数実行の成否を判別用の変数
	HRESULT result;

	Camera* camera = camera_;

	// マッピング
	ObjectBuff* objectMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&objectMap);
	assert(SUCCEEDED(result));

	// 定数バッファへデータ転送
	objectMap->mat = matWorld_ * camera->GetMatView() * camera->GetMatProjection();

	// マッピング終了処理
	constBuff_->Unmap(0, nullptr);
}