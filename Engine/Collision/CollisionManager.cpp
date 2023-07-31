#include "CollisionManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "MeshCollider.h"

CollisionManager::~CollisionManager()
{
	// �R���C�_�[�폜
	colliders_.clear();
}

void CollisionManager::CheckAllCollision()
{
	for (std::forward_list<BaseCollider*>::iterator it = colliders_.begin(); it != colliders_.end(); ++it)
	{
		BaseCollider* collider = *it;
		collider->Update();
	}

	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// �S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;
		BaseCollider* colA = *itA;

		// �Փ˔�������Ȃ����ǂ���
		if (colA->GetIsCollision() == false) continue;

		// ���C��������
		if (colA->GetShapeType() == SHAPE_RAY) {
			bool result = false;

			// �����p�̃C�e���[�^
			std::forward_list<BaseCollider*>::iterator it;

			// ���܂łōł��߂��R���C�_�[���L�^���邽�߂̃C�e���[�^
			BaseCollider* it_hit = nullptr;

			// ���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
			float distance = FLT_MAX;

			// ���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
			Vector3 inter;

			// �S�ẴR���C�_�[�Ƃ̑�������`�F�b�N
			it = colliders_.begin();
			for (; it != colliders_.end(); ++it) {
				BaseCollider* col = *it;

				// �Փ˔�������Ȃ����ǂ���
				if (col->GetIsCollision() == false) continue;

				// ����������Ȃ���΃X�L�b�v
				if (!(colA->attribute_ & col->attribute_)) continue;

				Ray* ray = dynamic_cast<Ray*>(colA);

				// ���̏ꍇ
				if (col->GetShapeType() == SHAPE_SPHERE) {
					Sphere* sphere = dynamic_cast<Sphere*>(col);
					float tempDistance;
					Vector3 tempInter;

					// ������Ȃ���Ώ��O
					if (!Collision::CheckRay2Sphere(*ray, *sphere, &tempDistance, &tempInter)) continue;

					// �������ŏ��łȂ���Ώ��O
					if (tempDistance >= distance) continue;

					// ���܂ōł��߂��̂ŋL�^�����
					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}

				// ���b�V���̏ꍇ
				else if (col->GetShapeType() == SHAPE_MESH) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

					float tempDistance;
					Vector3 tempInter;
					if (!meshCollider->CheckCollisionRay(*ray, &tempDistance, &tempInter)) continue;
					if (tempDistance >= distance) continue;

					result = true;
					distance = tempDistance;
					inter = tempInter;
					it_hit = *it;
				}
			}

			// �������Ă�����
			if (result) {
				colA->SetIsHit(true);
				colA->SetInter(inter);
				it_hit->SetIsHit(true);
				it_hit->SetInter(inter);
			}
		}

		else {
			for (; itB != colliders_.end(); ++itB) {
				BaseCollider* colB = *itB;

				// �Փ˔�������Ȃ����ǂ���
				if (colB->GetIsCollision() == false) continue;

				// ����������Ȃ���΃X�L�b�v
				if (!(colA->attribute_ & colB->attribute_)) continue;

				// �Ƃ��ɋ�
				if (colA->GetShapeType() == SHAPE_SPHERE && colB->GetShapeType() == SHAPE_SPHERE) {
					Sphere* sphereA = dynamic_cast<Sphere*>(colA);
					Sphere* sphereB = dynamic_cast<Sphere*>(colB);
					Vector3 inter;
					if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colB->SetIsHit(true);
						colB->SetInter(inter);
					}
				}

				// ���b�V���Ƌ�
				else if (colA->GetShapeType() == SHAPE_MESH && colB->GetShapeType() == SHAPE_SPHERE) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					Vector3 inter;
					Vector3 reject;

					if (meshCollider->CheckCollisionSphere(*sphere, &inter, &reject)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colA->SetReject(reject);
						colA->SetCollider(colB);
						colB->SetIsHit(true);
						colB->SetInter(inter);
						colB->SetReject(reject);
						colB->SetCollider(colA);
					}
				}

				// ���ƃ��b�V��
				else if (colA->GetShapeType() == SHAPE_SPHERE && colB->GetShapeType() == SHAPE_MESH) {
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					Vector3 inter;
					Vector3 reject;

					if (meshCollider->CheckCollisionSphere(*sphere, &inter, &reject)) {
						colA->SetIsHit(true);
						colA->SetInter(inter);
						colA->SetReject(reject);
						colA->SetCollider(colB);
						colB->SetIsHit(true);
						colB->SetInter(inter);
						colB->SetReject(reject);
						colB->SetCollider(colA);
					}
				}
			}
		}
	}
}

CollisionManager* CollisionManager::GetInstance()
{
	// �C���X�^���X����
	static CollisionManager inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}