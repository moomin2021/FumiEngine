#include "DebugCamera.h"

#include <imgui_impl_DX12.h>

void DebugCamera::Initialize(Player* inPlayer)
{
	camera_ = std::make_unique<Camera>();
	pPlayer_ = inPlayer;
}

void DebugCamera::Update()
{
	Vector3 target = pPlayer_->GetPosition();
	target_ = target;
	eye_ = { target_.x, 20.0f, target_.z - 0.1f};

	camera_->SetEye(eye_);
	camera_->SetTarget(target_);
	camera_->SetUp(up_);
}

void DebugCamera::MatUpdate()
{
	camera_->Update();
}

void DebugCamera::Debug()
{
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

	camera_->SetEye(eye_);
	camera_->SetTarget(target_);
	camera_->SetUp(up_);
	ImGui::End();
}