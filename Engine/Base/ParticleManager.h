#pragma once
#include "Camera.h"
#include "Matrix4.h"
#include "float4.h"
#include "float2.h"

#include <wrl.h>
#include <d3d12.h>
#include <vector>
#include <forward_list>

enum BILLBOARD {
	NONE,
	ALL,
	X,
	Y,
	Z
};

class ParticleManager
{
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点データ
	struct Vertex {
		float3 pos;
		float scale;
	};

	// 定数バッファ用
	struct ConstBufferData {
		Matrix4 mat;
	};

	// パーティクル1粒
	struct Particle {
		float3 position = {};

		// 速度
		float3 velocity = {};

		// 加速度
		float3 accel = {};

		// 現在フレーム
		uint16_t frame = 0;

		// 終了フレーム
		uint16_t num_frame = 0;

		// スケール
		float scale = 1.0f;

		// 初期値
		float startScale = 1.0f;

		// 最終値
		float endScale = 0.0f;
	};

#pragma region メンバ変数
private:
	// パーティクル配列
	std::forward_list<Particle> particles_;

	// 最大頂点数
	const uint16_t vertexCount_ = 1024;

	// オブジェクトデータ
	float3 position_ = { 0.0f, 0.0f, 0.0f };
	float3 rotation_ = { 0.0f, 0.0f, 0.0f };
	float3 scale_ = { 1.0f, 1.0f, 1.0f };
	float4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 頂点データ
	std::vector<Vertex> vertices_;			// 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// 頂点バッファービュー
	ComPtr<ID3D12Resource> vertexBuff_;		// 頂点バッファ

	// インデックスデータ
	std::vector<uint16_t> indexes_;		// インデックスデータ
	D3D12_INDEX_BUFFER_VIEW indexView_;	// インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;	// インデックスバッファ

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	ConstBufferData* constMap_;

	bool isAlive_ = true;

	// カメラ
	static Camera* sCamera_;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	ParticleManager();

	// デストラクタ
	~ParticleManager();

	// 更新処理
	void Update(BILLBOARD billBoard = BILLBOARD::NONE);

	// 描画処理
	void Draw(uint16_t handle = 0);

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="life"> 生存時間 </param>
	/// <param name="pos"> 初期座標 </param>
	/// <param name="velocity"> 速度 </param>
	/// <param name="accel"> 加速度 </param>
	void Add(uint16_t life, float3 pos, float3 velocity, float3 accel, float startScale, float endScale);

private:
	// 頂点バッファ作成
	void CreateVertexBuff();

	// インデックスバッファ作成
	void CreateIndexBuff();
#pragma endregion

#pragma region セッター
public:
	// カメラを設定
	static void SetCamera(Camera* camera) { sCamera_ = camera; }

	void SetPos(const float3& pos) { position_ = pos; }
#pragma endregion

	bool GetIsAlive() { return isAlive_; }
};