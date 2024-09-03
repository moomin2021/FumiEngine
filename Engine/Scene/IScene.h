/**
 * @file BaseScene.h
 * @brief シーン管理クラスとシーンを繋ぐインターフェンスクラス
 * @author moomin
 * @date 2024/03/13
 */

#pragma once

enum class Scene {
	TITLE,
	GAME,

	NONE,
};

class IScene
{
public:
	virtual ~IScene() = 0;

	// 指定のシーンに変更する
	virtual void ChangeScene(const Scene& nextScene) = 0;

	// ゲームを終了させる
	virtual void GameEnd() = 0;

	virtual bool GetIsGameEnd() = 0;
};