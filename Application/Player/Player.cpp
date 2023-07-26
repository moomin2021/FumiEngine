#include "Player.h"
#include "Object3D.h"
#include "float3.h"
#include "Vector3.h"
#include "Util.h"
#include "Texture.h"
#include "WinAPI.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Util.h"

Player::Player()
{
#pragma region ���̓N���X
	// ���̓N���X�C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X
#pragma endregion

#pragma region �J����
	// �J�����𐶐����ݒ�
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });

	// �J������K�p
	Object3D::SetCamera(camera_.get());
#pragma endregion

#pragma region �e
	// �e�̃��f���ǂݍ���
	mBullet_ = std::make_unique<Model>("sphere");

	// �e�Ƀ��f����ݒ�
	Bullet::SetModel(mBullet_.get());
#pragma endregion

#pragma region �R���C�_�[
	// ���C�̃R���C�_�[�𐶐�
	eyeCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	eyeCollider_->SetAttribute(COL_PLAYER_RAY);
	eyeCollider_->SetIsCollision(false);

	// �R���C�_�[��ǉ�
	CollisionManager::GetInstance()->AddCollider(eyeCollider_.get());
#pragma endregion

#pragma region �N���X�w�A
	crossHairHandle_ = LoadTexture("Resources/crossHair.png");
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });
#pragma endregion
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(eyeCollider_.get());
}

void Player::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();

	// �J�����̍X�V
	camera_->Update();
}

void Player::Object3DDraw()
{
	for (auto& bullets : bullets_) {
		bullets->Draw();
	}
}

void Player::FrontSpriteDraw()
{
	sCrossHair_->Draw(crossHairHandle_);
}

void (Player::* Player::stateTable[]) () = {
	&Player::Normal,// �ʏ���
	&Player::Air,	// �󒆏��
};

void Player::Normal() {
	// ���_����
	EyeMove();

	// �ړ�����
	Move();

	// ������
	Shoot();

	// �W�����v����
	Jump();

	// �R���C�_�[�̍X�V
	ColliderUpdate();
}

void Player::Air() {
	// ���_����
	EyeMove();

	// �ړ�����
	Move();

	// ������
	Shoot();

	// ��������
	Fall();

	// �R���C�_�[�̍X�V
	ColliderUpdate();
}

void Player::Shoot()
{
	for (size_t i = 0; i < bullets_.size(); i++) {
		// �e�̍X�V����
		bullets_[i]->Update();

		// �����t���O��[OFF]��������
		if (bullets_[i]->GetIsAlive() == false) {
			// �e������
			bullets_.erase(bullets_.begin() + i);
		}
	}

	eyeCollider_->SetIsCollision(false);

	// �c�e����0�ȉ��Ȃ炱�̌�̏������΂�
	if (nowBullet_ <= 0) return;

	// �}�E�X�����N���b�N���Ă��Ȃ������炱�̌�̏������΂�
	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT) == false) return;

	// �c�e�������炷
	nowBullet_--;

	eyeCollider_->SetIsCollision(true);

	// �e�𐶐�
	bullets_.emplace_back(std::make_unique<Bullet>(camera_->GetEye(), forwardVec_));
}

void Player::EyeMove()
{
	// ���_�̊p�x
	static float3 angle = { 0.0f, 90.0f, 0.0f };

	// �}�E�X�̈ړ��ʕ����_�̊p�x�ɉ��Z
	angle.x += mouse_->GetMouseVelosity().x * cameraAngleSpd_;
	angle.y += mouse_->GetMouseVelosity().y * cameraAngleSpd_;

	// ���_�ړ��̏㉺�ɐ�����t����
	angle.y = Util::Clamp(angle.y, 180.0f, 0.0f);

	// �O���x�N�g���v�Z
	forwardVec_ = {
		sinf(Util::Degree2Radian(angle.x)),
		cosf(Util::Degree2Radian(angle.y)),
		cosf(Util::Degree2Radian(angle.x))
	};

	// �O���x�N�g�����K��
	forwardVec_.normalize();

	// �E�x�N�g���̌v�Z
	rightVec_ = Vector3(forwardVec_.x, 0.0f, forwardVec_.z);
	rightVec_ = -rightVec_.cross(rightVec_ + Vector3(0.0f, 1.0f, 0.0f));

	// �E�x�N�g�����K��
	rightVec_.normalize();

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Move()
{

	// �ړ�����
	Vector3 moveVec = {
		static_cast<float>(key_->PushKey(DIK_D) - key_->PushKey(DIK_A)),
		0.0f,
		static_cast<float>(key_->PushKey(DIK_W) - key_->PushKey(DIK_S))
	};

	if (moveVec.length() > 0.0f) {
		moveSpd_ += moveAcc_;
	}

	else {
		moveSpd_ -= moveAcc_;
	}

	moveSpd_ = Util::Clamp(moveSpd_, maxSpd_, 0.0f);

	float3 forwardMove = {
		moveVec.z * forwardVec_.x,
		0.0f,
		moveVec.z * forwardVec_.z
	};

	float3 rightMove = {
		moveVec.x * rightVec_.x,
		0.0f,
		moveVec.x * rightVec_.z
	};

	Vector3 resultVec = forwardMove + rightMove;
	resultVec.normalize();

	camera_->SetEye(camera_->GetEye() + resultVec * moveSpd_);

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Jump()
{
	// [SPACE]�L�[����������W�����v����
	if (isGround_ && key_->TriggerKey(DIK_SPACE)) {
		gravity_ = -jumpSpd_;
		isGround_ = false;
		state_ = AIR;
	}
}

void Player::Fall()
{
	// �n�ʂɒ����Ă��Ȃ��Ȃ痎������������
	if (isGround_ == false) {
		gravity_ += gAcc_;// �d�͉����x�����Z
		camera_->SetEye(camera_->GetEye() + Vector3(0.0f, -1.0f, 0.0f) * gravity_);
	}

	// �J�����̍��W��2.0f�ȉ��ɂȂ�Ȃ��悤��
	float3 eye = camera_->GetEye();
	eye.y = Util::Clamp(eye.y, 1000.0f, 2.0f);

	// �J�����̍��W��2.0�ȉ��ɂȂ�����n�ʂɒ��������Ƃɂ���
	if (eye.y <= 2.0f) {
		isGround_ = true;
		state_ = NORMAL;
	}

	// �J�����̍��W�X�V
	camera_->SetEye(eye);

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::Reload()
{
	// [R]�L�[��������Ă��Ȃ������炱�̌�̏������΂�
	if (key_->TriggerKey(DIK_R) == false) return;


}

void Player::ColliderUpdate()
{
	// ���C�̊J�n�ʒu��ݒ�
	eyeCollider_->SetOffset(camera_->GetEye());

	// ���C�̕�����ݒ�
	eyeCollider_->SetDir(forwardVec_);
}

void Player::OnCollision()
{

}