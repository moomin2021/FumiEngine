#pragma once
class Slider
{
#pragma region メンバ変数
private:
#pragma endregion

#pragma region メンバ関数
public:
	Slider() {}
	~Slider();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};