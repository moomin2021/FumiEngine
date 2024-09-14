/**
 * @file BaseScene.h
 * @brief シーンの基底クラス
 * @author moomin
 * @date 2024/03/13
 */

#pragma once

#include "IScene.h"

class BaseScene {
protected:
	// クラス所有元に情報を伝えるインターフェイス
	IScene* sceneIf_ = nullptr;

public:
	BaseScene(IScene* changer);
	virtual ~BaseScene() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void MatUpdate() = 0;
	virtual void ImGuiUpdate() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};