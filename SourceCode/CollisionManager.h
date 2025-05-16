#pragma once
#include <vector>
#include "GameObject.h"
#include "Collider.h"

using namespace std;

class CollisionManager : public GameObject
{
public:
	CollisionManager();
	~CollisionManager();

	void Update();
	void Add(Collider* colliderPtr);
	void Remove(Collider* colliderPtr);

private:
	int colliderID;
	vector<Collider*> colliderVec;	// ※：ポイント　コリジョン判定するColliderオブジェクト
};


