#include "GameMain.h"

GameMain::GameMain() {
	sceneManagerPtr = SceneManager::GetInstance();
	collisionManagerPtr = new CollisionManager();

	// GameObjectを登録
	bossPtr = new Boss();
	GameObject* gobjPtr = bossPtr;
	gobjPtr->tag_name = "Boss";
	gameObjectVec.push_back(gobjPtr);

	enemyPtr[0] = new Enemy();
	gobjPtr = enemyPtr[0];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	CapsuleCollider* colliderPtr = (CapsuleCollider*)enemyPtr[0];
	colliderPtr->SetGameObject(enemyPtr[0]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[1] = new Enemy();
	//	座標
	enemyPtr[1]->trans = VGet(-1000.0f, 100.0f, 800.f);
	gobjPtr = enemyPtr[1];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[1];
	colliderPtr->SetGameObject(enemyPtr[1]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[2] = new Enemy();
	//	座標
	enemyPtr[2]->trans = VGet(-8800.0f, 100.0f, 5500.f);
	gobjPtr = enemyPtr[2];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[2];
	colliderPtr->SetGameObject(enemyPtr[2]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[3] = new Enemy();
	//	座標
	enemyPtr[3]->trans = VGet(-8300.0f, 100.0f, 4200.f);
	gobjPtr = enemyPtr[3];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[3];
	colliderPtr->SetGameObject(enemyPtr[3]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[4] = new Enemy();
	//	座標
	enemyPtr[4]->trans = VGet(-6000.0f, 100.0f, 550.f);
	gobjPtr = enemyPtr[4];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[4];
	colliderPtr->SetGameObject(enemyPtr[4]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[5] = new Enemy();
	//	座標
	enemyPtr[5]->trans = VGet(-6800.0f, 100.0f, 1900.f);
	gobjPtr = enemyPtr[5];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[5];
	colliderPtr->SetGameObject(enemyPtr[5]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[6] = new Enemy();
	//	座標
	enemyPtr[6]->trans = VGet(-6400.0f, 100.0f, 2300.f);
	gobjPtr = enemyPtr[6];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[6];
	colliderPtr->SetGameObject(enemyPtr[6]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[7] = new Enemy();
	//	座標
	enemyPtr[7]->trans = VGet(-8200.0f, 100.0f, 2600.f);
	gobjPtr = enemyPtr[7];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[7];
	colliderPtr->SetGameObject(enemyPtr[7]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[8] = new Enemy();
	//	座標
	enemyPtr[8]->trans = VGet(-4000.0f, 100.0f, 600.f);
	gobjPtr = enemyPtr[8];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[8];
	colliderPtr->SetGameObject(enemyPtr[8]);
	collisionManagerPtr->Add(colliderPtr);

	enemyPtr[9] = new Enemy();
	//	座標
	enemyPtr[9]->trans = VGet(-7200.0f, 100.0f, 2500.f);
	gobjPtr = enemyPtr[9];
	gobjPtr->tag_name = "Enemy";
	gameObjectVec.push_back(gobjPtr);

	colliderPtr = (CapsuleCollider*)enemyPtr[9];
	colliderPtr->SetGameObject(enemyPtr[9]);
	collisionManagerPtr->Add(colliderPtr);

	// プレイヤーのインスタンス生成
	playerPtr = nullptr;
	player2Ptr = nullptr;
	player3Ptr = nullptr;


	// GameObjectを登録
	// ※：ポイント②　タグ名を設定しておく。当たり判定のコールバック関数で判定用に使う
	if (GameSelect::selectMobnum == 1) {
		playerPtr = new Player();

		gobjPtr = playerPtr;
		gobjPtr->tag_name = "Player";
		gameObjectVec.push_back(gobjPtr);
		// ※：ポイント①　プレイヤーのコライダー情報を設定しコリジョンマネージャーに登録する
		CapsuleCollider* colliderPtr = (CapsuleCollider*)playerPtr;
		colliderPtr->SetGameObject(playerPtr);
		collisionManagerPtr->Add(colliderPtr);

		playerPtr->SetupCollision(collisionManagerPtr);
	}
	if (GameSelect::selectMobnum == 0) {
		player2Ptr = new Player2();

		gobjPtr = player2Ptr;
		gobjPtr->tag_name = "Player2";
		gameObjectVec.push_back(gobjPtr);
		player2Ptr->SetupCollision(collisionManagerPtr);

		CapsuleCollider* colliderPtr = (CapsuleCollider*)player2Ptr;
		colliderPtr->SetGameObject(player2Ptr);
		collisionManagerPtr->Add(colliderPtr);
	}

	if (GameSelect::selectMobnum == 2) {
		player3Ptr = new Player3();

		gobjPtr = player3Ptr;
		gobjPtr->tag_name = "Player3";
		gameObjectVec.push_back(gobjPtr);
		player3Ptr->SetupCollision(collisionManagerPtr);

		CapsuleCollider* colliderPtr = (CapsuleCollider*)player3Ptr;
		colliderPtr->SetGameObject(player3Ptr);
		collisionManagerPtr->Add(colliderPtr);
	}
	bossPtr->SetPlayer(playerPtr);
	bossPtr->SetPlayer2(player2Ptr);
	bossPtr->SetPlayer3(player3Ptr);
	bossPtr->BossCollision();

	for (int i = 0; i < 10; i++) {
		enemyPtr[i]->SetPlayer(playerPtr);
		enemyPtr[i]->SetPlayer2(player2Ptr);
		enemyPtr[i]->SetPlayer3(player3Ptr);
		enemyPtr[i]->EnemyCollision();
	}
	// エフェクトマネージャー生成
	effectManagerPtr = new EffectManager();
	effectManagerPtr->LoadCSV("Assets/Effect/CSV/EffectList.csv");
	effectManagerPtr->tag_name = "EffectManager";
	gameObjectVec.push_back(effectManagerPtr);

	bossPtr->SetupEffectManager(effectManagerPtr);

	if (playerPtr != nullptr)
		playerPtr->SetupEffectManager(effectManagerPtr);
	if (player2Ptr != nullptr)
		player2Ptr->SetupEffectManager(effectManagerPtr);
	if (player3Ptr != nullptr)
		player3Ptr->SetupEffectManager(effectManagerPtr);

	gameObjectVec.push_back(collisionManagerPtr);

	bossPtr->SetupCollision(collisionManagerPtr);

	for (int i = 0; i < 10; i++) {
		enemyPtr[i]->SetupCollision(collisionManagerPtr);
	}

	//マップモデルロード
	mapMHandle = MV1LoadModel("Assets/low_poly_dungeon_room_2/dungeon.mv1");

	collision.SetModelHandle(mapMHandle);

	BossBGM = LoadSoundMem("Assets/sound/n65.mp3");

	//CreateDirLightHandle(VGet(0.0f, -1.0f, 0.0f));

//    scenemanagerPtr->ChangeScene = true;

}

GameMain::~GameMain() {
	gameObjectVec.clear();
	gameObjectVec.shrink_to_fit();
}

void GameMain::Update() {
	//ボスのBGM再生
	if (bossPtr->ACT_STATUS_CHASE || bossPtr->ACT_STATUS_ATTCK) {
		BGM = true;
	}
	if (CheckSoundMem(BossBGM) == 1) {
		BGM = false;
	}
	//プレイヤーかボスが死ぬと止まる
	if (BGM && !bossPtr->dead && (playerPtr != nullptr && !playerPtr->dead)) {

		ChangeVolumeSoundMem(180, BossBGM);
		PlaySoundMem(BossBGM, DX_PLAYTYPE_LOOP);
	}

	if (BGM && !bossPtr->dead && (player2Ptr != nullptr && !player2Ptr->dead)) {

		ChangeVolumeSoundMem(180, BossBGM);
		PlaySoundMem(BossBGM, DX_PLAYTYPE_LOOP);
	}

	if (BGM && !bossPtr->dead && (player3Ptr != nullptr && !player3Ptr->dead)) {

		ChangeVolumeSoundMem(180, BossBGM);
		PlaySoundMem(BossBGM, DX_PLAYTYPE_LOOP);
	}

	if (bossPtr->dead) {
		Bendcount++;
		StopSoundMem(BossBGM);
	}
	else if ((playerPtr != nullptr && playerPtr->dead) ||
		(player2Ptr != nullptr && player2Ptr->dead) ||
		(player3Ptr != nullptr && player3Ptr->dead)) {
		Pendcount++;
		StopSoundMem(BossBGM);
	}
	//プレイヤーかボスが死ぬとシーンが変わる
	if (Bendcount > 150) {
		sceneManagerPtr->SceneChange("Clear");
	}
	else if (Pendcount > 90) {
		sceneManagerPtr->SceneChange("Over");
	}
	else {
		if (GameSelect::selectMobnum == 1) {
			bossPtr->SetTargetPos(playerPtr->trans);

			for (int i = 0; i < 10; i++) {
				enemyPtr[i]->SetTargetPos(playerPtr->trans);
			}
		}
		if (GameSelect::selectMobnum == 0) {
			bossPtr->SetTargetPos(player2Ptr->trans);

			for (int i = 0; i < 10; i++) {
				enemyPtr[i]->SetTargetPos(player2Ptr->trans);
			}
		}
		if (GameSelect::selectMobnum == 2) {
			bossPtr->SetTargetPos(player3Ptr->trans);
			for (int i = 0; i < 10; i++) {
				enemyPtr[i]->SetTargetPos(player3Ptr->trans);
			}

		}
		//enemyManagerPtr->SetTargetPos(gameObjectVec[0]->trans);

		//DrawFormatString(320, 240, 32000, "Game Main");

		MV1SetPosition(mapMHandle, VGet(0.f, 100.f, 0.f));
		if (playerPtr != nullptr) {
			if (!playerPtr->Dive) {
				// コリジョン判定
				chk_collision();
			}
		}
		else {
			chk_collision();
		}

		// マップモデル描画
		MV1DrawModel(mapMHandle);
		DrawLine3D(VGet(-6900.0f, 100.0f, 200.f), VGet(-6100.0f, 1100.0f, 200.f), GetColor(255, 0,0));
		DrawLine3D(VGet(-6900.0f, 1100.0f, 200.f), VGet(-6100.0f, 100.0f, 200.f), GetColor(255, 0, 0));
		DrawLine3D(VGet(-8850.0f, 1100.0f, 2050.f), VGet(-8850.0f, 100.0f, 2950.f), GetColor(255, 0, 0));
		DrawLine3D(VGet(-8850.0f, 100.0f, 2050.f), VGet(-8850.0f, 1100.0f, 2950.f), GetColor(255, 0, 0));
		//map_collision();

		// 登録されているGameObjectの更新処理実行
		for (int i = 0; i < gameObjectVec.size(); i++) {
			gameObjectVec[i]->Update();
		}
	}
}

void GameMain::chk_collision() {

	//MV1SetPosition(mapMHandle, VGet(320.f, 240.f, 0.f));
	//MATRIX matrix = MV1GetMatrix(mapMHandle);

	//Player* playerPtr = static_cast<Player*>(gameObjectVec[2]);
	//Player2* player2Ptr = static_cast<Player2*>(gameObjectVec[2]);
	//Player3* player3Ptr = static_cast<Player3*>(gameObjectVec[2]);
	VECTOR hitPos;
	VECTOR playerPos = {};
	if (playerPtr != nullptr) {
		playerPos = playerPtr->trans;
	}
	if (player2Ptr != nullptr) {
		playerPos = player2Ptr->trans;
	}
	if (player3Ptr != nullptr) {
		playerPos = player3Ptr->trans;
	}
	playerPos.y += 50.f;

	GameObject* tmp = nullptr;
	tmp = playerPtr;
	if (tmp == nullptr)
		tmp = player2Ptr;
	if (tmp == nullptr)
		tmp = player3Ptr;
	if (collision.CheckRaycast(playerPos, tmp->GetForward(), 50.f, &hitPos)) {
		//DrawFormatString(10, 10, 32000, "Hit");
		if (playerPtr != nullptr)
			playerPtr->SetWallHit(true);

		if (player2Ptr != nullptr)
			player2Ptr->SetWallHit(true);

		if (player3Ptr != nullptr)
			player3Ptr->SetWallHit(true);
	}
	else {
		// 地面の当たり判定
		if (playerPtr != nullptr)
			playerPtr->SetWallHit(false);

		if (player2Ptr != nullptr)
			player2Ptr->SetWallHit(false);

		if (player3Ptr != nullptr)
			player3Ptr->SetWallHit(false);

		if (playerPtr != nullptr) {
			playerPos = playerPtr->trans;
		}
		if (player2Ptr != nullptr) {
			playerPos = player2Ptr->trans;
		}
		if (player3Ptr != nullptr) {
			playerPos = player3Ptr->trans;
		}
		playerPos.y += 100.f;
		hitPos = VGet(0.f, 240.f, 0.f);
		if (playerPtr != nullptr)
			playerPtr->SetGroundHit(false, hitPos);

		if (player2Ptr != nullptr)
			player2Ptr->SetGroundHit(false, hitPos);

		if (player3Ptr != nullptr)
			player3Ptr->SetGroundHit(false, hitPos);
		if (collision.CheckRaycast(playerPos, VGet(0.f, -1.f, 0.f), 110.f, &hitPos)) {
			// 地面にヒット
			//DrawFormatString(10, 60, 32000, "Ground Hit %f", hitPos.y);
			if (playerPtr != nullptr)
				playerPtr->SetGroundHit(true, hitPos);

			if (player2Ptr != nullptr)
				player2Ptr->SetGroundHit(true, hitPos);

			if (player3Ptr != nullptr)
				player3Ptr->SetGroundHit(true, hitPos);
		}
		else {
			hitPos = VGet(0.f, 0.f, 0.f);
			if (playerPtr != nullptr)
				playerPtr->SetGroundHit(false, hitPos);

			if (player2Ptr != nullptr)
				player2Ptr->SetGroundHit(false, hitPos);

			if (player3Ptr != nullptr)
				player3Ptr->SetGroundHit(false, hitPos);
		}
	}
}

void GameMain::map_collision() {
	DrawLine3D(VGet(-1850.0f, 1100.0f, -1850.f), VGet(1850.0f, 100.0f, -1850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 100.0f, -1850.f), VGet(1850.0f, 1100.0f, -1850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(1850.0f, 1100.0f, 2850.f), VGet(1850.0f, 100.0f, -1850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(1850.0f, 100.0f, 2850.f), VGet(1850.0f, 1100.0f, -1850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(1850.0f, 1100.0f, 2850.f), VGet(-1850.0f, 100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(1850.0f, 100.0f, 2850.f), VGet(-1850.0f, 1100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 1100.0f, 800.f), VGet(-1850.0f, 100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 100.0f, 800.f), VGet(-1850.0f, 1100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 1100.0f, -1850.f), VGet(-1850.0f, 100.0f, 200.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 100.0f, -1850.f), VGet(-1850.0f, 1100.0f, 200.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 1100.0f, 800.f), VGet(-6170.0f, 100.0f, 800.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-1850.0f, 100.0f, 800.f), VGet(-6170.0f, 1100.0f, 800.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6830.0f, 1100.0f, 200.f), VGet(-1850.0f, 100.0f, 200.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6830.0f, 100.0f, 200.f), VGet(-1850.0f, 1100.0f, 200.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6830.0f, 1100.0f, 200.f), VGet(-6830.0f, 100.0f, 2150.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6830.0f, 100.0f, 200.f), VGet(-6830.0f, 1100.0f, 2150.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6170.0f, 1100.0f, 2850.f), VGet(-6170.0f, 100.0f, 800.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6170.0f, 100.0f, 2850.f), VGet(-6170.0f, 1100.0f, 800.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8850.0f, 1100.0f, 2150.f), VGet(-6830.0f, 100.0f, 2150.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8850.0f, 100.0f, 2150.f), VGet(-6830.0f, 1100.0f, 2150.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6170.0f, 1100.0f, 2850.f), VGet(-8150.0f, 100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-6170.0f, 100.0f, 2850.f), VGet(-8150.0f, 1100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8850.0f, 1100.0f, 2150.f), VGet(-8850.0f, 100.0f, 7170.f), GetColor(255, 0, 0));
	DrawLine3D(VGet(-8850.0f, 100.0f, 2150.f), VGet(-8850.0f, 1100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8150.0f, 1100.0f, 7170.f), VGet(-8150.0f, 100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8150.0f, 100.0f, 7170.f), VGet(-8150.0f, 1100.0f, 2850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-13800.0f, 1100.0f, 7170.f), VGet(-8850.0f, 100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-13800.0f, 100.0f, 7170.f), VGet(-8850.0f, 1100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8150.0f, 1100.0f, 7170.f), VGet(-4150.0f, 100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-8150.0f, 100.0f, 7170.f), VGet(-4150.0f, 1100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-13800.0f, 1100.0f, 7170.f), VGet(-13800.0f, 100.0f, 11850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-13800.0f, 100.0f, 7170.f), VGet(-13800.0f, 1100.0f, 11850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-4150.0f, 1100.0f, 11850.f), VGet(-4150.0f, 100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-4150.0f, 100.0f, 11850.f), VGet(-4150.0f, 1100.0f, 7170.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-4150.0f, 1100.0f, 11850.f), VGet(-13800.0f, 100.0f, 11850.f), GetColor(255, 255, 255));
	DrawLine3D(VGet(-4150.0f, 100.0f, 11850.f), VGet(-13800.0f, 1100.0f, 11850.f), GetColor(255, 255, 255));
}

