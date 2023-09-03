#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"

#include <vector>
#include <memory>

enum ItemNumber {
	ExtendedMagazine,// 拡張マガジン
	Syringe,// 注射器
};

struct Item {
	// オブジェクト
	std::unique_ptr<Object3D> obj = nullptr;

	// コライダー
	std::unique_ptr<SphereCollider> col = nullptr;

	// アイテム番号
	uint8_t itemNum = 0;
};

class ItemManager {
#pragma region メンバ変数
private:
	// 各アイテムのモデル
	std::vector<std::unique_ptr<Model>> mItems_;

	// 各アイテムのオブジェクト
	std::vector<Item> items_;
#pragma endregion

#pragma region メンバ関数
public:
	static ItemManager* GetInstace();

	// デストラクタ
	~ItemManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// アイテム追加
	void AddItem(const Vector3& pos);

	void DeleteItem(SphereCollider* collider);

private:
	// コンストラクタ
	ItemManager();
#pragma endregion
};