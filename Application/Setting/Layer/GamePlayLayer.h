#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"
#include "Slider.h"

#include <vector>
#include <memory>

class GamePlayLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};
	std::vector<std::unique_ptr<Slider>> sliders_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	GamePlayLayer(CollisionManager2D* inColMgr2D) : BaseLayer(inColMgr2D) {}
	~GamePlayLayer() {}

	void Initialize();
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();
#pragma endregion
};