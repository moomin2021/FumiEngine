#include "Door.h"
#include "Easing.h"

Door::Door() :
#pragma region 初期化リスト
	object_(nullptr),

	col2D_{},
	col3D_{},

	isRight_(false),
	isMove_(false),

	timer_(0),
	maxTime_(0)

#pragma endregion
{
	
}

Door::~Door() {
	//delete object_;
}

void Door::Initialize()
{
	object_ = std::make_unique<Object3D>();

	object_->scale_ = { 10.0f, 10.0f, 10.0f };

	maxTime_ = 120;
}

void Door::Update()
{
	//object_->Update();

	if (object_->rotation_.y >= 90.0f) {
		// 2D用
		col2D_.sP.x = object_->position_.x;
		col2D_.sP.y = object_->position_.z + 2.5f;
		col2D_.eP.x = object_->position_.x;
		col2D_.eP.y = object_->position_.z - 2.5f;

		// 3D用
		col3D_.p[0] = { object_->position_.x, object_->position_.y + 5.0f, object_->position_.z + 2.5f };
		col3D_.p[1] = { object_->position_.x, object_->position_.y - 5.0f, object_->position_.z + 2.5f };
		col3D_.p[2] = { object_->position_.x, object_->position_.y - 5.0f, object_->position_.z - 2.5f };
		col3D_.p[3] = { object_->position_.x, object_->position_.y + 5.0f, object_->position_.z - 2.5f };
	}
	else {
		// 2D用
		col2D_.sP.x = object_->position_.x + 2.5f;
		col2D_.sP.y = object_->position_.z;
		col2D_.eP.x = object_->position_.x - 2.5f;
		col2D_.eP.y = object_->position_.z;

		// 3D用
		col3D_.p[0] = { object_->position_.x + 2.5f, object_->position_.y + 5.0f, object_->position_.z };
		col3D_.p[1] = { object_->position_.x + 2.5f, object_->position_.y - 5.0f, object_->position_.z };
		col3D_.p[2] = { object_->position_.x - 2.5f, object_->position_.y - 5.0f, object_->position_.z };
		col3D_.p[3] = { object_->position_.x - 2.5f, object_->position_.y + 5.0f, object_->position_.z };
	}

	if (isMove_) {
		timer_++;
		float rate = (float)timer_ / maxTime_;

		if (object_->rotation_.y >= 90.0f) {
			if (isRight_) {
				object_->position_.z = Easing::Circ::easeOut(oldPos_.z, oldPos_.z - 5.0f, rate);
			}
			else {
				object_->position_.z = Easing::Circ::easeOut(oldPos_.z, oldPos_.z + 5.0f, rate);
			}
		}
		else {
			if (isRight_) {
				object_->position_.x = Easing::Circ::easeOut(oldPos_.x, oldPos_.x - 5.0f, rate);
			}
			else {
				object_->position_.x = Easing::Circ::easeOut(oldPos_.x, oldPos_.x + 5.0f, rate);
			}
		}

		if (rate >= 1.0f) {
			timer_ = 0;

			if (object_->rotation_.y >= 90.0f) {
				if (isRight_) object_->position_.z = oldPos_.z - 5.0f;
				else object_->position_.z = oldPos_.z + 5.0f;
			}
			else {
				if (isRight_) object_->position_.x = oldPos_.x - 5.0f;
				else object_->position_.x = oldPos_.x + 5.0f;
			}

			if (isRight_) isRight_ = false;
			else isRight_ = true;
			isMove_ = false;
		}
	}
}

void Door::Draw()
{
	object_->Draw();
}
