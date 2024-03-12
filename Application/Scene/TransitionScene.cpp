#include "TransitionScene.h"
#include "WinAPI.h"
#include "PipelineManager.h"
#include "SceneManager.h"
#include "Texture.h"

TransitionScene::TransitionScene(IScene* sceneIf) : BaseScene(sceneIf) {}

void TransitionScene::Initialize()
{
	Vector2 winSize = {
		static_cast<float>(WinAPI::GetInstance()->GetWidth()),
		static_cast<float>(WinAPI::GetInstance()->GetHeight())
	};

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

	sLoad_ = std::make_unique<Sprite>();
	sLoad_->SetAnchorPoint({ 0.5f, 0.5f });
	sLoad_->SetSize({ 100.0f, 100.0f });
	sLoad_->SetPosition({ winSize.x - 100.0f, winSize.y - 100.0f });

	loadHandle_ = LoadTexture("Sprite/load.png");

	time_ = 0;
}

void TransitionScene::Update()
{
	static float rota = 0.0f;
	rota -= 5.0f;
	sLoad_->SetRotation(rota);

	time_++;

	camera_->Update();
	sLoad_->MatUpdate();

	if (time_ >= maxTimr_) {
		sceneIf_->ChangeScene(Scene::TITLE);
	}
}

void TransitionScene::Draw()
{
	PipelineManager::PreDraw("Sprite");
	sLoad_->Draw(loadHandle_);
}

void TransitionScene::Finalize()
{
}

void TransitionScene::Collision()
{
}

void TransitionScene::MatUpdate()
{
}
