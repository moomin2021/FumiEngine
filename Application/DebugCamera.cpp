#include "DebugCamera.h"

#include <imgui_impl_DX12.h>

void DebugCamera::Initialize(Player* inPlayer)
{
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();
	camera_ = std::make_unique<Camera>();
	pPlayer_ = inPlayer;
}

void DebugCamera::Update()
{
	Move();
	Zoom();

	camera_->SetEye(eye_);
	camera_->SetTarget(target_);
	camera_->SetUp(up_);
}

void DebugCamera::MatUpdate()
{
	camera_->Update();
}

void DebugCamera::Debug(bool isDebug)
{
	if (isDebug == false) return;

	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat("eyeX", &eye_.x, -100.0f, 100.0f);
	ImGui::SliderFloat("eyeY", &eye_.y, -100.0f, 100.0f);
	ImGui::SliderFloat("eyeZ", &eye_.z, -100.0f, 100.0f);

	ImGui::SliderFloat("targetX", &target_.x, -100.0f, 100.0f);
	ImGui::SliderFloat("targetY", &target_.y, -100.0f, 100.0f);
	ImGui::SliderFloat("targetZ", &target_.z, -100.0f, 100.0f);

	ImGui::SliderFloat("upX", &up_.x, -1.0f, 1.0f);
	ImGui::SliderFloat("upY", &up_.y, -1.0f, 1.0f);
	ImGui::SliderFloat("upZ", &up_.z, -1.0f, 1.0f);

	ImGui::Text("mouse = %d", mouse_->GetMouseWheel());

	camera_->SetEye(eye_);
	camera_->SetTarget(target_);
	camera_->SetUp(up_);
	ImGui::End();
}

void DebugCamera::Move()
{
	eye_.z += (key_->PushKey(DIK_S) - key_->PushKey(DIK_W)) * moveSpd_;
	eye_.x += (key_->PushKey(DIK_A) - key_->PushKey(DIK_D)) * moveSpd_;

	target_.z = eye_.z - 0.1f;
	target_.x = eye_.x;
}

void DebugCamera::Zoom()
{
	eye_.y += (float)mouse_->GetMouseWheel() * -0.01f;
	eye_.y = Util::Clamp(eye_.y, maxEyeY_, minEyeY_);
}