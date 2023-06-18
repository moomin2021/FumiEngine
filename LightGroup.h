#pragma once
#include <DirectXMath.h>
#include "DX12Cmd.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup {
private:// エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:// 定数
	static const uint16_t DirLightNum = 3;
	static const uint16_t PointLightNum = 3;
	static const uint16_t SpotLightNum = 3;
	static const uint16_t CircleShadowNum = 1;

public:// サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		// 環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		// 点光源用
		PointLight::ConstBufferData pointLights[PointLightNum];
		// スポットライト用
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		// 丸影用
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

private:// メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 環境光の色
	XMFLOAT3 ambientColor = { 1.0f, 1.0f, 1.0f };
	// 平行光源の配列
	DirectionalLight dirLights[DirLightNum];
	// 点光源の配列
	PointLight pointLights[PointLightNum];
	// スポットライト用
	SpotLight spotLights[SpotLightNum];
	// 丸影の配列
	CircleShadow circleShadows[CircleShadowNum];
	// ダーティフラグ
	bool dirty = false;

public:// 静的メンバ関数
	LightGroup();

private:// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();

public:// メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="color"> ライト色 </param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index"> ライト番号 </param>
	/// <param name="active"> 有効フラグ </param>
	void SetDirLightActive(uint16_t index, bool active);

	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index"> ライト番号 </param>
	/// <param name="lightdir"> ライト方向 </param>
	void SetDirLightDir(uint16_t index, const XMVECTOR& lightdir);

	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="index"> ライト番号 </param>
	/// <param name="lightcolor"> ライト色 </param>
	void SetDirLightColor(uint16_t index, const XMFLOAT3& lightcolor);

	void SetPointLightActive(uint16_t index, bool active);
	void SetPointLightPos(uint16_t index, const XMFLOAT3& lightpos);
	void SetPointLightColor(uint16_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(uint16_t index, const XMFLOAT3& lightAtten);

	void SetSpotLightActive(uint16_t index, bool active);
	void SetSpotLightDir(uint16_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(uint16_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(uint16_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(uint16_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(uint16_t index, const XMFLOAT2& lightFactorAngle);

	void SetCircleShadowActive(uint16_t index, bool active);
	void SetCircleShadowCasterPos(uint16_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(uint16_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCasterLight(uint16_t index, float distanceCasterLight);
	void SetCircleShadowAtten(uint16_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(uint16_t index, const XMFLOAT2& lightFactorAngle);
};