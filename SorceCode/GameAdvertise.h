#pragma once
#include "Scene.h"

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

class GameAdvertise : public Scene {
public:
	GameAdvertise();
	~GameAdvertise();

	void Update();

private:
	int movieGHandle;
	XINPUT_STATE PAD;
};