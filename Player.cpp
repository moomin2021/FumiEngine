#include "Player.h"

Player::Player() :
#pragma region ���������X�g
	// ���̓N���X�C���X�^���X
	key_(nullptr),
	mouse_(nullptr)
#pragma endregion
{
	// ���̓N���X�C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X
}

void Player::Update()
{
}

void Player::Draw()
{
}
