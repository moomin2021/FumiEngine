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

	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 定数バッファ生成
	constBuff_ = CreateBufferResource((sizeof(ConstBufferData) + 0xff) & ~0xff);
	
	// 定数バッファへのデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	// インスタンシング用リソースを生成
	instBuff_ = CreateBufferResource(sizeof(Matrix4) * instNum_);

	result = instBuff_->Map(0, nullptr, (void**)&instMap_);
	assert(SUCCEEDED(result));

	index_ = Texture::GetInstance()->GetIndex();

	D3D12_SHADER_RESOURCE_VIEW_DESC instSRVDesc{};
	instSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	instSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instSRVDesc.Buffer.FirstElement = 0;
	instSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instSRVDesc.Buffer.NumElements = (UINT)instNum_;
	instSRVDesc.Buffer.StructureByteStride = sizeof(Matrix4);
	D3D12_CPU_DESCRIPTOR_HANDLE instSRVHandleCPU = Texture::GetInstance()->GetCPUDescriptorHandle(DescSIZE::SRV, index_);
	device->CreateShaderResourceView(instBuff_.Get(), &instSRVDesc, instSRVHandleCPU);
}

void Instancing3D::MatUpdate()
{
	// オブジェクトデータが変更されていたら処理する
	if (hasChanget_)
	{
		for (size_t i = 0; i < worlds_.size(); i++)
		{
			worlds_[i] = Matrix4Identity();
			worlds_[i] *= Matrix4Scale({ 1.0f, 1.0f, 1.0f });
			worlds_[i] *= Matrix4RotateZ(Util::Degree2Radian(0.0f));
			worlds_[i] *= Matrix4RotateX(Util::Degree2Radian(0.0f));
			worlds_[i] *= Matrix4RotateY(Util::Degree2Radian(0.0f));
			worlds_[i] *= Matrix4Translate(transforms_[i].position);
		}

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

	for (size_t i = 0; i < worlds_.size(); i++)
	{
		instMap_[i] = worlds_[i];
	}
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
	model_->InstDraw(instNum_, index_);
}

void Instancing3D::AddTransform(const Vector3& position)
{
	worlds_.emplace_back();
	transforms_.emplace_back(position);
	hasChanget_ = true;
}