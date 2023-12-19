#include "Instancing3D.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "Util.h"
#include "Buffer.h"

// 静的メンバ変数の実態
Camera* Instancing3D::sCamera_ = nullptr;// カメラ
LightGroup* Instancing3D::sLightGroup_ = nullptr;// ライト

Instancing3D::Instancing3D(uint16_t instNum, Model* model) : instNum_(instNum), model_(model)
{
	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// 定数バッファ生成
	constBuff_ = CreateBufferResource((sizeof(ConstBufferData) + 0xff) & ~0xff);
	
	// 定数バッファへのデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	// インスタンシング用リソースを生成
	instBuff_ = CreateBufferResource(sizeof(Matrix4) * instNum_);

	result = instBuff_->Map(0, nullptr, (void**)&instMap_);
	assert(SUCCEEDED(result));
}

void Instancing3D::MatUpdate()
{
	// オブジェクトデータが変更されていたら処理する
	if (hasChanget_)
	{
#pragma region ワールド行列計算
		// 行列初期化
		matWorld_ = Matrix4Identity();

		// ワールド行列にスケーリングを反映
		matWorld_ *= Matrix4Scale(scale_);

		// ワールド行列に回転を反映
		matWorld_ *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
		matWorld_ *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
		matWorld_ *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));

		// ワールド行列に平行移動を反映
		matWorld_ *= Matrix4Translate(position_);
#pragma endregion

		// 変更したのでフラグを[OFF]にする
		hasChanget_ = false;
	}

#pragma region 定数バッファへのデータ転送
	// ビュープロジェクション転送
	constMap_->viewProj = sCamera_->GetMatView() * sCamera_->GetMatProjection();

	// カメラの位置座標(XYZ)転送
	constMap_->cameraPos = sCamera_->GetEye();

	// 色(RGBA)転送
	constMap_->color = color_;
#pragma endregion
}

void Instancing3D::Draw()
{
	// 描画フラグが[OFF]ならこれ以降処理しない
	if (isDraw_ == false) return;

	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// 定数バッファビュー（CBV）の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// ライトの描画
	sLightGroup_->Draw();

	// モデルの情報を元に描画
	model_->Draw();
}
