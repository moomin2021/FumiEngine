#pragma once
#include "Object3D.h"

class TouchableObject : public Object3D {
#pragma region メンバ変数

#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TouchableObject(Model* model);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
#pragma endregion
};