#pragma once
#include "Dxlib.h"
#include "GameObject.h"
#include "Animator.h"
#include "Collision.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Player2.h"
#include "Player3.h"


using namespace std;

class EnemyAttack : public GameObject, CapsuleCollider
{
public:
	int frameIndices[2];

	EnemyAttack() {

	}

	~EnemyAttack() {

	}

	void Update() {

	}

	void Draw() {

	}


	void SetPlayer(Player* p) {
		playerPtr = p;
	}

	void SetPlayer2(Player2* p2) {
		player2Ptr = p2;
	}

	void SetPlayer3(Player3* p3) {
		player3Ptr = p3;
	}


	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr);
private:
	Player* playerPtr;
	Player2* player2Ptr;
	Player3* player3Ptr;

};

class Enemy : public GameObject, CapsuleCollider
{
public:
	static const int STATUS_LIVE = 1;
	static const int STATUS_DEAD = 2;
	static const int STATUS_DAMAGE = 3;


	static const int ACT_STATUS_ATTCK = 2;
	static const int ACT_STATUS_CHASE = 1;
	static const int ACT_STATUS_STOP = 0;

public:
	Enemy();
	~Enemy();
	void Move();
	void Update();
	void EnemyCollision();
	void transform_collider();
	void SetupCollision(CollisionManager* managerPtr);
	void Think();
	void targetChase();
	void Status();
	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr);

	void SetTargetPos(VECTOR pos) {// 追いかけるターゲットのワールド座標をセット④
		targetPos = pos;
	}

	vector<Collider*> GetCollider() {
		return attackVec;
	}

	Player* GetPlayer() { return playerPtr; }
	Player2* GetPlayer2() { return player2Ptr; }
	Player3* GetPlayer3() { return player3Ptr; }
	void SetPlayer(Player* p) {
		playerPtr = p;
	}
	void SetPlayer2(Player2* p2) {
		player2Ptr = p2;
	}
	void SetPlayer3(Player3* p3) {
		player3Ptr = p3;
	}


	int MHandle;
	float EHP;
	float Edamage;
	bool Hit;

private:
	int anim_id;

	int status;
	int actStatus;
	int GetEXP;
	bool EAttack;
	bool dead;
	bool deadanim;
	bool EXP;

	VECTOR targetPos;


	vector<Collider*> attackVec;

	Animator* animatorPtr;
	CollisionManager* collisionManagerPtr;
	Player* playerPtr;
	Player2* player2Ptr;
	Player3* player3Ptr;

};

