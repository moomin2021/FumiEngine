#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"

#include <vector>
#include <memory>

class SettingLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	std::unique_ptr<Sprite> sSettingBackground_ = nullptr;
	uint16_t gSettingBackground_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	SettingLayer(CollisionManager2D* inColMgr2D) : BaseLayer(inColMgr2D) {}
	~SettingLayer() {}

	void Initialize();
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();
#pragma endregion
};