#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> �L�[�{�[�h����
#include "Mouse.h"// -> �}�E�X�L�[�{�[�h����
#include "Object3D.h"// -> �I�u�W�F�N�g3D�N���X
#include "PostEffect.h"
#include "PipelineManager.h"
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

	PipelineManager::GetInstance();

	std::unique_ptr<PostEffect> postEffect = std::make_unique<PostEffect>();

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;

		// �L�[�{�[�h���͍X�V����
		Key::GetInstance()->Update();

		// �}�E�X���͍X�V����
		Mouse::GetInstance()->Update();

		// �V�[���Ǘ��N���X�X�V����
		SceneManager::GetInstance()->Update();

		postEffect->PreDraw();

		SceneManager::GetInstance()->Draw();

		postEffect->PostDraw();

		// --�`��O����-- //
		DX12Cmd::GetInstance()->PreDraw();

		PipelineManager::GetInstance()->PreDraw("PostEffectTest", false);

		postEffect->Draw();

		// �V�[���Ǘ��N���X�`�揈��
		//SceneManager::GetInstance()->Draw();

		// --�`��㏈��-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	return 0;
}