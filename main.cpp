#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> キーボード入力
#include "Mouse.h"// -> マウスキーボード入力
#include "Object3D.h"// -> オブジェクト3Dクラス
#pragma endregion

#include "SceneManager.h"

// --Windowsアプリでのエントリーポイント(main関数)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// WinAPIの初期化
	WinAPI::GetInstance()->Initialize(1280, 720);

	// DirectX12の初期化
	DX12Cmd::GetInstance()->Initialize();

	// --テクスチャクラス-- //
	Texture::GetInstance()->Initialize();

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;

		// キーボード入力更新処理
		Key::GetInstance()->Update();

		// マウス入力更新処理
		Mouse::GetInstance()->Update();

		// シーン管理クラス更新処理
		SceneManager::GetInstance()->Update();

		// --描画前処理-- //
		DX12Cmd::GetInstance()->PreDraw();

		// シーン管理クラス描画処理
		SceneManager::GetInstance()->Draw();

		// --描画後処理-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	return 0;
}