#include "CameraManager.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Line3D.h"
#include "Instancing3D.h"
#include "ParticleEmitter.h"

void CameraManager::Initialize(Camera* inCamera)
{
	// カメラを登録
	nowCamera_ = inCamera;

	// カメラを設定
	Object3D::SetCamera(nowCamera_);
	Sprite::SetCamera(nowCamera_);
	Line3D::SetCamera(nowCamera_);
	Instancing3D::SetCamera(nowCamera_);
	ParticleEmitter::SetCamera(nowCamera_);
}

void CameraManager::MatUpdate()
{
	nowCamera_->Update();
}

void CameraManager::ChangeCamera(Camera* inCamera)
{
	// カメラを登録
	nowCamera_ = inCamera;

	// カメラを設定
	Object3D::SetCamera(nowCamera_);
	Sprite::SetCamera(nowCamera_);
	Line3D::SetCamera(nowCamera_);
	Instancing3D::SetCamera(nowCamera_);
	ParticleEmitter::SetCamera(nowCamera_);
}