#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion

#include "Sprite.h"


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

	int valoHandle = Texture::LoadTexture(L"Resources/valo.jpg");
	int haeHandle = Texture::LoadTexture(L"Resources/hae.png");

	// �X�v���C�g
	Sprite* sprite[2];
	sprite[0] = new Sprite();
	sprite[1] = new Sprite();

	sprite[0]->Initialize();

	sprite[1]->position = { 100.0f, 100.0f, 0.0f };
	sprite[1]->Initialize();

	// --�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (winAPI->IsEndMessage() == true) break;

		sprite[0]->Update();
		sprite[1]->Update();

		// --�`��O����-- //
		dx12->PreDraw();

		// �X�v���C�g�`��O����
		Sprite::PreDraw();
		sprite[0]->Draw(valoHandle);
		sprite[1]->Draw(haeHandle);

		// --�`��㏈��-- //
		dx12->PostDraw();
	}

	// --�E�B���h�E�N���X�o�^����-- //
	winAPI->WinClassUnregister();

	delete sprite[0];
	delete sprite[1];

	return 0;
}