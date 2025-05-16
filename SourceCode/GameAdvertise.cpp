#include "GameAdvertise.h"


GameAdvertise::GameAdvertise() {
	//	PlayMovie("Assets/Movie/Ys7_demo_web.wmv", 3, DX_MOVIEPLAYTYPE_NORMAL);
	movieGHandle = LoadGraph("Assets/Movie/DxLib 2024-02-14 10-45-32.mp4");
	SeekMovieToGraph(movieGHandle, 0);
	PlayMovieToGraph(movieGHandle);
}

GameAdvertise::~GameAdvertise() {
	PauseMovieToGraph(movieGHandle);
	DeleteGraph(movieGHandle);
}

void GameAdvertise::Update() {
	//DrawGraph(0, 0, movieGHandle, FALSE);
	GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
	DrawRotaGraph(1020, 530, 1.0f, 0.f, movieGHandle, FALSE, FALSE);
	if (GetMovieStateToGraph(movieGHandle) == 0) {
		// 動画再生が終了したらタイトルへ遷移
		SceneChange("title");
	}

	if (CheckHitKey(KEY_INPUT_SPACE)) {
		// スペースが押されたらタイトルへ遷移
		SceneChange("title");
	}

	if (PAD.Buttons[13]) {
		SceneChange("title");
	}

	WaitTimer(10);
}