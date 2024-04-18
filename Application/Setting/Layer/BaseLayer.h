#pragma once
#include "CollisionManager2D.h"

class BaseLayer
{
#pragma region メンバ変数
protected:
	CollisionManager2D* pColMgr2D_ = nullptr;
	float alpha_ = 1.0f;
#pragma endregion

#pragma region メンバ関数
public:
	BaseLayer(CollisionManager2D* inColMgr2D) : pColMgr2D_(inColMgr2D) {}
	virtual ~BaseLayer() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Collision() = 0;
	virtual void MatUpdate() = 0;
	virtual void Finalize() = 0;
#pragma endregion

#pragma region セッター関数
public:
	void SetAlpha(float inAlpha) { alpha_ = inAlpha; }
#pragma endregion

#pragma region ゲッター関数
	float GetAlpha() { return alpha_; }
#pragma endregion
};