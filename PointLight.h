#pragma once

class PointLight {
#pragma region 構造体
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		float3 lightPos;	// ライトの座標(XYZ)
		float pad1;// パディング
		float3 lightColor;	// ライトの色(XYZ)
		float pad2;// パディング
		float3 lightAtten;	// ライト距離減衰係数(XYZ)
		bool active;		// ライト有効フラグ
	};
#pragma endregion

#pragma region メンバ変数
private:
	// ライト座標(ワールド座標系)
	float3 lightPos_ = { 0.0f, 0.0f, 0.0f };

	// ライト色
	float3 lightColor_ = { 1.0f, 1.0f, 1.0f };

	// ライト距離減衰係数
	float3 lightAtten_ = { 1.0f, 1.0f, 1.0f };

	// 有効フラグ
	bool active_ = true;
#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// ライトの座標(XYZ)を設定
	/// </summary>
	/// <param name="lightPos"> ライトの座標(XYZ) </param>
	inline void SetLightPos(const float3& lightPos) { lightPos_ = lightPos; }

	/// <summary>
	/// ライトの色(RGB)を設定
	/// </summary>
	/// <param name="lightColor"> ライトの色(RGB) </param>
	inline void SetLightColor(const float3& lightColor) { lightColor_ = lightColor; }

	/// <summary>
	/// ライトの距離減衰係数(XYZ)を設定
	/// </summary>
	/// <param name="lightAtten"> ライトの距離減衰係数(XYZ) </param>
	inline void SetLightAtten(const float3& lightAtten) { lightAtten_ = lightAtten; }

	/// <summary>
	/// ライト有効フラグを設定
	/// </summary>
	/// <param name="active"> ライト有効フラグ </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region ゲッター関数
public:
	/// <summary>
	/// ライトの座標(XYZ)を取得
	/// </summary>
	/// <returns> ライトの座標(XYZ) </returns>
	inline const float3& GetLightPos() { return lightPos_; }

	/// <summary>
	/// ライトの色(RGB)を取得
	/// </summary>
	/// <returns> ライトの色(RGB) </returns>
	inline const float3& GetLightColor() { return lightColor_; }

	/// <summary>
	/// ライトの距離減衰係数(XYZ)を取得
	/// </summary>
	/// <returns> ライトの距離減衰係数(XYZ) </returns>
	inline const float3& GetLightAtten() { return lightAtten_; }

	/// <summary>
	/// ライト有効フラグを取得
	/// </summary>
	/// <returns> ライトの有効フラグ </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};