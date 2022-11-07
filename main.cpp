#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion


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

	// --コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (winAPI->IsEndMessage() == true) break;

		// --描画前処理-- //
		dx12->PreDraw();

		// --描画後処理-- //
		dx12->PostDraw();
	}

	// --ウィンドウクラス登録解除-- //
	winAPI->WinClassUnregister();

	return 0;
}