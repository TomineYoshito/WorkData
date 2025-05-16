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
// XInput�{�^�����͒�`
#define XINPUT_BUTTON_DPAD_UP		(0)	// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_DOWN		(1)	// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_LEFT		(2)	// �f�W�^�������{�^����
#define XINPUT_BUTTON_DPAD_RIGHT	(3)	// �f�W�^�������{�^���E
#define XINPUT_BUTTON_START		(4)	// START�{�^��
#define XINPUT_BUTTON_BACK		(5)	// BACK�{�^��
#define XINPUT_BUTTON_LEFT_THUMB	(6)	// ���X�e�B�b�N��������
#define XINPUT_BUTTON_RIGHT_THUMB	(7)	// �E�X�e�B�b�N��������
#define XINPUT_BUTTON_LEFT_SHOULDER	(8)	// LB�{�^��
#define XINPUT_BUTTON_RIGHT_SHOULDER	(9)	// RB�{�^��
#define XINPUT_BUTTON_A			(12)	// A�{�^��
#define XINPUT_BUTTON_B			(13)	// B�{�^��
#define XINPUT_BUTTON_X			(14)	// X�{�^��
#define XINPUT_BUTTON_Y			(15)	// Y�{�^��

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
	Player();       //�R���X�g���N�^
	~Player();      //�f�X�g���N�^

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
	void OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr);    // �R���W��������Ńq�b�g�������̃R�[���o�b�N�֐�
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

