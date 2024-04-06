#pragma once
#include "Weapon.h"

class Revolver : public Weapon
{
#pragma region メンバ変数

#pragma endregion

#pragma region メンバ関数
public:
	Revolver(Object3D* inObject);
	~Revolver() {}

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Collision() override;
	void MatUpdate() override;
	void Finalize() override;

private:
	void Reset();
#pragma endregion
};