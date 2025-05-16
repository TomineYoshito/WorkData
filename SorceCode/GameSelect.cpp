#include "GameSelect.h"

static vector <AnimationInfo> animInfoflog = {
	{ 0, "dead" ,false},
	{ 1, "loading" ,false},
	{ 2, "damagi" ,false},
	{ 3, "idle" ,true},
	{ 4, "attack" ,false},
	{ 5, "dash" ,false},
	{ 6, "attack2" ,false},
	{ 7, "walk" ,false},
};

static vector <AnimationInfo> animInfoDoragon = {
	{ 0, "notanim" ,false},
	{ 1, "attack" ,false},
	{ 2, "death" ,false},
	{ 3, "idle" ,true},
	{ 4, "back" ,false},
	{ 5, "idle" ,true},
	{ 6, "walk" ,false},
	{ 7, "skill" ,false},
	{ 8, "skill2" ,false},
};

static vector <AnimationInfo> animInfowolf = {
	{ 0, "attack" ,false},
	{ 1, "attack2" ,false},
	{ 2, "damage" ,false},
	{ 3, "dead" ,false},
	{ 4, "idle" ,true},
	{ 5, "run" ,false},
	{ 6, "walk" ,false},
};



int GameSelect::selectMobnum = -1;

GameSelect::GameSelect() {
	sceneManagerPtr = SceneManager::GetInstance();

	MHandle = MV1LoadModel("Assets/flog.mv1");
	MHandle2 = MV1LoadModel("Assets/Dragon/Dragon.mv1");
	MHandle3 = MV1LoadModel("Assets/wolf.mv1");
	MV1SetPosition(MHandle, VGet(960.0f, 700.0f, 300.0f));
	MV1SetPosition(MHandle2, VGet(100.0f, -100.0f, 1000.0f));
	MV1SetPosition(MHandle3, VGet(1880.0f, 200.0f, 1000.0f));
	MV1SetRotationXYZ(MHandle, VGet(-0.5f,-5.5f, -0.5f));
	MV1SetRotationXYZ(MHandle2, VGet(0.0f, -6.0f, 0.0f));
	MV1SetRotationXYZ(MHandle3, VGet(0.f, -5.2f, 0.f));

	anim_id = 3;
	anim_id2 = 5;
	anim_id3 = 4;
	animatorPtr = new Animator();
	//animatorPtr->Load(animInfo);
	animatorPtr->Load(MHandle, animInfoflog);
	animatorPtr->Play(MHandle, anim_id);
	animator2Ptr = new Animator();
	animator2Ptr->Load(MHandle2, animInfoDoragon);
	animator2Ptr->Play(MHandle2, anim_id2);
	animator3Ptr = new Animator();
	//animatorPtr->Load(animInfo);
	animator3Ptr->Load(MHandle3, animInfowolf, 7);
	animator3Ptr->Play(MHandle3, anim_id3);

	// カメラの座標を初期化
	CameraPos.x = 900.0f;
	CameraPos.y = 500.0f;
	CameraPos.z = -1000.0f;

}

GameSelect::~GameSelect() {

}

void GameSelect::Update() {
	GetJoypadXInputState(DX_INPUT_PAD1, &PAD);

	// カメラの設定に反映する
	SetCameraPositionAndTarget_UpVecY(CameraPos, VGet(900.0f, 500.0f, 500.0f));

	DrawFormatString(900, 700, GetColor(255, 0, 0), "キャラ選択");

	if (PAD.Buttons[0]) {
		Flog = true;
		Doragon = false;
		Wolf = false;
	}
	if (Flog) {
		DrawCircle(980, 220, 200, GetColor(255,255,255), TRUE);
		anim_id = 5;
		animatorPtr->Play(MHandle, anim_id);
		if (PAD.Buttons[13]) {
			selectMobnum = 1;
			SceneChange("Main");
		}
	}
	else {
		anim_id = 3;
		animatorPtr->Play(MHandle, anim_id);
	}

	if (PAD.Buttons[2]) {
		Doragon = true;
		Flog = false;
		Wolf = false;
	}
	if (Doragon) {
		DrawCircle(480, 600, 200, GetColor(255, 255, 255), TRUE);
		anim_id2 = 6;
		animator2Ptr->Play(MHandle2, anim_id2);
		if (PAD.Buttons[13]) {
			selectMobnum = 0;
			SceneChange("Main");
		}
	}
	else {
		anim_id2 = 5;
		animator2Ptr->Play(MHandle2, anim_id2);
	}
	if (PAD.Buttons[3]) {
		Wolf = true;
		Doragon = false;
		Flog = false;
	}
	if (Wolf) {
		DrawCircle(1400, 600, 200, GetColor(255, 255, 255), TRUE);
		anim_id3 = 5;
		animator3Ptr->Play(MHandle3, anim_id3);
		if (PAD.Buttons[13]) {
			selectMobnum = 2;
			SceneChange("Main");
		}
	}
	else {
		anim_id3 = 4;
		animator3Ptr->Play(MHandle3, anim_id3);
	}
	if (PAD.Buttons[1]) {
		Doragon = false;
		Flog = false;
		Wolf = false;
	}

	MV1DrawModel(MHandle);
	MV1DrawModel(MHandle2);
	MV1DrawModel(MHandle3);
	animatorPtr->Update();
	animator2Ptr->Update();
	animator3Ptr->Update();

	if (!Doragon && !Flog && !Wolf) {
		DrawFormatString(850, 650, GetColor(255, 0, 0), "十字キーで選択");
	}
	if (Doragon || Flog || Wolf) {
		DrawFormatString(850, 650, GetColor(255, 0, 0), "Ａボタンで決定");
	}

	DrawFormatString(930, 430, GetColor(255, 0, 0), "防御型");
	DrawFormatString(400, 750, GetColor(255, 0, 0), "攻撃型");
	DrawFormatString(1350, 750, GetColor(255, 0, 0), "速度型");

	//DrawFormatString(400, 600, GetColor(50, 50, 255), "未完成");
	//DrawFormatString(1350, 600, GetColor(50, 50, 255), "未完成");
}
