#pragma once

/**
 * @file BasicTask.h
 * @brief 基本的な処理の純粋仮想関数が宣言されているクラス
 * @author moomin
 * @date 2024/03/13
 */

class BasicTask {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Collision() = 0;
	virtual void MatUpdate() = 0;
	virtual void Finalize() = 0;
};