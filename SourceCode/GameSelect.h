#pragma once
#include "Dxlib.h"
#include <math.h>
#include <string>
#include <vector>
#include "Scene.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "Animator.h"

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

class GameSelect :public Scene {
public:
	GameSelect();
	~GameSelect();

	void Update();
	//bool FlogStart;
	//bool DoragonStart;

	static int selectMobnum;

private:

	int MHandle;
	int MHandle2;
	int MHandle3;
	int anim_id;
	int anim_id2;
	int anim_id3;
	bool Flog;
	bool Doragon;
	bool Wolf;
	VECTOR CameraPos;
	Animator* animatorPtr;
	Animator* animator2Ptr;
	Animator* animator3Ptr;
	SceneManager* sceneManagerPtr;    // �V�[���Ǘ��N���X����
	XINPUT_STATE PAD;
};