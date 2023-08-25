#include "GameScene.h"
#include "Object3D.h"
#include "PipelineManager.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <json.hpp>

GameScene::GameScene() {}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
#pragma region ���C�g
	// ���C�g�O���[�v����
	lightGroup_ = std::make_unique<LightGroup>();

	// ���s��������
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->SetLightDir({ 1.0f, -1.0f, 0.0f });
	dirLight_->SetLightColor({ 1.0f, 1.0f, 1.0f });

	// ���s������o�^
	lightGroup_->AddDirLight(dirLight_.get());
#pragma endregion

	// �X�e�[�W�}�l�[�W���[����
	stageMgr_ = std::make_unique<StageManager>();
	stageMgr_->Initialize();

	// �G�l�~�[�}�l�[�W���[����
	enemyMgr_ = std::make_unique<EnemyManager>();
	enemyMgr_->Initialize();

	// �v���C���[����
	player_ = std::make_unique<Player>();
	player_->Initialize(enemyMgr_.get());

	// �I�u�W�F�N�g3D�Ƀ��C�g��K�p
	Object3D::SetLightGroup(lightGroup_.get());

	// �X�e�[�W�ǂݍ���
	LoadStage("Resources/Stage/stage1.json");
}

void GameScene::Update()
{
	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->Update();

	// �v���C���[
	player_->Update();

	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g�X�V����
	ObjUpdate();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �X�e�[�W�}�l�[�W���[
	stageMgr_->Draw();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->Draw();

	// �v���C���[
	player_->DrawObject3D();

	PipelineManager::PreDraw("Sprite");

	// �v���C���[�O�ʃX�v���C�g�`��
	player_->DrawSprite();
}

void GameScene::ObjUpdate()
{
	// �X�e�[�W�}�l�[�W���[
	stageMgr_->ObjUpdate();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->ObjUpdate();

	// �v���C���[
	player_->ObjUpdate();
}

void GameScene::OnCollision()
{
	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();

	// �X�e�[�W�}�l�[�W���[
	stageMgr_->OnCollision();

	// �G�l�~�[�}�l�[�W���[
	enemyMgr_->OnCollision();

	// �v���C���[
	player_->OnCollision();
}

void GameScene::LoadStage(std::string fileName)
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

				// ���f���ǉ�
				stageMgr_->AddModel(object["file_name"]);
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
			//enemyManager_->CreateAddEnemy0(objectData.translation, objectData.scaling);
		}

		else if (objectData.className == "Item") {
			// �A�C�e����ǉ�
			//ItemManager::GetInstace()->AddItem(objectData.translation);
		}

		else if (objectData.className == "BossGenerator") {
			enemyMgr_->SetBossGenerator(objectData.translation);
		}

		else {
			// �I�u�W�F�N�g�ǉ�
			stageMgr_->AddObject3D(objectData.fileName, objectData.translation, objectData.rotation, objectData.scaling);
		}
	}
}