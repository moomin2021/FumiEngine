#pragma once
#include "Object3D.h"
#include "Model.h"
#include "Enemy.h"

#include <map>
#include <string>
#include <memory>

class LoadStage
{
#pragma region �\����
	// ���x���f�[�^
	struct LevelData {
		// �I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			// �t�@�C����
			int isActive;
			std::string fileName;
			std::string className;
			float3 translation;
			float3 rotation;
			float3 scaling;
		};

		std::vector<ObjectData> objects;
	};
#pragma endregion

#pragma region �����o�ϐ�
public:
	std::vector<std::unique_ptr<Enemy>> enemys_;

private:
	// ���f��
	std::map<std::string, std::unique_ptr<Model>> model_;

	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Object3D>> object_;

	// �G
	std::unique_ptr<Model> mEnemy_;

#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LoadStage();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Json�t�@�C���ǂݍ���
	/// </summary>
	void LoadJson(std::string fileName);
#pragma endregion

#pragma region �Q�b�^�[�֐�

#pragma endregion
};