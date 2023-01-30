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
	ComPtr<ID3D12Resource> objectBuff_;// ---> オブジェクト

	// カメラ
	Camera* camera_;

	// モデル
	Model* model_;

public:// メンバ関数
	// [Object3D]インスタンス作成
	static Object3D* CreateObject3D();

	// 描画処理
	void Draw();

	// 描画前処理
	static void PreDraw();

#pragma region セッター
	// カメラを設定
	void SetCamera(Camera* camera) { camera_ = camera; }

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

	// 初期化処理
	void Initialize();
};