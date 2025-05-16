#pragma once
#include "Dxlib.h"
#include <math.h>
#include <string>
#include <vector>
#include "Player.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Enemy.h"
#include "Boss.h"
#include "EffectManager.h"
#include "Player2.h"
#include "GameSelect.h"
#include "Player3.h"

using namespace std;

class GameMain : public Scene
{
	VECTOR cameraPos;
	VECTOR lookAtPos;
	vector<GameObject*> gameObjectVec;

	//Player* playerPtr;
	vector <int> mapCubeFrameIndexVec;
	Collision collision;

	CollisionManager* collisionManagerPtr;
public:
	GameMain();     //コンストラクタ
	~GameMain();    //デストラクタ

	void Update();

	int mapMHandle;
private:
	int ShadowMapHandle;
	int Bendcount;
	int Pendcount;
	int BossBGM;
	int ClearBGM;
	bool BGM;

	void chk_collision();
	void map_collision();
	SceneManager* sceneManagerPtr;
	Boss* bossPtr;
	BossAttack* bossAPtr = new BossAttack();
	Enemy* enemyPtr[10];

	Player* playerPtr;
	Player2* player2Ptr;
	Player3* player3Ptr;
	EffectManager* effectManagerPtr;
};

