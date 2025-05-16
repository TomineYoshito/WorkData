#include <vector>

#include "SceneManager.h"

SceneManager* SceneManager::scenemanagerPtr = nullptr;

//-------------------------------------------------------------------------------------------
//
// �V�[���N���X��sceneInfo�֒ǉ�������w�b�_�[�t�@�C�����ǉ�����
//
//-------------------------------------------------------------------------------------------
#include "GameMain.h"
#include "GameTitle.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameAdvertise.h"
#include "GameSelect.h"

struct SceneInfo {
	int scene_id;				// �V�[��ID
	string scene_name;			// �V�[����
	string scene_class_name;	// �V�[���N���X��
};

//-------------------------------------------------------------------------------------------
//
// �V�[�����̃e�[�u��
//
// �V�[����ǉ�����ꍇ�́A�����ɃV�[������ǋL���Ă���
// �ǋL������Acreate_scene�֐����ŁA�V�[���𐶐����āAscene_ptr�|�C���^�ϐ��֑������B
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
	// �V�[���j��
	if (scene_ptr != NULL) delete scene_ptr;
}

Scene* SceneManager::create_scene(string scene_class_name)
{
	Scene* scene_ptr = NULL;

	if (scene_class_name == "GameTitle") {
		// �^�C�g��
		scene_ptr = new GameTitle();
		ChangeScene = false;
	}
	else if (scene_class_name == "GameAdvertise") {
		scene_ptr = new GameAdvertise();
	}
	else if (scene_class_name == "GameMain") {
		// �C���Q�[��
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
	else if (scene_class_name == "�V�[���������Đ��䂷��N���X���������炱�̏������𑝂₷") {
		// scene_class_name�́AScene.csv��Class Name��ɋL�q���ꂽ������Ŕ��f����
	}

	return scene_ptr;
}


void SceneManager::SceneChange(string scene_name)
{
	if (scene_ptr != NULL) {
		delete scene_ptr;
	}

	// �V�[����񂪃e�[�u����`�̏ꍇ�̏���
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
	// �V�[���̃t���[���X�V�����Ăяo��
	this->scene_ptr->Update();

	// �V�[���̎��s��ԊĎ�
	string scene_name = scene_ptr->SceneSurveillance();
	if (scene_name != "") {
		if (scene_name != this->scene_name) {
			// �V�[���̕ύX�w�����������̂ŃV�[����ύX����
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
