#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion

#include "Sprite.h"


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

	int valoHandle = Texture::LoadTexture(L"Resources/valo.jpg");
	int haeHandle = Texture::LoadTexture(L"Resources/hae.png");

	// スプライト
	Sprite* sprite[2];
	sprite[0] = new Sprite();
	sprite[1] = new Sprite();

	sprite[0]->Initialize();

	sprite[1]->position = { 100.0f, 100.0f, 0.0f };
	sprite[1]->Initialize();

	// --コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (winAPI->IsEndMessage() == true) break;

		sprite[0]->Update();
		sprite[1]->Update();

		// --描画前処理-- //
		dx12->PreDraw();

		// スプライト描画前処理
		Sprite::PreDraw();
		sprite[0]->Draw(valoHandle);
		sprite[1]->Draw(haeHandle);

		// --描画後処理-- //
		dx12->PostDraw();
	}

	// --ウィンドウクラス登録解除-- //
	winAPI->WinClassUnregister();

	delete sprite[0];
	delete sprite[1];

	return 0;
}