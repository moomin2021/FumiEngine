#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> �L�[�{�[�h����
#pragma endregion

#include "Sprite.h"
#include "FumiEngine.h"
#include "Object3D.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Scene1.h"


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

	// --�L�[�{�[�h�N���X-- //
	Key* key = Key::GetInstance();
	key->Initialize(winAPI);

	// --�V�[�����Ǘ�����N���X-- //
	SceneManager* sceneManager = new SceneManager();
	sceneManager->ChangeScene(SceneManager::SCENE1);

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (winAPI->IsEndMessage() == true) break;

		// �L�[�{�[�h���͍X�V����
		key->Update();

		// �V�[���X�V����
		sceneManager->Update();

		// --�`��O����-- //
		dx12->PreDraw();

		sceneManager->Draw();

		// --�`��㏈��-- //
		dx12->PostDraw();
	}

	// --�E�B���h�E�N���X�o�^����-- //
	winAPI->WinClassUnregister();

	delete sceneManager;

	return 0;
}