#pragma once
#include "Dxlib.h"
#include <math.h>
#include <string>
#include <vector>
#include "GameObject.h"
#include "Animator.h"
#include "Collision.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "EffectManager.h"

using namespace std;

#define JUMP_POWER			25.0f;
#define GRAVITY				1.0f;
// XInputボタン入力定義
#define XINPUT_BUTTON_DPAD_UP		(0)	// デジタル方向ボタン上
#define XINPUT_BUTTON_DPAD_DOWN		(1)	// デジタル方向ボタン下
#define XINPUT_BUTTON_DPAD_LEFT		(2)	// デジタル方向ボタン左
#define XINPUT_BUTTON_DPAD_RIGHT	(3)	// デジタル方向ボタン右
#define XINPUT_BUTTON_START		(4)	// STARTボタン
#define XINPUT_BUTTON_BACK		(5)	// BACKボタン
#define XINPUT_BUTTON_LEFT_THUMB	(6)	// 左スティック押し込み
#define XINPUT_BUTTON_RIGHT_THUMB	(7)	// 右スティック押し込み
#define XINPUT_BUTTON_LEFT_SHOULDER	(8)	// LBボタン
#define XINPUT_BUTTON_RIGHT_SHOULDER	(9)	// RBボタン
#define XINPUT_BUTTON_A			(12)	// Aボタン
#define XINPUT_BUTTON_B			(13)	// Bボタン
#define XINPUT_BUTTON_X			(14)	// Xボタン
#define XINPUT_BUTTON_Y			(15)	// Yボタン

class PlayerAttack : public GameObject, CapsuleCollider
{
public:
	int frameIndices[2];

	PlayerAttack() {

	}

	~PlayerAttack() {

	}

	void Update() {

	}

	void Draw() {

	}

	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {
	}

};

class Player : public GameObject, CapsuleCollider
{
public:
	Player();       //コンストラクタ
	~Player();      //デストラクタ

	CollisionRect GetCollisionRect() {
		MATRIX matrix = MV1GetMatrix(MHandle);
		CollisionRect rect;
		rect.min_pos = VGet(-120.f, -150.f, 40.f);
		rect.max_pos = VGet(50.f, 150.f, -40.f);
		rect.min_pos = VTransform(rect.min_pos, matrix);
		rect.max_pos = VTransform(rect.max_pos, matrix);
		DrawLine3D(rect.min_pos, rect.max_pos, GetColor(255, 0, 0));
		return rect;
	}

	void SetWallHit(bool flag) {
		IsWallHit = flag;
	}
	void SetGroundHit(bool flag, VECTOR pos) {
		GroundPos = pos;
		IsGroundHit = flag;
	}

	void Update();
	void Attack();
	void AttackCollision();
	void transform_collider();
	void SetupCollision(CollisionManager* managerPtr);
	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr);    // コリジョン判定でヒットした時のコールバック関数
	vector<Collider*> GetCollider() {
		return attackVec;
	}
	void SetupEffectManager(EffectManager* ptr);

	int AttackPoint = 10;
	int AttackPointUP = 20;
	int EXP = 0;
	bool Dive;
	bool dead;
	bool isAttack;
	bool isAttack2;
	bool P1;


private:
	int MHandle;
	int MouseX;
	int MouseY;
	int X;
	int JampPower = 0;
	int Time = 0;
	int Level = 1;
	int Level2;
	int SPoint = 3;
	int SPoint2 = 5;
	int GHandle;
	int GHandle2;
	int GHandle3;
	int GHandle4;
	int GHandle5;
	int anim_id;
	int Sound1;
	int Sound2;
	int Sound3;
	int Hitcnt;
	int EHitcnt;
	int Effectcnt;
	float speed = 10;
	float HP;
	float MAXHP;
	float damage;
	float damage2;
	bool work;
	bool work2;
	bool Step;
	bool IsJamp;
	bool IsGroundHit;
	bool IsWallHit;
	bool CoolTime;
	bool command;
	bool command2;
	bool Hit;
	bool EHit;
	bool end;
	bool notAttack;
	bool DiveFlag1;
	bool DiveFlag2;
	bool SoundFlag;
	bool SoundFlag2;
	bool SoundFlag3;
	bool PAD_key;
	bool effect1;
	bool effect2;
	unsigned int Cr;
	unsigned int Cr2;
	unsigned int Cr3;
	unsigned int Cr4;
	bool key;
	Animator* animatorPtr;
	CollisionManager* collisionManagerPtr;
	EffectManager* effectManagerPtr;

	vector<Collider*> attackVec;
	VECTOR cameraPosition;
	VECTOR GroundPos;
	XINPUT_STATE PAD;
	float cameraHAngle;
	float cameraVAngle;
	void Move();
	void Jamp();
	void Camera();
	void Status();
};

