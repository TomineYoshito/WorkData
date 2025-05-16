#include <vector>

#include "SceneManager.h"

SceneManager* SceneManager::scenemanagerPtr = nullptr;

//-------------------------------------------------------------------------------------------
//
// シーンクラスをsceneInfoへ追加したらヘッダーファイルも追加する
//
//-------------------------------------------------------------------------------------------
#include "GameMain.h"
#include "GameTitle.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameAdvertise.h"
#include "GameSelect.h"

struct SceneInfo {
	int scene_id;				// シーンID
	string scene_name;			// シーン名
	string scene_class_name;	// シーンクラス名
};

//-------------------------------------------------------------------------------------------
//
// シーン情報のテーブル
//
// シーンを追加する場合は、ここにシーン情報を追記していく
// 追記したら、create_scene関数内で、シーンを生成して、scene_ptrポインタ変数へ代入する。
//
//-------------------------------------------------------------------------------------------
static vector<SceneInfo> sceneInfo = {	
	{ 0, "Main", "GameMain" },
	{ 1, "title", "GameTitle" },
	{ 2, "advertise", "GameAdvertise" },
	{ 3, "Clear", "GameClear"},
	{ 4, "Over", "GameOver"},
	{ 5, "Select", "GameSelect"},
};

void SceneManager::DeleteInstance() {
	delete scenemanagerPtr;
}


SceneManager::~SceneManager()
{
	// シーン破棄
	if (scene_ptr != NULL) delete scene_ptr;
}

Scene* SceneManager::create_scene(string scene_class_name)
{
	Scene* scene_ptr = NULL;

	if (scene_class_name == "GameTitle") {
		// タイトル
		scene_ptr = new GameTitle();
		ChangeScene = false;
	}
	else if (scene_class_name == "GameAdvertise") {
		scene_ptr = new GameAdvertise();
	}
	else if (scene_class_name == "GameMain") {
		// インゲーム
		scene_ptr = new GameMain();
		ChangeScene = true;
	}
	else if (scene_class_name == "GameClear") {
		scene_ptr = new GameClear();
		ChangeScene = true;
	}
	else if (scene_class_name == "GameOver") {
		scene_ptr = new GameOver();
		ChangeScene = true;
	}
	else if (scene_class_name == "GameSelect") {
		scene_ptr = new GameSelect();
	}
	else if (scene_class_name == "シーンが増えて制御するクラスが増えたらこの条件文を増やす") {
		// scene_class_nameは、Scene.csvのClass Name列に記述された文字列で判断する
	}

	return scene_ptr;
}


void SceneManager::SceneChange(string scene_name)
{
	if (scene_ptr != NULL) {
		delete scene_ptr;
	}

	// シーン情報がテーブル定義の場合の処理
	for (int i = 0; i < sceneInfo.size(); i++) {
		if (sceneInfo[i].scene_name == scene_name) {
			this->scene_ptr = create_scene(sceneInfo[i].scene_class_name);
			if (scene_ptr != NULL) {
				this->scene_ptr->scene_name = scene_name;
				this->scene_name = scene_name;
				this->scene_id = sceneInfo[i].scene_id;
				break;
			}
		}
	}
}


void SceneManager::SceneChangeByID(int scene_id)
{
}


void SceneManager::Execute()
{
	// シーンのフレーム更新処理呼び出し
	this->scene_ptr->Update();

	// シーンの実行状態監視
	string scene_name = scene_ptr->SceneSurveillance();
	if (scene_name != "") {
		if (scene_name != this->scene_name) {
			// シーンの変更指示があったのでシーンを変更する
			SceneChange(scene_name);
		}
	}

	//DrawFormatString(50, 10, 65535, "scene %s", this->scene_name.c_str());
}

vector<string> SceneManager::GetSceneNameList()
{
	vector<string> sceneList;
	for (int i = 0; i < sceneInfo.size(); i++) {
		sceneList.push_back(sceneInfo[i].scene_name);
	}

	return sceneList;
}
