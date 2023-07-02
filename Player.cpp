#include "Player.h"

Player::Player() :
#pragma region 初期化リスト
	// 入力クラスインスタンス
	key_(nullptr),
	mouse_(nullptr)
#pragma endregion
{
	// 入力クラスインスタンス取得
	key_	= Key::GetInstance();	// キーボード
	mouse_	= Mouse::GetInstance();	// マウス
}

void Player::Update()
{
}

void Player::Draw()
{
}
