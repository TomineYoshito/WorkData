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

using namespace std;

SceneManager* scenemanagerPtr = SceneManager::GetInstance();

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(true); // Window���[�h
    SetGraphMode(1920, 1080, 32);    // ��ʃT�C�Y�̐ݒ�

    // �c�w���C�u��������������
    if (DxLib_Init() == -1) return -1;        // �G���[���N�����璼���ɏI��

    SetDrawScreen(DX_SCREEN_BACK);  // �`�悷���ʂ̎w��
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    

	// �V�[���ύX
	scenemanagerPtr->SceneChange("title");

    // �J�����ʒu
    VECTOR CameraPos = VGet(320.0f, 420.0f, 1.0f);
    CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));

	// �`�悷�镶����̃T�C�Y��ݒ�
	SetFontSize(50);

    // ���C�����[�v
    while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        // ���b�Z�[�W���[�v�ɑ��鏈��������
        if (ProcessMessage() == -1) break;        // �G���[���N�����烋�[�v�𔲂���

        ClearDrawScreen();  // ��ʏ���
        SetUseBackCulling(TRUE);

        

        // �V�[�����s
        scenemanagerPtr->Execute();
		//bossPtr->Update();

        ScreenFlip();   // ��ʃt���b�v
    }

	delete scenemanagerPtr;    // �v���C���[�̍폜

    DxLib_End();    // �c�w���C�u�����g�p�̏I������
    return 0;       // �\�t�g�̏I��
}