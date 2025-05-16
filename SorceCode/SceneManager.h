#pragma once
#include "Scene.h"

class SceneManager
{
private:
	//�V���O���g��
	static SceneManager* scenemanagerPtr;
	SceneManager(){}

public:
	//���g�̃C���X�^���X���擾����֐�
	static SceneManager* GetInstance() {
		if (scenemanagerPtr == nullptr)scenemanagerPtr = new SceneManager{};
		return scenemanagerPtr;
	}

	void DeleteInstance();

public:
	~SceneManager();

	void Execute();										// �V�[�����s
	void SceneChange(string id);						// ���O�ŃV�[���ύX
	void SceneChangeByID(int scene_id);					// ID�ŃV�[���ύX

	vector<string> GetSceneNameList();

	bool ChangeScene;

private:
	int scene_id;										// ���ݎ��s���̃V�[��ID
	string scene_name;									// ���ݎ��s���̃V�[����
	string load_scene_name;								// ���[�h�w�����Ă���V�[����
	int load_scene_id;									// ���[�h�w�����Ă���V�[��ID
	Scene* scene_ptr;									// ���ݎ��s���̃V�[���̃|�C���^

private:
	Scene* create_scene(string scene_class_name);		// �V�[������
};
