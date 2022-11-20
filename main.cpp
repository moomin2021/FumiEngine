#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion

#include "Sprite.h"
#include "FumiEngine.h"
#include "Object3D.h"
#include "SceneManager.h"
#include "Camera.h"


// --Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// --WinAPI�N���X-- //
	WinAPI* winAPI = WinAPI::GetInstance();// -> �C���X�^���X�擾
	winAPI->Initialize();// -> ����������

	// --DirectX12�N���X-- //
	DX12Cmd* dx12 = DX12Cmd::GetInstance();/// -> �C���X�^���X�擾
	dx12->Initialize(winAPI);// -> ����������

	// --�e�N�X�`���N���X-- //
	Texture* texture = Texture::GetInstance();// -> �C���X�^���X�擾
	texture->Initialize(dx12->GetDevice());// -> ����������

	int valoHandle = LoadTexture(L"Resources/valo.jpg");
	int haeHandle = LoadTexture(L"Resources/hae.png");

	// �X�v���C�g
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

	// --�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (winAPI->IsEndMessage() == true) break;

		camera->Update();

		sprite[0]->Update();
		sprite[1]->Update();

		object[0]->Update(camera);
		object[1]->Update(camera);

		// --�`��O����-- //
		dx12->PreDraw();

		Object3D::PreDraw();
		object[0]->Draw(valoHandle);
		object[1]->Draw(valoHandle);

		// �X�v���C�g�`��O����
		Sprite::PreDraw();
		sprite[0]->Draw(valoHandle);
		sprite[1]->Draw();

		// --�`��㏈��-- //
		dx12->PostDraw();
	}

	// --�E�B���h�E�N���X�o�^����-- //
	winAPI->WinClassUnregister();

	delete sprite[0];
	delete sprite[1];
	delete object[0];
	delete object[1];

	return 0;
}