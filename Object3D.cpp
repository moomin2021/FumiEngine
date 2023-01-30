#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --便利系関数-- //
#include "Util.h"

Object3D::Object3D() :
#pragma region 初期化リスト
	position_{0.0f, 0.0f, 0.0f},// -> 座標
	rotation_{0.0f, 0.0f, 0.0f},// -> 回転角
	scale_{1.0f, 1.0f, 1.0f},// ----> スケール

	// 定数バッファ
	objectBuff_(nullptr),// -> オブジェクト

	// 透視投影行列
	matProjection_{},

	// カメラ
	camera_(nullptr),

	// モデル
	model_(nullptr)
#pragma endregion
{
	// 初期化処理
	Initialize();
}

Object3D* Object3D::CreateObject3D()
{
	// インスタンス生成
	Object3D* object = new Object3D();

	// 生成したインスタンスを返す
	return object;
}

void Object3D::Draw()
{
	// オブジェクトの更新処理
	{
		// --スケール、回転、平行移動行列の計算-- //
		XMMATRIX matScale, matRot, matTrans;
		matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation_.z));
		matRot *= XMMatrixRotationX(Util::Degree2Radian(rotation_.x));
		matRot *= XMMatrixRotationY(Util::Degree2Radian(rotation_.y));
		matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

		// --ワールド行列の合成-- //
		XMMATRIX matWorld = XMMatrixIdentity();// -> 変形のリセット
		matWorld *= matScale;// -> ワールド行列にスケーリングを反映
		matWorld *= matRot;// -> ワールド行列に回転を反映
		matWorld *= matTrans;// -> ワールド行列に平行移動を反映

		// 関数実行の成否を判別用の変数
		HRESULT result;

		// マッピング
		ObjectBuff* objectMap = nullptr;
		result = objectBuff_->Map(0, nullptr, (void**)&objectMap);
		assert(SUCCEEDED(result));

		// 定数バッファへデータ転送
		objectMap->mat = matWorld * camera_->GetMatView() * matProjection_;

		// マッピング終了処理
		objectBuff_->Unmap(0, nullptr);
	}

	// オブジェクトの描画処理
	{
		// --SRVヒープのハンドルを取得-- //
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

		// --ハンドルを指定された分まで進める-- //
		srvGpuHandle.ptr += model_->textureHandle_;

		// --定数バッファビュー（CBV）の設定コマンド-- //
		DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(0, objectBuff_->GetGPUVirtualAddress());
		DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(1, model_->materialBuff_->GetGPUVirtualAddress());

		// --指定されたSRVをルートパラメータ1番に設定-- //
		DX12Cmd::GetCmdList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

		// --頂点バッファビューの設定コマンド-- //
		DX12Cmd::GetCmdList()->IASetVertexBuffers(0, 1, &model_->vbView_);

		// --インデックスバッファビューの設定コマンド-- //
		DX12Cmd::GetCmdList()->IASetIndexBuffer(&model_->ibView_);

		//// --描画コマンド-- //
		DX12Cmd::GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(model_->indexes_.size()), 1, 0, 0, 0);
	}
}

void Object3D::PreDraw() {
	// パイプラインステートの設定
	DX12Cmd::GetCmdList()->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	// ルートシグネチャの設定
	DX12Cmd::GetCmdList()->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	// プリミティブ形状を設定
	DX12Cmd::GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --デスクリプタヒープの配列をセットするコマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DX12Cmd::GetCmdList()->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::SetPos(const XMFLOAT3& position)
{
	position_ = position;
}

void Object3D::SetRot(const XMFLOAT3& rotation)
{
	rotation_ = rotation;
}

void Object3D::SetScale(const XMFLOAT3& scale)
{
	scale_ = scale;
}

void Object3D::Initialize()
{
	// 関数実行の成否を判別用の変数
	HRESULT result;

	// オブジェクトの定数バッファを作成
	{
		// 定数バッファのヒープ設定
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		// 定数バッファのリソース設定
		D3D12_RESOURCE_DESC resdesc{};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = (sizeof(ObjectBuff) + 0xff) & ~0xff;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.SampleDesc.Count = 1;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		result = DX12Cmd::GetDevice()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&objectBuff_));
		assert(SUCCEEDED(result));
	}

	// --透視投影行列の計算-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> 上下画角45度
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);
}