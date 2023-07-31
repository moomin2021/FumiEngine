#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider : public BaseCollider {
#pragma region �����o�ϐ�
private:
	// �O�p�`
	std::vector<Triangle> triangles_;

	// ���[���h�s��̋t�s��
	Matrix4 invMatWorld_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MeshCollider(Object3D* object);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);

	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);

private:
	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	/// <param name="model"> ���f�� </param>
	void ConstructTriangles(Model* model);
#pragma endregion
};