#include "Matrix4.h"
#include "float4.h"
#include "float3.h"
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"

class Object3D {
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Matrix4 viewProj;	// ビュープロジェクション
		Matrix4 world;		// ワールド行列
		float3 cameraPos;	// カメラ座標(ワールド座標)
		float pad1;			// パディング
		float4 color;		// 色(RGBA)
	};

#pragma region メンバ変数
private:
	// オブジェクトデータ
	float3 position_;	// 位置(XYZ)
	float3 rotation_;	// 回転(XYZ)
	float3 scale_;		// 拡縮(XYZ)
	float4 color_;		// 色(RGBA)

	// オブジェクトデータを変更したかどうか
	bool hasChanget_;

	// ワールド行列
	Matrix4 matWorld_;

	// 定数バッファ
	ComPtr<ID3D12Resource>	constBuff_;	// 定数バッファ
	ConstBufferData*		constMap_;	// マッピング処理用

	// モデル
	Model* model_;

	// 静的メンバ変数
	static Camera*		sCamera_;		// カメラ
	static LightGroup*	sLightGroup_;	// ライト
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3D(Model* model);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

#pragma region セッター関数
	/// <summary>
	/// 座標(XYZ)を設定
	/// </summary>
	/// <param name="position"> 座標(XYZ) </param>
	inline void SetPosition(const float3& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// 回転(XYZ)を設定
	/// </summary>
	/// <param name="rotation"> 回転(XYZ) </param>
	inline void SetRotation(const float3& rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// 拡縮(XYZ)を設定
	/// </summary>
	/// <param name="scale"> 拡縮(XYZ) </param>
	inline void SetScale(const float3& scale) { scale_ = scale, hasChanget_ = true; }

	/// <summary>
	/// 色(RGBA)を設定
	/// </summary>
	/// <param name="color"> 色(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }

	/// <summary>
	/// モデルを設定
	/// </summary>
	/// <param name="model"> モデル </param>
	inline void SetModel(Model* model) { model_ = model; }

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="camera"> カメラ </param>
	static inline void SetCamera(Camera* camera) { sCamera_ = camera; }

	/// <summary>
	/// ライトグループを設定
	/// </summary>
	/// <param name="lightGroup"> ライトグループ </param>
	static inline void SetLightGroup(LightGroup* lightGroup) { sLightGroup_ = lightGroup; }
#pragma endregion

#pragma region ゲッター関数
	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns> ワールド行列 </returns>
	inline const Matrix4& GetMatWorld() { return matWorld_; }
#pragma endregion

private:
	/// <summary>
	/// オブジェクトデータの更新
	/// </summary>
	void UpdateData();
#pragma endregion
};