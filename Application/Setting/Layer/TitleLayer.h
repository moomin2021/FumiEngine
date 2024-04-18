#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"

#include <vector>
#include <memory>

class TitleLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};

	std::unique_ptr<Sprite> titleS_ = nullptr;
	uint16_t titleH_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	TitleLayer(CollisionManager2D* inColMgr2D) : BaseLayer(inColMgr2D) {}
	~TitleLayer() {}

	void Initialize();
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();
#pragma endregion
};