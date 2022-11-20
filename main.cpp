#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> キーボード入力
#pragma endregion

#include "Sprite.h"
#include "FumiEngine.h"
#include "Object3D.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Scene1.h"


// --Windowsアプリでのエントリーポイント(main関数)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// --WinAPIクラス-- //
	WinAPI* winAPI = WinAPI::GetInstance();// -> インスタンス取得
	winAPI->Initialize();// -> 初期化処理

	// --DirectX12クラス-- //
	DX12Cmd* dx12 = DX12Cmd::GetInstance();/// -> インスタンス取得
	dx12->Initialize(winAPI);// -> 初期化処理

	// --テクスチャクラス-- //
	Texture* texture = Texture::GetInstance();// -> インスタンス取得
	texture->Initialize(dx12->GetDevice());// -> 初期化処理

	// --キーボードクラス-- //
	Key* key = Key::GetInstance();
	key->Initialize(winAPI);

	// --シーンを管理するクラス-- //
	SceneManager* sceneManager = new SceneManager();
	sceneManager->ChangeScene(SceneManager::SCENE1);

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (winAPI->IsEndMessage() == true) break;

		// キーボード入力更新処理
		key->Update();

		// シーン更新処理
		sceneManager->Update();

		// --描画前処理-- //
		dx12->PreDraw();

		sceneManager->Draw();

		// --描画後処理-- //
		dx12->PostDraw();
	}

	// --ウィンドウクラス登録解除-- //
	winAPI->WinClassUnregister();

	delete sceneManager;

	return 0;
}