#pragma once
#include "BaseLayer.h"
#include "BoxButton.h"

#include <vector>
#include <memory>

class PauseLayer : public BaseLayer
{
#pragma region メンバ変数
private:
	std::vector<std::unique_ptr<BoxButton>> buttons_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	PauseLayer(CollisionManager2D* inColMgr2D) : BaseLayer(inColMgr2D) {}
	~PauseLayer() {}

	void Initialize();
	void Update();
	void Draw();
	void Collision();
	void MatUpdate();
	void Finalize();
#pragma endregion
};