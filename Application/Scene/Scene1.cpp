#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

using namespace DirectX;

Scene1::Scene1() {}

Scene1::~Scene1() {}

void Scene1::Initialize()
{
#pragma region ���C�g
	// ���s�����̐����Ɛݒ�
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });

	// ���C�g�O���[�v�̐���
	lightGroup_ = std::make_unique<LightGroup>();

	// ���s���������C�g�O���[�v�ɓo�^
	lightGroup_->AddDirLight(dirLight_.get());

	// ���C�g��K�p
	Object3D::SetLightGroup(lightGroup_.get());
#pragma endregion

	// �v���C���[����
	player_ = std::make_unique<Player>();

	// �G�l�~�[�}�l�[�W���[����
	enemyManager_ = std::make_unique<EnemyManager>();

	// �X�e�[�W�ǂݍ���
	LoadStage("Resources/Stage/stage1.json");
}

void Scene1::Update()
{
	// �G�X�V
	enemyManager_->Update();

	// �X�e�[�W��̃I�u�W�F�N�g�X�V
	for (auto& i : stageObjects_) i->Update();

	// �v���C���[�X�V
	player_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �G�`��
	enemyManager_->Draw();

	// �X�e�[�W��̃I�u�W�F�N�g�`��
	for (auto& i : stageObjects_) i->Draw();

	// �v���C���[�`��
	player_->Object3DDraw();

	PipelineManager::PreDraw("Sprite");

	// �v���C���[�O�ʃX�v���C�g�`��
	player_->FrontSpriteDraw();
}

void Scene1::LoadStage(std::string fileName)
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

				if (stageModels_.count(object["file_name"]) == 0) {
					stageModels_.emplace(object["file_name"], std::make_unique<Model>(object["file_name"]));
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
			enemyManager_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else {
			// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
			std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>(stageModels_[objectData.fileName].get());

			// ���W
			newObject->SetPosition(objectData.translation);

			// ��]�p
			newObject->SetRotation(objectData.rotation);

			// �g�k
			newObject->SetScale(objectData.scaling);

			// �z��ɓo�^
			stageObjects_.emplace_back(std::move(newObject));
		}
	}
}