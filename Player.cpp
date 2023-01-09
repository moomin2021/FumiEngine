#include "Player.h"
#include "Util.h"
#include "FumiEngine.h"
#include "WinAPI.h"

Player::Player() :
#pragma region ���������X�g
	col_{},// -> �����蔻��p�f�[�^

	key_(nullptr),// -> �L�[�{�[�h����
	mouse_(nullptr),// -> �}�E�X
	camera_(nullptr),// -> �J����

	// �J�����A���O��
	angleX_(0.0f), angleY_(0.0f),

	cameraSens_(0.0f),// -> �J�������x

	moveSpeed_(0.0f),// -> �ړ����x

	forwardVec_{0.0f, 0.0f, 0.0f},// -> �O���x�N�g��
	rightVec_{0.0f, 0.0f, 0.0f}// -> �E�x�N�g��
#pragma endregion
{}

Player::~Player()
{
	delete reticle_;
}

void Player::Initialize()
{
	col_ = { {-10.0f, 0.0f }, 1.0f };// -> �����蔻��p�f�[�^
	oldCol_ = { {0.0f, 0.0f}, 1.0f };

	key_ = Key::GetInstance();// -> �L�[�{�[�h���̓C���X�^���X�擾
	mouse_ = Mouse::GetInstance();// -> �}�E�X���̓C���X�^���X�擾

	// �J�����A���O��
	angleX_ = 0.0f;
	angleY_ = 90.0f;

	cameraSens_ = 0.05f;// -> �J�������x

	moveSpeed_ = 0.4f;// -> �ړ����x

	reticleH_ = LoadTexture("Resources/reticle.png");
	reticle_ = new Sprite();
	reticle_->position.x = WinAPI::GetWidth() / 2.0f;
	reticle_->position.y = WinAPI::GetHeight() / 2.0f;
}

void Player::Update()
{
	oldCol_ = col_;

	// ���_�ړ�����
	EyeMove();

	Move();

	col_.p.x = camera_->eye_.x;
	col_.p.y = camera_->eye_.z;

	reticle_->Update();
}

void Player::Draw()
{
	Sprite::PreDraw();
	reticle_->Draw(reticleH_);
}

void Player::EyeMove()
{
	angleX_ += mouse_->GetMouseVelosity().x * cameraSens_;
	angleY_ += mouse_->GetMouseVelosity().y * cameraSens_;

	// �㉺�̉�]�𐧌䂷��
	angleY_ = Util::Clamp(angleY_, 180.0f, 0.0f);

	// �O���x�N�g���̌v�Z
	forwardVec_.x = sinf(Util::Degree2Radian(angleX_));
	forwardVec_.y = cosf(Util::Degree2Radian(angleY_));
	forwardVec_.z = cosf(Util::Degree2Radian(angleX_));

	// �O���x�N�g���̐��K��
	forwardVec_.normalize();

	// �E�x�N�g���̌v�Z
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// �E�x�N�g���̐��K��
	rightVec_.normalize();

	// --�����_�X�V����-- //
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.y = camera_->eye_.y + cosf(Util::Degree2Radian(angleY_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}

void Player::Move()
{
	Vector3 forwardMove{};
	Vector3 rightMove{};

	// --�X�e�B�b�N�̓��͕����ňړ�����-- //
	forwardMove.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * forwardVec_.z * moveSpeed_;
	forwardMove.x += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * forwardVec_.x * moveSpeed_;
	rightMove.z += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * rightVec_.z * moveSpeed_;
	rightMove.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * rightVec_.x * moveSpeed_;

	camera_->eye_.x += forwardMove.x + rightMove.x;
	camera_->eye_.z += forwardMove.z + rightMove.z;
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}

void Player::Target() {
	camera_->target_.x = camera_->eye_.x + sinf(Util::Degree2Radian(angleX_)) * 10.0f;
	camera_->target_.z = camera_->eye_.z + cosf(Util::Degree2Radian(angleX_)) * 10.0f;
}
