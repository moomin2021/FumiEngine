#include "LoadStage.h"
#include "EnemyManager.h"
#include "Enemy0.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

LoadStage::LoadStage()
{
	mEnemy_ = std::make_unique<Model>("cube");
	LoadJson("Resources/Stage/stage1.json");
}

void LoadStage::Update()
{
	//for (size_t i = 0; i < enemys_.size(); i++) {
	//	// �e�̍X�V����
	//	enemys_[i]->Update();

	//	// �����t���O��[OFF]��������
	//	if (enemys_[i]->GetIsAlive() == false) {
	//		// �e������
	//		enemys_.erase(enemys_.begin() + i);
	//	}
	//}
	for (auto& objs : object_) {
		objs->Update();
	}

}

void LoadStage::Draw()
{
	for (auto& objs : object_) {
		objs->Draw();
	}

	//for (auto& enemys : enemys_) {
	//	enemys->Draw();
	//}
}

void LoadStage::LoadJson(std::string fileName)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fileName);

	// �t�@�C���I�[�v�����s�`�F�b�N
	if (file.fail()) assert(0);

	// JSON�����񂩂�񓚂����f�[�^
	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���G�f�B�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	// ���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// �v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});

			// ���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			// ���f��
			if (object.contains("file_name")) {
				// �t�@�C����
				objectData.fileName = object["file_name"];

				if (model_.count(object["file_name"]) == 0) {
					model_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
				}
			}

			// �N���X�̖��O
			if (object.contains("class_name")) {
				objectData.className = object["class_name"];
			}

			// visible(�\����])
			//objectData.isActive = object["visible"];

			// �������X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];

			// ���s�ړ�
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			// ��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][0];

			// �X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];
		}

		// �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
		if (object.contains("children")) {

		}
	}

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		if (objectData.className == "Enemy") {
			//std::unique_ptr<Enemy0> newEnemy = std::make_unique<Enemy0>();
			Enemy0* newEnemy = new Enemy0(mEnemy_.get());

			// ������
			newEnemy->Initialize(objectData.translation);

			// �z��ɓo�^
			EnemyManager::GetInstance()->AddEnemy(newEnemy);
			//enemys_.emplace_back(std::move(newEnemy));
		}

		else {
			// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
			std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>(model_[objectData.fileName].get());

			// ���W
			newObject->SetPosition(objectData.translation);

			// ��]�p
			newObject->SetRotation(objectData.rotation);

			// �g�k
			newObject->SetScale(objectData.scaling);

			// �\����\��
			//newObject->SetActive(objectData.isActive);

			// �z��ɓo�^
			object_.emplace_back(std::move(newObject));
		}
	}
}