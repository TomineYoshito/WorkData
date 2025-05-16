

#include "CollisionManager.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}


void CollisionManager::Add(Collider* colliderPtr)
{
	colliderPtr->colliderID = colliderID;
	colliderVec.push_back(colliderPtr);
	++colliderID;
}


void CollisionManager::Remove(Collider* colliderPtr)
{
	vector<Collider*>::iterator it = colliderVec.begin();
	while (it != colliderVec.end()) {
		Collider* colPtr = *it;
		if (colPtr->colliderID == colliderPtr->colliderID) {
			colliderVec.erase(it);
			break;
		}

		++it;
	}

	//colliderVec.erase(colliderVec.begin() + index);
}


void CollisionManager::Update()
{
	// ���F�|�C���g colliderVer�������蔻��Ώۂ�Collider�^�N���X��o�^����vector�z��
	for (int i = 0; i < colliderVec.size(); i++) {
		// ���Ƌ��œ����蔻�肷��
		if (colliderVec[i]->colliderType == Collider::SPHERE_TYPE) {
			SphereCollider* colliderPtr = (SphereCollider*)colliderVec[i];
			VECTOR center1 = VAdd(colliderPtr->GetGameObject()->trans, colliderPtr->offset);
			DrawSphere3D(center1, colliderPtr->radius, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

			for (int j = i + 1; j < colliderVec.size(); j++) {
				SphereCollider* colliderPtr2 = (SphereCollider*)colliderVec[j];
				VECTOR center2 = VAdd(colliderVec[j]->GetGameObject()->trans, colliderPtr2->offset);
				DrawSphere3D(center2, colliderPtr2->radius, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

				if (HitCheck_Sphere_Sphere(center1, colliderPtr->radius, center2, colliderPtr2->radius)) {
					// �q�b�g���̃R�[���o�b�N�֐��Ăяo��
					// �q�b�g�����I�u�W�F�N�g��GameObject��Collider��n��
					colliderPtr->OnCollisionHit(colliderVec[j], colliderVec[j]->GetGameObject());
					colliderVec[j]->OnCollisionHit(colliderPtr, colliderPtr->GetGameObject());
				}
			}
		}

		// �J�v�Z���ƃJ�v�Z���œ����蔻�肷��
		if (colliderVec[i]->colliderType == Collider::CAPSULE_TYPE) {
			CapsuleCollider* colliderPtr = (CapsuleCollider*)colliderVec[i];
			//DrawCapsule3D(colliderPtr->StartPos, colliderPtr->EndPos, colliderPtr->radius, 32, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);

			for (int j = i + 1; j < colliderVec.size(); j++) {
				CapsuleCollider* colliderPtr2 = (CapsuleCollider*)colliderVec[j];
				//DrawCapsule3D(colliderPtr2->StartPos, colliderPtr2->EndPos, colliderPtr2->radius, 32, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);

				if (HitCheck_Capsule_Capsule(colliderPtr->StartPos, colliderPtr->EndPos, colliderPtr->radius,
					colliderPtr2->StartPos, colliderPtr2->EndPos, colliderPtr2->radius)) {
					// �q�b�g���̃R�[���o�b�N�֐��Ăяo��
					// �q�b�g�����I�u�W�F�N�g��GameObject��Collider��n��
					colliderPtr->OnCollisionHit(colliderVec[j], colliderVec[j]->GetGameObject());
					colliderVec[j]->OnCollisionHit(colliderPtr, colliderPtr->GetGameObject());
				}
			}
		}
	}
}