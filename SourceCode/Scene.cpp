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
		// �V�[���ύX�w����������
		if (scene_name != scene_change_name) {
			// ���ݍĐ����̃V�[�����ƕs��v�ɂȂ����̂ŃV�[���ύX���ꂽ�V�[��������ʃN���X��
			// SceneManager�֕Ԃ�
			string name = scene_change_name;
			scene_change_name = "";
			return name;
		}
	}

	return scene_change_name;
}
