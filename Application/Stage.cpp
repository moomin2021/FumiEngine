#include "Stage.h"
#include "CollisionAttribute.h"
#include "PipelineManager.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

Stage::Stage() {}

Stage::~Stage() {}

void Stage::MatUpdate()
{
	// �I�u�W�F�N�g�s��X�V
	for (auto& object : objects_) object->MatUpdate();
}

void Stage::Draw()
{
	// �`��ݒ���I�u�W�F�N�g3D�p��
	PipelineManager::PreDraw("Object3D");

	// �I�u�W�F�N�g�`�揈��
	for (auto& object : objects_) object->Draw();
}

void Stage::Load(std::string fileName)
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

				// �����t�@�C�����̃��f�������������烂�f���ǉ�
				if (models_.count(object["file_name"]) == 0) {
					models_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
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
			// �G��ǉ�
			//enemyMgr_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else if (objectData.className == "Item") {
			// �A�C�e����ǉ�
			//ItemManager::GetInstace()->AddItem(objectData.translation);
		}

		else if (objectData.className == "BossGenerator") {
			//enemyMgr_->SetBossGenerator(objectData.translation);
		}

		// �I�u�W�F�N�g�ǉ�
		else {
			// �I�u�W�F�N�g
			objects_.emplace_front(std::make_unique<Object3D>(models_[objectData.fileName].get()));
			objects_.front()->SetPosition(objectData.translation);
			objects_.front()->SetRotation(objectData.rotation);
			objects_.front()->SetScale(objectData.scaling);

			// �R���C�_�[
			colliders_.emplace_front(std::make_unique<MeshCollider>(objects_.front().get()));
			colliders_.front()->SetAttribute(COL_STAGE_OBJ);
			colliders_.front()->SetObject3D(objects_.front().get());
			colMgr_->AddCollider(colliders_.front().get());
		}
	}
}