#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> キーボード入力
#include "Mouse.h"// -> マウスキーボード入力
#include "Pad.h"
#include "Object3D.h"// -> オブジェクト3Dクラス
#include "Sound.h"
#include "ImGuiManager.h"
#include "DeltaTime.h"
#pragma endregion

#include "SceneManager.h"
#include "PipelineManager.h"

#include <memory>

// --Windowsアプリでのエントリーポイント(main関数)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// WinAPIの初期化
	WinAPI::GetInstance()->Initialize(1920, 1080);

	// DirectX12の初期化
	DX12Cmd::GetInstance()->Initialize();

	// --テクスチャクラス-- //
	Texture::GetInstance()->Initialize();

	// サウンドクラス
	Sound::GetInstance()->Initialize();

	// ImGuiクラス
	ImGuiManager::GetInstance()->Initialize();

	DeltaTime::GetInstance()->Initialize();

	PipelineManager::GetInstance();

	// シーン管理クラスの生成と初期化
	std::unique_ptr<SceneManager> sceneMgr = std::make_unique<SceneManager>();
	sceneMgr->Initialize();

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;
		//if (Key::GetInstance()->PushKey(DIK_ESCAPE) == true) break;
		if (sceneMgr->GetIsGameEnd()) break;

		DeltaTime::GetInstance()->Update();

		// キーボード入力更新処理
		Key::GetInstance()->Update();

		// マウス入力更新処理
		Mouse::GetInstance()->Update();

		// パッド入力更新
		Pad::GetInstance()->Update();

		// サウンド更新処理
		Sound::GetInstance()->Update();

		// シーン管理クラス更新処理
		sceneMgr->Update();

		// シーン管理クラス描画処理
		sceneMgr->Draw();
	}

	return 0;
}
