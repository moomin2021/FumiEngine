#pragma once

class CircleShadow {
#pragma region 構想体
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Vector3 dir;				// 方向
		float3 casterPos;			// キャスター座標
		float distCasterLight;	// キャスターとライトの距離
		float3 atten;				// 距離減衰係数
		float pad3;// パディング
		float2 factorAngleCos;		// 減衰角度
		bool active;				// 有効フラグ
		float pad4;// パディング
	};
#pragma endregion

#pragma region メンバ変数
private:
	// 方向(XYZ)
	Vector3 dir_ = { 1.0f, 0.0f, 0.0f };

	// キャスター座標(XYZ)
	float3 casterPos_ = { 0.0f, 0.0f, 0.0f };

	// キャスターとライトの距離
	float distCasterLight_ = 100.0f;

	// 距離減衰係数(XYZ)
	float3 atten_ = { 0.5f, 0.6f, 0.0f };

	// 減衰角度(開始角度、終了角度)
	float2 factorAngleCos_ = { 0.2f, 0.5f };

	// 有効フラグ
	bool active_ = true;
#pragma endregion

#pragma region セッター関数
public:
	/// <summary>
	/// 方向(XYZ)を設定
	/// </summary>
	/// <param name="dir"> 方向(XYZ) </param>
	inline void SetDir(const Vector3& dir) { dir_ = Vector3Normalize(dir); }

	/// <summary>
	/// キャスター座標(XYZ)を設定
	/// </summary>
	/// <param name="casterPos"> キャスター座標(XYZ) </param>
	inline void SetCasterPos(const float3& casterPos) { casterPos_ = casterPos; }

	/// <summary>
	/// キャスターとライトの距離を設定
	/// </summary>
	/// <param name="distCasterLight"> キャスターとライトの距離 </param>
	inline void SetDistCasterLight(float distCasterLight) { distCasterLight_ = distCasterLight; }

	/// <summary>
	/// 距離減衰係数(XYZ)を設定
	/// </summary>
	/// <param name="atten"></param>
	inline void SetAtten(const float3& atten) { atten_ = atten; }

	/// <summary>
	/// 減衰角度(開始角度、終了角度)を設定
	/// </summary>
	/// <param name="factorAngle"> 減衰角度(開始角度, 終了角度) </param>
	inline void SetFactorAngle(const float2& factorAngle) {
		factorAngleCos_.x = cosf(Util::Degree2Radian(factorAngle.x));
		factorAngleCos_.y = cosf(Util::Degree2Radian(factorAngle.y));
	}

	/// <summary>
	/// 有効フラグを設定
	/// </summary>
	/// <param name="active"> 有効フラグ </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region ゲッター関数
public:
	/// <summary>
	/// 方向(XYZ)を取得
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetDir() { return dir_; }

	/// <summary>
	/// キャスターの座標(XYZ)を取得
	/// </summary>
	/// <returns> キャスターの座標(XYZ) </returns>
	inline const float3& GetCasterPos() { return casterPos_; }

	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns> キャスターとライトの距離 </returns>
	inline float GetDistCasterLight() { return distCasterLight_; }

	/// <summary>
	/// 距離減衰係数(XYZ)を取得
	/// </summary>
	/// <returns> 距離減衰係数(XYZ) </returns>
	inline const float3& GetAtten() { return atten_; }

	/// <summary>
	/// 減衰角度(開始角度、終了角度)を取得
	/// </summary>
	/// <returns> 減衰角度(開始角度、終了角度) </returns>
	inline const float2& GetFactorAngleCos() { return factorAngleCos_; }

	/// <summary>
	/// 有効フラグを取得
	/// </summary>
	/// <returns> 有効フラグ </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};