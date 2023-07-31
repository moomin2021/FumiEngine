#pragma once
#include "Matrix4.h"
#include "float4.h"

#include <wrl.h>

class ParticleManager
{
#pragma region メンバ変数
private:

#pragma endregion

#pragma region メンバ関数
public:
	// インスタンスを取得
	ParticleManager* GetInstance();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// コンストラクタ
	ParticleManager();
#pragma endregion
};