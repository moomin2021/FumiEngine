#pragma region 自クラス読み込み(含める)
#include "FMWinAPI.h"// -> WinAPI
#include "FMDX12Cmd.h"// -> DirectX12
#pragma endregion


// --Windowsアプリでのエントリーポイント(main関数)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// --WinAPIクラス-- //
	WinAPI* winAPI = WinAPI::GetInstance();// -> インスタンス取得
	winAPI->Initialize();// -> 初期化処理

	// --DirectX12クラス-- //
	DX12Cmd* dx12 = DX12Cmd::GetInstance();/// -> インスタンス取得
	dx12->Initialize(winAPI);// -> 初期化処理

	// --コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (winAPI->IsEndMessage() == true) break;


	}

	// --ウィンドウクラス登録解除-- //
	winAPI->WinClassUnregister();

#pragma region メモリ解放
	winAPI->Release();// -> WinAPI
#pragma endregion

	return 0;
}