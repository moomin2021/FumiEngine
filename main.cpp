#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> �L�[�{�[�h����
#include "Mouse.h"// -> �}�E�X�L�[�{�[�h����
#include "Object3D.h"// -> �I�u�W�F�N�g3D�N���X
#pragma endregion

#include "SceneManager.h"

// --Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// WinAPI�̏�����
	WinAPI::GetInstance()->Initialize(1280, 720);

	// DirectX12�̏�����
	DX12Cmd::GetInstance()->Initialize();

	// --�e�N�X�`���N���X-- //
	Texture::GetInstance()->Initialize();

	// �}�E�X����
	Mouse* mouse = Mouse::GetInstance();
	mouse->Initialize(WinAPI::GetInstance());

	// --�V�[���Ǘ��N���X-- //
	SceneManager* sceneM = SceneManager::GetInstance();

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;

		// �L�[�{�[�h���͍X�V����
		Key::GetInstance()->Update();

		// �}�E�X���͍X�V����
		mouse->Update();

		// �V�[���Ǘ��N���X�X�V����
		sceneM->Update();

		// --�`��O����-- //
		DX12Cmd::GetInstance()->PreDraw();

		// �V�[���Ǘ��N���X�`�揈��
		sceneM->Draw();

		// --�`��㏈��-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	return 0;
}