#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "GameMain.h"

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

class GameOver : public Scene {
public:
	GameOver();
	~GameOver();

	void Update();
private:
	SceneManager* sceneManagerPtr;
	XINPUT_STATE PAD;
};
