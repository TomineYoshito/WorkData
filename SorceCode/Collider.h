#pragma once
#include "GameObject.h"

class Collider
{
public:
	static const int SPHERE_TYPE = 1;
	static const int BOX_TYPE = 2;
	static const int CAPSULE_TYPE = 3;

	int colliderID;
	int colliderType;

public:
	Collider();
	~Collider();

	void SetGameObject(GameObject* gobjPtr) {
		gameObjectPtr = gobjPtr;
	}

	GameObject* GetGameObject() {
		return gameObjectPtr;
	}

	// コリジョン判定でヒットした時のコールバック関数の仮想関数
	// 派生クラス側で必ず実装する必要がある。
	virtual void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) = 0;

private:
	GameObject* gameObjectPtr;
};

class SphereCollider : public Collider {
public:
	SphereCollider() {
		offset = VGet(0.f, 0.f, 0.f);
		radius = 0.f;
	}

	~SphereCollider() {}

	VECTOR offset;
	float radius;
};

class CapsuleCollider : public Collider {
public:
	CapsuleCollider() {
		StartPos = VGet(0.f, 0.f, 0.f);
		EndPos = VGet(0.f, 0.f, 0.f);
		orgStartPos = VGet(0.f, 0.f, 0.f);
		orgEndPos = VGet(0.f, 0.f, 0.f);
		radius = 0.f;
	}

	~CapsuleCollider() {}

	VECTOR orgStartPos;
	VECTOR orgEndPos;
	VECTOR StartPos;
	VECTOR EndPos;
	float radius;
};

class BoxCollider : public Collider {
public:
	BoxCollider() {
		minPos = VGet(0.f, 0.f, 0.f);
		maxPos = VGet(0.f, 0.f, 0.f);
	}

	~BoxCollider() {}

	VECTOR maxPos;
	VECTOR minPos;
	VECTOR localBoxMaxPos;
	VECTOR localBoxMinPos;
};
