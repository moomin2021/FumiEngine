#include "ItemManager.h"
#include "Util.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

ItemManager::ItemManager() {}

ItemManager* ItemManager::GetInstace()
{
	static ItemManager inst;
	return &inst;
}

ItemManager::~ItemManager()
{
	for (auto& i : items_) CollisionManager::GetInstance()->RemoveCollider(i.col.get());
	items_.clear();
}

void ItemManager::Initialize()
{
#pragma region モデル読み込み
	mItems_.resize(2);
	mItems_[0] = std::make_unique<Model>("cube");
	mItems_[1] = std::make_unique<Model>("cube");
#pragma endregion
}

void ItemManager::MatUpdate()
{
	for (auto& i : items_) {
		i.obj->MatUpdate();
	}
}

void ItemManager::Draw()
{
	for (auto& i : items_) {
		i.obj->Draw();
	}
}

void ItemManager::AddItem(const Vector3& pos)
{
	// アイテム生成
	items_.emplace_back();

	Item& itr = items_.back();

	// アイテム番号をランダムで決定
	items_.back().itemNum = static_cast<uint8_t>(Util::GetRandomInt(0, 1));

	// オブジェクト生成
	items_.back().obj = std::make_unique<Object3D>(mItems_[items_.back().itemNum].get());
	items_.back().obj->SetPosition(pos);

	// コライダー生成
	items_.back().col = std::make_unique<SphereCollider>();
	items_.back().col->SetObject3D(items_.back().obj.get());
	items_.back().col->SetAttribute(COL_ITEM);
	CollisionManager::GetInstance()->AddCollider(items_.back().col.get());
}

void ItemManager::DeleteItem(SphereCollider* collider)
{
	for (auto it = items_.begin(); it != items_.end();) {
		if (it->col.get() == collider) {
			CollisionManager::GetInstance()->RemoveCollider(it->col.get());
			it = items_.erase(it);
		}

		else {
			++it;
		}
	}
}