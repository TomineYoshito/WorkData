#include "DxLib.h"
#include "Player.h"
#include "GameObject.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "GameMain.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "GameTitle.h"
#include "GameClear.h"

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

using namespace std;

SceneManager* scenemanagerPtr = SceneManager::GetInstance();

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(true); // Windowモード
    SetGraphMode(1920, 1080, 32);    // 画面サイズの設定

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1) return -1;        // エラーが起きたら直ちに終了

    SetDrawScreen(DX_SCREEN_BACK);  // 描画する画面の指定
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    

	// シーン変更
	scenemanagerPtr->SceneChange("title");

    // カメラ位置
    VECTOR CameraPos = VGet(320.0f, 420.0f, 1.0f);
    CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));

	// 描画する文字列のサイズを設定
	SetFontSize(50);

    // メインループ
    while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        // メッセージループに代わる処理をする
        if (ProcessMessage() == -1) break;        // エラーが起きたらループを抜ける

        ClearDrawScreen();  // 画面消去
        SetUseBackCulling(TRUE);

        

        // シーン実行
        scenemanagerPtr->Execute();
		//bossPtr->Update();

        ScreenFlip();   // 画面フリップ
    }

	delete scenemanagerPtr;    // プレイヤーの削除

    DxLib_End();    // ＤＸライブラリ使用の終了処理
    return 0;       // ソフトの終了
}