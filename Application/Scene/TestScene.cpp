#include "TestScene.h"

TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::Initialize()
{
#pragma region �J����
	camera_ = std::make_unique<Camera>();
#pragma endregion
}

void TestScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void TestScene::Draw()
{
}

void TestScene::OnCollision()
{
}

void TestScene::MatUpdate()
{
	// �J����
	camera_->Update();
}