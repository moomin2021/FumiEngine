#pragma once
#include "Object3D.h"
#include "Model.h"

#include <map>
#include <string>
#include <memory>

class LoadBlender
{
#pragma region �\����
	// ���x���f�[�^
	struct LevelData {
		// �I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			// �t�@�C����
			std::string fileName;
			float3 translation;
			float3 rotation;
			float3 scaling;
		};

		std::vector<ObjectData> objects;
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���f��
	std::map<std::string, std::unique_ptr<Model>> model_;

	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Object3D>> object_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LoadBlender();

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
};