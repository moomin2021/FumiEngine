#pragma once
#include "Object3D.h"
#include <memory>
#include "ColType.h"

class Wall {
public:
	std::unique_ptr<Object3D> object_;
	Line2D col2D_;
	Board col3D_;

public:
	Wall();
	void Draw();
};