#pragma once
#include <vector>
#include <string>

#include "DxLib.h"

using namespace std;

class Scene
{
private:
	string scene_change_name;

public:
	//! シーンID
	int scene_id;

	//! シーン名
	string scene_name;

	int frame_counter;

	Scene();
	virtual ~Scene() = 0;								// Sceneをdeleteした時、派生クラスのデストラクタも呼ばれるようにするため仮想関数にする
	virtual void Update() = 0;							// シーンの更新（派生クラス側で定義する関数）

	string SceneSurveillance();
	void SceneChange(string scene_name);
};



