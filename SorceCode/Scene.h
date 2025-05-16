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
	//! �V�[��ID
	int scene_id;

	//! �V�[����
	string scene_name;

	int frame_counter;

	Scene();
	virtual ~Scene() = 0;								// Scene��delete�������A�h���N���X�̃f�X�g���N�^���Ă΂��悤�ɂ��邽�߉��z�֐��ɂ���
	virtual void Update() = 0;							// �V�[���̍X�V�i�h���N���X���Œ�`����֐��j

	string SceneSurveillance();
	void SceneChange(string scene_name);
};



