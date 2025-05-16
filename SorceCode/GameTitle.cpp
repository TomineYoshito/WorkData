#include "GameTitle.h"

GameTitle::GameTitle()
{
	sceneManagerPtr = SceneManager::GetInstance();
	start_time = GetNowCount();
}

GameTitle::~GameTitle()
{

}

int GameTitle::UserLoader()
{
	return 0;
}

void GameTitle::Update()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
	// ���j���[�`��
	LoadGraphScreen(0, 0, "Assets/icon/demo2.png", TRUE);
	DrawFormatString(600, 700, GetColor(50,50,255), "�G���^�[�L�[��B�{�^���Ō���");

	if (PAD.Buttons[13]) {
		SceneChange("Select");
	}

	if (GetNowCount() - start_time > 5000) {
		// 5�b��ɃA�h�o�^�C�Y��ʂ֑J��
		SceneChange("advertise");
	}
}