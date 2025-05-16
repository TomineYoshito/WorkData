#include "Scene.h"
#include "SceneManager.h"

Scene::Scene()
{
	scene_id = 0;
	scene_name = "";
	scene_change_name = "";

	frame_counter = 0;
}

Scene::~Scene()
{
}


void Scene::SceneChange(string name)
{
	scene_change_name = name;
}


string Scene::SceneSurveillance()
{
	if (scene_change_name != "") {
		// シーン変更指示があった
		if (scene_name != scene_change_name) {
			// 現在再生中のシーン名と不一致になったのでシーン変更されたシーン名を上位クラスの
			// SceneManagerへ返す
			string name = scene_change_name;
			scene_change_name = "";
			return name;
		}
	}

	return scene_change_name;
}
