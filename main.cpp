#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion

#include "Sprite.h"
#include "FumiEngine.h"
#include "Object3D.h"
#include "SceneManager.h"
#include "Camera.h"


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

	int valoHandle = LoadTexture(L"Resources/valo.jpg");
	int haeHandle = LoadTexture(L"Resources/hae.png");

	// スプライト
	Sprite* sprite[2];
	sprite[0] = new Sprite();
	sprite[1] = new Sprite();

	sprite[1]->position = { 100.0f, 100.0f };

	Object3D* object[2];
	object[0] = new Object3D();
	object[1] = new Object3D();
	object[0]->CreateCube();
	object[1]->CreateModel();
	object[0]->position_.x = 2.0f;
	object[0]->rotation_.x = 45.0f;

	Camera* camera = new Camera();
	camera->eye_ = { 0.0f, 0.0f, -10.0f };

	// --コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (winAPI->IsEndMessage() == true) break;

		camera->Update();

		sprite[0]->Update();
		sprite[1]->Update();

		object[0]->Update(camera);
		object[1]->Update(camera);

		// --描画前処理-- //
		dx12->PreDraw();

		Object3D::PreDraw();
		object[0]->Draw(valoHandle);
		object[1]->Draw(valoHandle);

		// スプライト描画前処理
		Sprite::PreDraw();
		sprite[0]->Draw(valoHandle);
		sprite[1]->Draw();

		// --描画後処理-- //
		dx12->PostDraw();
	}

	// --ウィンドウクラス登録解除-- //
	winAPI->WinClassUnregister();

	delete sprite[0];
	delete sprite[1];
	delete object[0];
	delete object[1];

	return 0;
}