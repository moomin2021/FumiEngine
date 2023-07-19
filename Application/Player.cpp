#include "Player.h"
#include "Object3D.h"
#include "float3.h"
#include "Vector3.h"
#include "Util.h"
#include "Texture.h"
#include "WinAPI.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Player::Player() :
#pragma region ���������X�g
	// ���̓N���X�C���X�^���X
	key_(nullptr),
	mouse_(nullptr),

	// ���_�J����
	camera_(nullptr),

	// �e�̃��f��
	mBullet_(nullptr),

	// �e
	bullets_{},

	// �����x�N�g��
	forwardVec_{},	// �O��
	rightVec_{},	// �E

	// ���x
	moveSpd_(1.0f),			// �ړ����x
	cameraAngleSpd_(0.3f),	// �J�����̊p�x�̈ړ����x

	// �N���X�w�A
	crossHairHandle_(0),
	sCrossHair_(nullptr)
#pragma endregion
{
	// ���̓N���X�C���X�^���X�擾
	key_	= Key::GetInstance();	// �L�[�{�[�h
	mouse_	= Mouse::GetInstance();	// �}�E�X

	// �J�����𐶐����ݒ�
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 2.0f, 0.0f });

	// �J������K�p
	Object3D::SetCamera(camera_.get());

	// �e�̃��f���ǂݍ���
	mBullet_ = std::make_unique<Model>("sphere");

	// �e�Ƀ��f����ݒ�
	Bullet::SetModel(mBullet_.get());

	// ���C�̃R���C�_�[�𐶐�
	rayCollider_ = std::make_unique<RayCollider>(camera_->GetEye());
	rayCollider_->SetAttribute(COL_PLAYER_RAY);
	rayCollider_->SetIsCollision(false);

	// �R���C�_�[��ǉ�
	CollisionManager::GetInstance()->AddCollider(rayCollider_.get());

	crossHairHandle_ = LoadTexture("Resources/crossHair.png");
	sCrossHair_ = std::make_unique<Sprite>();
	sCrossHair_->SetAnchorPoint({ 0.5f, 0.5f });
	sCrossHair_->SetPosition({
		WinAPI::GetInstance()->GetWidth() / 2.0f,
		WinAPI::GetInstance()->GetHeight() / 2.0f });
	sCrossHair_->SetSize({ 26, 26 });
}

Player::~Player()
{
	CollisionManager::GetInstance()->RemoveCollider(rayCollider_.get());
}

void Player::Update()
{
	// �e��������
	Shoot();

	// ���_�ړ�
	EyeMove();

	// �ړ�
	Move();

	// �R���C�_�[�̍X�V
	ColliderUpdate();

	if (rayCollider_->GetIsHit()) {
		int num = 0;
	}

	// �J�����̍X�V
	camera_->Update();
}

void Player::Draw()
{
	for (auto& bullets : bullets_) {
		bullets->Draw();
	}
}

void Player::Draw2D()
{
	sCrossHair_->Draw(crossHairHandle_);
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

	rayCollider_->SetIsCollision(false);

	// �}�E�X�����N���b�N���Ă��Ȃ������炱�̌�̏������΂�
	if (mouse_->TriggerMouseButton(MouseButton::M_LEFT) == false) return;

	rayCollider_->SetIsCollision(true);

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

	float3 forwardMove = {
		moveVec.z * forwardVec_.x * moveSpd_,
		0.0f,
		moveVec.z * forwardVec_.z * moveSpd_
	};

	float3 rightMove = {
		moveVec.x * rightVec_.x * moveSpd_,
		0.0f,
		moveVec.x * rightVec_.z * moveSpd_
	};

	camera_->SetEye(camera_->GetEye() + (forwardMove + rightMove));

	// �J�����̒����_��ݒ�
	camera_->SetTarget(camera_->GetEye() + forwardVec_ * 10.0f);
}

void Player::ColliderUpdate()
{
	// ���C�̊J�n�ʒu��ݒ�
	rayCollider_->SetOffset(camera_->GetEye());

	// ���C�̕�����ݒ�
	rayCollider_->SetDir(forwardVec_);
}