#include "TestPlayer.h"
#include "CollisionAttribute.h"
#include <imgui_impl_DX12.h>

TestPlayer::TestPlayer() {}

TestPlayer::~TestPlayer()
{
	colMgr_->RemoveCollider(collider_.get());
}

void TestPlayer::Initialize()
{
	// �C���X�^���X�擾
	key_ = Key::GetInstance();
	colMgr_ = CollisionManager::GetInstance();

	// ���f��
	model_ = std::make_unique<Model>("sphere");

	// �I�u�W�F�N�g
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ 0.0f, 1.0f, 0.0f });

	// �R���C�_�[
	collider_ = std::make_unique<SphereCollider>();
	collider_->SetAttribute(COL_PLAYER);
	collider_->SetObject3D(object_.get());
	colMgr_->AddCollider(collider_.get());
}

void TestPlayer::Update()
{
	// �ړ�
	static float3 pos = object_->GetPosition();

	pos.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.1f;
	pos.y += (key_->PushKey(DIK_SPACE) - key_->PushKey(DIK_LCONTROL)) * 0.1f;
	pos.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.1f;

	object_->SetPosition(pos);

	ImGui::Begin("Player");
	ImGui::Text("pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
}

void TestPlayer::Draw()
{
	ImGui::End();

	// �`�揈��
	object_->Draw();
}

void TestPlayer::OnCollision()
{
	if (collider_->GetIsHit()) {
		Vector3 reject = collider_->GetReject();
		object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		ImGui::Text("Reject = {%f, %f, %f}", reject.x, reject.y, reject.z);
	}

	else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void TestPlayer::ObjUpdate()
{
	// �I�u�W�F�N�g�X�V����
	object_->Update();
}