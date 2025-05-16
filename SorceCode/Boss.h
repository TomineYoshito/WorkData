#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "Collision.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "Player2.h"
#include "Player3.h"

using namespace std;

static float BHP;
static float Bdamage;

class BossAttack : public GameObject, CapsuleCollider
{
public:
	int frameIndices[2];

	BossAttack() {

	}

	~BossAttack() {

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
	bool Hit;
};

class Boss : public GameObject, CapsuleCollider
{
public:
	static const int ACT_STATUS_ATTCK = 2;
	static const int ACT_STATUS_CHASE = 1;
	static const int ACT_STATUS_STOP = 0;

public:
	Boss();
	~Boss();
	void Move();
	void Update();
	void BossCollision();
	void transform_collider();
	void SetupCollision(CollisionManager* managerPtr);
	void Think();
	void targetChase();
	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr);    // コリジョン判定でヒットした時のコールバック関数
	void SetupEffectManager(EffectManager* ptr);
	vector<Collider*> GetCollider() {
		return attackVec;
	}

	void SetTargetPos(VECTOR pos);
	void Status();


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

	bool dead;
private:
	int anim_id;
	int MHandle;
	int actStatus;
	int Sound1;
	int Sound2;
	int Sound3;
	int cnt;
	int BHP2;
	float damage;
	float damage2;
	float MAXHP;
	float BRast = 10000.f;
	bool isAttack;
	bool end;
	bool SoundFlag;
	bool RastSpart;
	bool RastSpart2;
	bool BAttack;
	bool AttackSound;
	bool BStart;
	VECTOR targetPos;
	Animator* animatorPtr;
	vector<Collider*> attackVec;
	vector<GameObject*> gameObjectVec;
	CollisionManager* collisionManagerPtr;
	EffectManager* effectManagerPtr;
	Player* playerPtr;
	Player2* player2Ptr;
	Player3* player3Ptr;
};

