#pragma once
#include "CollisionPrimitive.h"

class Collision {
#pragma region �����o�֐�
public:
	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere0"> ��0 </param>
	/// <param name="sphere1"> ��1 </param>
	/// <param name="inter">  </param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere0, const Sphere& sphere1, Vector3* inter = nullptr);

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="plane"> ���� </param>
	/// <param name="inter"> ��_(���ʏ�̍ŋߐړ_) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point"> �_ </param>
	/// <param name="traiangle"> �O�p�` </param>
	/// <param name="closest"> �ŋߐړ_(�o�͗p) </param>
	/// <returns>  </returns>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="triangle"> �O�p�` </param>
	/// <param name="inter"> ��_(�O�p�`��̍ŋߐړ_) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="plane"> ���� </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="triangle"> �O�p�` </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="sphere"> �� </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
#pragma endregion
};