#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#pragma endregion


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

	// --�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (winAPI->IsEndMessage() == true) break;

		// --�`��O����-- //
		dx12->PreDraw();

		// --�`��㏈��-- //
		dx12->PostDraw();
	}

	// --�E�B���h�E�N���X�o�^����-- //
	winAPI->WinClassUnregister();

	return 0;
}