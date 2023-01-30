#pragma once
// Direct3D 12 用
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// 数学関数
#include <DirectXMath.h>
using namespace DirectX;

//　ComPtr用
#include <wrl.h>
using namespace Microsoft::WRL;

#include "fMath.h"

// シーケンスコンテナの一種
#include <vector>

// カメラクラス
#include "Camera.h"

// モデルクラス
#include "Model.h"

// 定数バッファ構造体(オブジェクト)
struct ObjectBuff {
	// 行列
	Matrix4 mat;
};

class Object3D {
private:// メンバ変数
	Float3 position_;// -> 座標
	Float3 rotation_;// -> 回転角
	Float3 scale_;// ----> スケール

	// 定数バッファ
	static D3D12_HEAP_PROPERTIES heapProp_;
	static D3D12_RESOURCE_DESC resdesc_;
	ComPtr<ID3D12Resource> constBuff_;

	// ワールド座標
	Matrix4 matWorld_;

	// カメラ
	static Camera* camera_;

	// モデル
	Model* model_;

	static ID3D12Device* device_;// -> デバイス
	static ID3D12GraphicsCommandList* cmdList_;// -> コマンドリスト

	// ダーティフラグ
	bool dirty;

public:// メンバ関数
	// [Object3D]インスタンス作成
	static Object3D* CreateObject3D();

	// オブジェクト3D全体の初期化
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

	// 描画処理
	void Draw();

	// 描画前処理
	static void PreDraw();

	// カメラセット
	static inline void SetCamera(Camera* camera) { camera_ = camera; }

#pragma region セッター
	// モデルを設定
	void SetModel(Model* model) { model_ = model; };

	// 各要素変更
	void SetPos(const Float3& position);
	void SetRot(const Float3& rotation);
	void SetScale(const Float3& scale);
#pragma endregion

#pragma region ゲッター
	// 各要素取得
	inline const Float3& GetPos() { return position_; }
	inline const Float3& GetRot() { return rotation_; }
	inline const Float3& GetScale() { return scale_; }
#pragma endregion

private:// メンバ関数
	// コンストラクタ
	Object3D();

	// 定数バッファ生成
	void GenerateConstBuffer();

	// 定数バッファ更新
	void TransferConstBuffer();
};