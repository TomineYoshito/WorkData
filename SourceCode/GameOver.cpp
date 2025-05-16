#include "GameOver.h"

GameOver::GameOver() {
	sceneManagerPtr = SceneManager::GetInstance();
}

GameOver::~GameOver() {

}

void GameOver::Update() {
	GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
	LoadGraphScreen(0, 0, "Assets/icon/GameOver.png", TRUE);

	if (PAD.Buttons[13]) {
		SceneChange("title");
	}
}