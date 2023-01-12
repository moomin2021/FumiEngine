#include "Wall.h"

Wall::Wall() {
	object_ = std::make_unique<Object3D>();
	object_->scale_ = { 10.0f, 10.0f, 10.0f };
}

void Wall::Draw() {
	object_->Draw();
}