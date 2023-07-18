#pragma once
#include "BaseEnemy.h"
#include "Object3D.h"
#include "SphereCollider.h"

#include <memory>

class Enemy0 : public BaseEnemy
{
#pragma region �����o�ϐ�
private:
	
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model"> ���f�� </param>
	Enemy0(Model* model);

	// �f�X�g���N�^
	~Enemy0();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos"> �����ʒu </param>
	void Initialize(float3 pos);

	// �X�V����
	void Update() override;

	// �`�揈��
	void Draw() override;
#pragma endregion
};