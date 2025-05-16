#pragma once
#include "Scene.h"

class SceneManager
{
private:
	//シングルトン
	static SceneManager* scenemanagerPtr;
	SceneManager(){}

public:
	//自身のインスタンスを取得する関数
	static SceneManager* GetInstance() {
		if (scenemanagerPtr == nullptr)scenemanagerPtr = new SceneManager{};
		return scenemanagerPtr;
	}

	void DeleteInstance();

public:
	~SceneManager();

	void Execute();										// シーン実行
	void SceneChange(string id);						// 名前でシーン変更
	void SceneChangeByID(int scene_id);					// IDでシーン変更

	vector<string> GetSceneNameList();

	bool ChangeScene;

private:
	int scene_id;										// 現在実行中のシーンID
	string scene_name;									// 現在実行中のシーン名
	string load_scene_name;								// ロード指示しているシーン名
	int load_scene_id;									// ロード指示しているシーンID
	Scene* scene_ptr;									// 現在実行中のシーンのポインタ

private:
	Scene* create_scene(string scene_class_name);		// シーン生成
};
