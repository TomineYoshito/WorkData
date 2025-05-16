#include "GameClear.h"

GameClear::GameClear() {
	sceneManagerPtr = SceneManager::GetInstance();

    ClearBGM = LoadSoundMem("Assets/sound/o12.mp3");
}

GameClear::~GameClear() {

}

void GameClear::Update() {
    GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
	LoadGraphScreen(0, 0, "Assets/icon/GameClear.png", TRUE);
    Clear = true;
    if (Clear && !BGM2) {
        ChangeVolumeSoundMem(180, ClearBGM);
        PlaySoundMem(ClearBGM, DX_PLAYTYPE_LOOP);
        BGM2 = true;
    }

    if (PAD.Buttons[13]) {
        StopSoundMem(ClearBGM);
        BGM2 = false;
        SceneChange("title");
    }
}