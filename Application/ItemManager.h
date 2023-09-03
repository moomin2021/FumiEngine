#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"

#include <vector>
#include <memory>

enum ItemNumber {
	ExtendedMagazine,// �g���}�K�W��
	Syringe,// ���ˊ�
};

struct Item {
	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> obj = nullptr;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> col = nullptr;

	// �A�C�e���ԍ�
	uint8_t itemNum = 0;
};

class ItemManager {
#pragma region �����o�ϐ�
private:
	// �e�A�C�e���̃��f��
	std::vector<std::unique_ptr<Model>> mItems_;

	// �e�A�C�e���̃I�u�W�F�N�g
	std::vector<Item> items_;
#pragma endregion

#pragma region �����o�֐�
public:
	static ItemManager* GetInstace();

	// �f�X�g���N�^
	~ItemManager();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �A�C�e���ǉ�
	void AddItem(const Vector3& pos);

	void DeleteItem(SphereCollider* collider);

private:
	// �R���X�g���N�^
	ItemManager();
#pragma endregion
};